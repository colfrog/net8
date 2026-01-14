//
// Created by laurent on 29/12/2025.
//

#include "Rooms.h"

#include <algorithm>
#include <sstream>
#include "Net8Protocol.h"

Rooms::Rooms(Net8Protocol *protocol) : m_protocol(protocol) {
    add_room("default");
    m_rooms[0].set_deletable(false);
}

int Rooms::add_room(const std::string &name) {
    auto it = std::find_if(m_rooms.begin(), m_rooms.end(), [](const Game &game) { return !game.is_active(); });
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

Game *Rooms::get_room(int game_id) {
    if (game_id < 0 || game_id >= m_rooms.size())
        throw Net8Protocol::protocol_error("Invalid game id");
    return &m_rooms.at(game_id);
}

const Game *Rooms::get_room(int game_id) const {
    if (game_id < 0 || game_id >= m_rooms.size())
        throw Net8Protocol::protocol_error("Invalid game id");
    return &m_rooms.at(game_id);
}

void Rooms::player_join(Player *player) {
    Game *room = m_room_of_player[player];
    room->join_game(player);
}

void Rooms::player_part(Player *player) {
    Game *room = m_room_of_player[player];
    room->part_game(player);
}

void Rooms::add_player(Player *player) {
    m_rooms[0].add_player(player);
    m_room_of_player[player] = &m_rooms[0];
}

void Rooms::remove_player(Player *player) {
    if (m_room_of_player.contains(player)) {
        Game *room = m_room_of_player[player];
        room->remove_player(player);
        m_room_of_player.erase(player);
    }
}

void Rooms::transfer_player(Player *player, int game_id) {
    Game *current_room = m_room_of_player[player];
    current_room->remove_player(player);
    m_rooms[game_id].add_player(player);
    player->set_game_id(game_id);
}

std::list<const Player *> Rooms::get_players(int game_id) const {
    return m_rooms[game_id].get_players();
}

std::string Rooms::to_string() const {
    std::stringstream ss;
    int count = 0;
    for (const Game &room : m_rooms) {
        ss << room.get_game_id() << "-" << room.get_name();
        if (count++ < m_rooms.size() - 1)
            ss << ":";
    }

    return ss.str();
}
