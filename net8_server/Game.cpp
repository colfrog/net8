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
    if (m_players.empty() && m_deletable) {
        m_active = false;
    }
}

const std::list<Player *> &Game::get_players() const {
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

bool Game::is_deletable() const {
    return m_deletable;
}

void Game::set_deletable(bool deletable) {
    m_deletable = deletable;
}

Deck &Game::get_deck() {
    return m_deck;
}

Pile &Game::get_pile() {
    return m_pile;
}

bool Game::fits_rules(const Card *card) {
    const Card *top_card = m_pile.top_card();
    return card->custom_rules(this) || top_card->get_type() == card->get_type() || top_card->get_face() == card->get_face();
}