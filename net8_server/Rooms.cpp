//
// Created by laurent on 29/12/2025.
//

#include "Rooms.h"

#include <algorithm>

Rooms::Rooms(Server *server) : m_server{server} {
    add_room("default");
}

int Rooms::add_room(const std::string &name) {
    auto it = std::find_if(m_rooms.begin(), m_rooms.end(), [](const Game &game) { return game.is_active(); });
    if (it != m_rooms.end()) {
        it->reset();
        it->set_name(name);
        it->set_active(true);
        return std::distance(m_rooms.begin(), it);
    } else {
        m_rooms.emplace_back(m_server, name);
        return m_rooms.size() - 1;
    }
}

void Rooms::add_player(Player *player) {
    m_rooms[0].add_player(player);
}

void Rooms::remove_player(Player *player) {
    Game *room = m_room_of_player[player];
    room->remove_player(player);
}

void Rooms::transfer_player(Player *player, int game_id) {
    Game *current_room = m_room_of_player[player];
    current_room->remove_player(player);
    m_rooms[game_id].add_player(player);
}

const std::list<Player *> Rooms::get_players(int game_id) const {
    return m_rooms[game_id].get_players();
}
