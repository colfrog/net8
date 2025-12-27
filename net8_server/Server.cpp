//
// Created by laurent on 22/12/2025.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdexcept>
#include <iostream>

#include "Server.h"

Server::Server(int port) {
    std::cout << "Creating socket..." << std::endl;
    m_server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_server_socket < 0)
        throw std::runtime_error("Error creating socket");

    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    std::cout << "Binding socket..." << std::endl;
    if (bind(m_server_socket, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) < 0)
        throw std::runtime_error("Error binding socket");

    if (fcntl(m_server_socket, F_SETFL, O_NONBLOCK) < 0)
        throw std::runtime_error("Cannot set the socket to nonblocking");
    listen(m_server_socket, m_max_connections);

#ifdef __linux__
    m_epoll_fd = epoll_create1(0);
    if (m_epoll_fd < 0)
        throw std::runtime_error("Error creating epoll");

    std::cout << "Registering with epoll..." << std::endl;
    epoll_event ev = {};
    ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
    ev.data.fd = m_server_socket;
    if (epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, m_server_socket, &ev) < 0)
        throw std::runtime_error("Error adding epoll event for server");
#else
    m_kqueue_fd = kqueue();
    if (m_kqueue_fd < 0)
        throw std::runtime_error("Error creating kqueue");

    std::cout << "Registering with kqueue..." << std::endl;
    struct kevent ev = {};
    ev.ident = m_server_socket;
    ev.flags = EV_ADD;
    ev.filter = EVFILT_READ;
    kevent(m_kqueue_fd, &ev, 1, nullptr, 0, nullptr);
#endif

    m_rooms.emplace_back(this, std::string("default"));
}

#ifndef __linux__
struct kevent prepare_event_to_add(int fd) {
    struct kevent ev = {};
    ev.ident = fd;
    ev.flags = EV_ADD;
    ev.filter = EVFILT_READ;
    return ev;
}

struct kevent prepare_event_to_del(int fd) {
    struct kevent ev = {};
    ev.ident = fd;
    ev.flags = EV_DELETE;
    return ev;
}
#endif

void Server::run() {
    int events_ready, fd;
#ifdef __linux__
    struct epoll_event events[m_max_events];
#else
    struct kevent events[m_max_events];
    std::vector<struct kevent> change_events;
    struct timespec timeout = {0, m_timeout*1000000};
#endif

    std::string message;
    char buffer[1024];
    ssize_t num_chars = 0;
    while (true) {
#ifdef __linux__
        events_ready = epoll_wait(m_epoll_fd, events, m_max_events, m_timeout);
        if (events_ready < 0)
            throw std::runtime_error("epoll error");
#else
        events_ready = kevent(m_kqueue_fd, change_events.data(), change_events.size(), events, m_max_events, &timeout);
        change_events.clear();
        if (events_ready < 0)
            throw std::runtime_error("kqueue error");
#endif

        for (int i = 0; i < events_ready; i++) {
#ifdef __linux__
            fd = events[i].data.fd;
#else
            fd = events[i].ident;
#endif
            if (fd == m_server_socket) {
                std::cout << "New connection" << std::endl;
                const int new_conn = accept(fd, nullptr, nullptr);
#ifndef __linux__
                change_events.push_back(prepare_event_to_add(new_conn));
#endif
                add_client(new_conn);
            } else {
                std::cout << "Received event from client " << events[i].filter << std::endl;
#ifdef __linux__
                if ((events[i].events&EPOLLIN) != 0) {
#else
                if ((events[i].filter&EVFILT_READ) != 0) {
#endif
                    std::cout << "Received message" << std::endl;
                    while ((num_chars = read(fd, buffer, sizeof(buffer))) > 0) {
                        message += std::string(buffer, num_chars);
                    }
                    receive_message(fd, message);
                    message.clear();
                }
#ifdef __linux__
                if ((events[i].events&EPOLLHUP) != 0) {
#else
                if ((events[i].flags&EV_EOF) != 0) {
                    change_events.push_back(prepare_event_to_del(fd));
#endif
                    std::cout << "Connection closed" << std::endl;
                    remove_client(fd);
                }
            }
        }
    }
}

void Server::add_client(int socket) {
    // On macOS/BSD we register the client in a single system call while asking for the next event
#ifdef __linux__
    epoll_event ev = {};
    ev.events = EPOLLIN|EPOLLET|EPOLLRDHUP;
    ev.data.fd = socket;
#endif

    if (fcntl(socket, F_SETFL, O_NONBLOCK) < 0)
        throw std::runtime_error("Error setting client socket to nonblocking");
#ifdef __linux__
    if (epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, socket, &ev) < 0)
        throw std::runtime_error("Error adding epoll event for client");
#endif

    m_client_sockets.push_back(socket);
}

void Server::remove_client(int socket) {
    // Same as add_client, we do it in the kqueue loop on macOS/BSD
#ifdef __linux__
    epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, socket, nullptr);
#endif

    m_client_sockets.remove(socket);
    m_room_of_client[socket]->remove_player(socket);
    close(socket);
}

void Server::receive_message(int socket, const std::string &message) {
    std::cout << message << std::endl;
    write(socket, message.c_str(), message.size());
}