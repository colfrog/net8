//
// Created by laurent on 02/01/2026.
//

#include "Playground.h"

Playground::Playground(Server *server) : m_server{server}, m_rooms{server} {}

int Playground::add_room(const std::string &name) {
    return m_rooms.add_room(name);
}

void Playground::add_player(int socket, const std::string &name) {
    m_players.add_player(socket, name);
    m_rooms.add_player(m_players.get(socket));
}

void Playground::remove_player(int socket) {
    if (m_players.exists(socket)) {
        Player *player = m_players.get(socket);
        m_rooms.remove_player(player);
        m_players.remove(player);
    }
}

void Playground::transfer_player(int socket, int game_id) {
    Player *player = m_players.get(socket);
    m_rooms.transfer_player(player, game_id);
}

const std::list<Player *> Playground::get_players(int game_id) const {
    return m_rooms.get_players(game_id);
}
