//
// Created by laurent on 11/01/2026.
//

#ifndef NET8_NET8PROTOCOL_H
#define NET8_NET8PROTOCOL_H

#include <map>
#include <functional>
#include <string>
#include <stdexcept>

#include "Playground.h"

class Server;

class Net8Protocol {
public:
    Net8Protocol(Server *server);

    void send_to_room(int game_id, const std::string &message) const;
    bool is_identified(int socket) const;

    void handle_message(int socket, const std::string &message);

    void on_connect(int socket);
    void on_disconnect(int socket);
    void on_ident(int socket, const std::string &name);
    void on_chat(int socket, const std::string &message);

    class protocol_error : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };
private:

    Server *m_server;
    Playground m_playground;
    std::map<std::string, std::function<void(int, const std::string &)>> m_commands;
};


#endif //NET8_NET8PROTOCOL_H