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

#include <list>
#include <string>

#include "Playground.h"

class Server {
public:
    Server(int port);
    void run();
    void send_to_room(int game_id, const std::string &message) const;
    void send_to_one(int socket, const std::string &message) const;

private:
    void add_client(int socket);
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
    std::list<int> m_client_sockets;
    int m_max_connections = 128;
    Playground m_playground{this};
};


#endif //NET8_SERVER_H
