//
// Created by laurent on 29/12/2025.
//

#include "Rooms.h"

#include <algorithm>
#include "Net8Protocol.h"

Rooms::Rooms(Net8Protocol *protocol) : m_protocol(protocol) {
    add_room("default");
    m_rooms[0].set_deletable(false);
}

int Rooms::add_room(const std::string &name) {
    auto it = std::find_if(m_rooms.begin(), m_rooms.end(), [](const Game &game) { return game.is_active(); });
    if (it != m_rooms.end()) {
        it->reset();
        it->set_name(name);
        it->set_active(true);
        m_protocol->announce_new_room(&*it);
        return std::distance(m_rooms.begin(), it);
    } else {
        m_rooms.emplace_back(m_protocol, name, m_rooms.size());
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
    player->set_game_id(game_id);
}

const std::list<const Player *> Rooms::get_players(int game_id) const {
    return m_rooms[game_id].get_players();
}
