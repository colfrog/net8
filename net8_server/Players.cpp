//
// Created by laurent on 02/01/2026.
//

#include "Players.h"

#include <algorithm>

void Players::add_player(int socket, const std::string &name) {
    m_players.emplace_back(socket, name);
    m_player_of_socket[socket] = &m_players.back();
}

Player *Players::get(int socket) {
    if (m_player_of_socket.contains(socket))
        return m_player_of_socket[socket];
    return nullptr;
}

const Player *Players::get(int socket) const {
    if (m_player_of_socket.contains(socket))
        return m_player_of_socket.at(socket);
    return nullptr;
}

void Players::remove(Player *player) {
    m_player_of_socket.erase(player->get_socket());
    m_players.remove(*player);
}

bool Players::exists(int socket) const {
    return m_player_of_socket.contains(socket);
}
