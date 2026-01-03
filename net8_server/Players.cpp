//
// Created by laurent on 02/01/2026.
//

#include "Players.h"

#include <algorithm>

void Players::add_player(int socket, const std::string &name) {
    m_players.emplace_back(socket, name);
    m_player_of_socket.insert({socket, &*m_players.end()});
}

Player *Players::get(int socket) {
    return m_player_of_socket[socket];
}

void Players::remove(Player *player) {
    m_player_of_socket.erase(player->get_socket());
    m_players.remove(*player);
}