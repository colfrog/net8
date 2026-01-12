//
// Created by laurent on 11/01/2026.
//

#include <string>
#include <iostream>

#include "Net8Protocol.h"
#include "Server.h"

Net8Protocol::Net8Protocol(Server *server) : m_server{server}, m_playground{this} {
    // Commands initialisation
    m_commands["IDENT"] = [this](int s, const std::string &m){ on_ident(s, m); };
    m_commands["CHAT"] = [this](int s, const std::string &m){ on_chat(s, m); };
}

void Net8Protocol::send_to_room(int game_id, const std::string &message) const {
    const std::list<Player *> players = m_playground.get_players(game_id);
    for (const Player *player : players) {
        m_server->send_message(player->get_socket(), message);
    }
}

bool Net8Protocol::is_identified(int socket) const {
    return m_playground.get_player(socket) != nullptr;
}

void Net8Protocol::fail_if_unidentified(int socket) const {
    if (!is_identified(socket)) throw protocol_error("Player not identified");
}

void Net8Protocol::handle_message(int socket, const std::string &message) {
    int command_end = message.find(':');
    std::string command = message.substr(0, command_end);
    std::string arg = message.substr(command_end + 1);
    if (m_commands.contains(command))
        m_commands[command](socket, arg);
    else throw protocol_error("Received unknown command " + command);
}

void Net8Protocol::log_message(bool out, int socket, const std::string &message) const {
    const Player *player = m_playground.get_player(socket);
    const std::string ip4 = m_server->get_ip4_addr(socket);
    std::cout << std::string(out ? "<--" : "-->") << std::string(player ? player->get_name() : "UNIDENTIFIED") << " (" << ip4 << ") - " << message << std::endl;
}

void Net8Protocol::on_connect(int socket) {
    m_server->send_message(socket, "IDENT");
}

void Net8Protocol::on_disconnect(int socket) {
    m_playground.remove_player(socket);
}

void Net8Protocol::on_ident(int socket, const std::string &name) {
    m_playground.add_player(socket, name);
}

void Net8Protocol::on_chat(int socket, const std::string &message) {
    const Player *player = m_playground.get_player(socket);
    send_to_room(player->get_game_id(), "CHAT:" + player->get_name() + ":" + message);
}