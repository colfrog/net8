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

Server::Server() {
    std::cout << "Creating socket..." << std::endl;
    m_server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_server_socket < 0)
        throw std::runtime_error("Error creating socket");

    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(11111);
    addr.sin_addr.s_addr = INADDR_ANY;

    std::cout << "Binding socket..." << std::endl;
    if (bind(m_server_socket, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) < 0)
        throw std::runtime_error("Error binding socket");

    if (fcntl(m_server_socket, F_SETFL, O_NONBLOCK) < 0)
        throw std::runtime_error("Cannot set the socket to nonblocking");
    listen(m_server_socket, m_max_connections);

    epoll_fd = epoll_create1(0);
    if (epoll_fd < 0)
        throw std::runtime_error("Error creating epoll");

    std::cout << "Registering with epoll..." << std::endl;
    epoll_event ev = {};
    ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
    ev.data.fd = m_server_socket;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, m_server_socket, &ev) < 0)
        throw std::runtime_error("Error adding epoll event for server");

    m_rooms.emplace_back(this, std::string("default"));
}

void Server::run() {
    int events_ready, max_events = 16;
    int fd;
    struct epoll_event events[max_events];
    std::string message;
    char buffer[1024];
    ssize_t num_chars = 0;
    while (true) {
        events_ready = epoll_wait(epoll_fd, events, max_events, 10);
        for (int i = 0; i < events_ready; i++) {
            fd = events[i].data.fd;
            if (fd == m_server_socket) {
                const int new_conn = accept(fd, nullptr, nullptr);
                add_client(new_conn);
            } else {
                if ((events[i].events&EPOLLIN) != 0) {
                    while ((num_chars = read(fd, buffer, sizeof(buffer))) > 0) {
                        message += std::string(buffer, num_chars);
                    }
                    receive_message(fd, message);
                    message.clear();
                }
                if ((events[i].events&EPOLLHUP) != 0) {
                    remove_client(fd);
                }
            }
        }
    }
}

void Server::add_client(int socket) const {
    epoll_event ev = {};
    ev.events = EPOLLIN|EPOLLET|EPOLLRDHUP;
    ev.data.fd = socket;
    if (fcntl(socket, F_SETFL, O_NONBLOCK) < 0)
        throw std::runtime_error("Error setting client socket to nonblocking");
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket, &ev) < 0)
        throw std::runtime_error("Error adding epoll event for client");
}

void Server::remove_client(int socket) {
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, socket, nullptr);
    m_client_sockets.remove(socket);
    m_room_of_client[socket]->remove_player(socket);
    close(socket);
}

void Server::receive_message(int socket, const std::string &message) {
    write(socket, message.c_str(), message.size());
}