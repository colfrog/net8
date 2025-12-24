//
// Created by laurent on 22/12/2025.
//

#ifndef NET8_SERVER_H
#define NET8_SERVER_H

#ifdef __linux__
#include <sys/epoll.h>
#elif defined(__OpenBSD__) || defined(__FreeBSD__) || defined(__APPLE__)
#include <sys/event.h>
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
    void add_client(int socket) const;
    void remove_client(int socket);
    void receive_message(int socket, const std::string &message);

    int m_server_socket;
    int epoll_fd;
    std::list<int> m_client_sockets;
    std::vector<Game> m_rooms;
    std::map<int, Game*> m_room_of_client;
    int m_max_connections = 128;
};


#endif //NET8_SERVER_H