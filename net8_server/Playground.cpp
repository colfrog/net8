//
// Created by laurent on 02/01/2026.
//

#include "Playground.h"

Playground::Playground(Net8Protocol *protocol) : m_protocol{protocol}, m_rooms{protocol} {}

int Playground::add_room(const std::string &name) {
    return m_rooms.add_room(name);
}

Game *Playground::get_room(int game_id) {
    return m_rooms.get_room(game_id);
}

const Game *Playground::get_room(int game_id) const {
    return m_rooms.get_room(game_id);
}

void Playground::player_join(int socket) {
    Player *player = m_players.get(socket);
    m_rooms.player_join(player);
}

void Playground::player_part(int socket) {
    Player *player = m_players.get(socket);
    m_rooms.player_part(player);
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

Player *Playground::get_player(int socket) {
    return m_players.get(socket);
}

const Player *Playground::get_player(int socket) const {
    return m_players.get(socket);
}

const std::list<const Player *> Playground::get_players(int game_id) const {
    return m_rooms.get_players(game_id);
}

const Rooms &Playground::get_rooms() const {
    return m_rooms;
}