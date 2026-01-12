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
    void fail_if_unidentified(int socket) const;

    void handle_message(int socket, const std::string &message);
    void log_message(bool out, int socket, const std::string &message) const;

    void announce_hand(const Player *player) const;
    void announce_join(const Player *player) const;
    void announce_part(const Player *player) const;
    void announce_leave(const Player *player) const;
    void announce_spectator(const Player *player) const;
    void announce_turn(const Player *player) const;
    void announce_top_card(const Game *game) const;
    void announce_inactive_room(const Game *game) const;
    void announce_new_room(const Game *game) const;

    void on_connect(int socket);
    void on_disconnect(int socket);

    class protocol_error : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };
private:
    void on_ident(int socket, const std::string &name);
    void on_chat(int socket, const std::string &message);
    void on_addroom(int socket, const std::string &message);
    void on_leaveroom(int socket, const std::string &message);
    void on_switchroom(int socket, const std::string &message);
    void on_joingame(int socket, const std::string &message);
    void on_partgame(int socket, const std::string &message);
    void on_play(int socket, const std::string &message);
    void on_draw(int socket, const std::string &message);

    Server *m_server;
    Playground m_playground;
    std::map<std::string, std::function<void(int, const std::string &)>> m_commands;
};


#endif //NET8_NET8PROTOCOL_H