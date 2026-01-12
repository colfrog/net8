//
// Created by laurent on 22/12/2025.
//

#ifndef NET8_SERVER_H
#define NET8_SERVER_H

#ifdef __linux__
#include <sys/epoll.h>
#else
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#endif

#include <map>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Net8Protocol.h"

class Server {
public:
    Server(int port);
    void run();
    std::string get_ip4_addr(int socket) const;
    void send_message(int socket, const std::string &message) const;
    void send_to_clients(const std::string &message) const;

private:
    void add_client(int socket, sockaddr_in addr);
    void remove_client(int socket);
    void receive_message(int socket, const std::string &message);

    int m_server_socket;
#ifdef __linux__
    int m_epoll_fd;
#else
    int m_kqueue_fd;
#endif
    int m_max_events = 16;
    int m_timeout = 10; // ms
    std::map<int, in_addr> m_clients;
    int m_max_connections = 128;
    Net8Protocol m_protocol{this};
};


#endif //NET8_SERVER_H
