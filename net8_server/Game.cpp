//
// Created by laurent on 22/12/2025.
//

#include "Game.h"

Game::Game(Server *server, std::string name) : m_server(server), m_name(std::move(name)) {}

void Game::reset() {
    m_deck.set_n_players(0);
    m_deck.rebuild(m_pile);
}

void Game::add_player(Player *player) {
    m_players.push_back(player);
    player->set_playing(false);
}

void Game::remove_player(Player *player) {
    m_players.remove(player);
    if (m_players.empty() && m_game_id != 0) {
        m_active = false;
    }
}

const std::list<Player *> Game::get_players() const {
    return m_players;
}

void Game::set_name(std::string name) {
    m_name = std::move(name);
}

const std::string &Game::get_name() const {
    return m_name;
}

bool Game::is_active() const {
    return m_active;
}

void Game::set_active(bool active) {
    m_active = active;
}
