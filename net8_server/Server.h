//
// Created by laurent on 22/12/2025.
//

#ifndef NET8_SERVER_H
#define NET8_SERVER_H

#ifdef __linux__
#include <sys/epoll.h>
#else
#include <sys/event.h>
#include <sys/time.h>
#endif

#include <list>
#include <map>
#include <vector>
#include <string>

#include "Game.h"

class Server {
public:
    Server(int port);
    void run();
    void send_to_room(int game_id, std::string message);
    void send_to_one(int socket, std::string message);

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
    std::vector<Game> m_rooms;
    std::map<int, Game*> m_room_of_client;
    int m_max_connections = 128;
};


#endif //NET8_SERVER_H