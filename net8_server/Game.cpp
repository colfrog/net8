//
// Created by laurent on 22/12/2025.
//

#include "Game.h"
#include "Net8Protocol.h"

Game::Game(Net8Protocol *protocol, std::string name) : m_protocol(protocol), m_name(std::move(name)) {}

void Game::reset() {
    m_deck.reset();
}

void Game::new_game() {
    m_deck.reset();
    for (Player *player : m_players) {
        player->get_hand().clear();
        player->get_hand().draw_n(this, 8);
        m_protocol->announce_hand(player);
    }
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

int Game::get_player_count() const {
    return m_players.size();
}

bool Game::fits_rules(const Card *card) {
    const Card *top_card = m_pile.top_card();
    return card->custom_rules(this) || top_card->get_type() == card->get_type() || top_card->get_face() == card->get_face();
}

void Game::do_turn(Player *player, bool play, int card_index, const std::string &arg) {
    if (player != m_players.front())
        throw game_error("It's not your turn!");

    Hand &hand = player->get_hand();
    if (card_index < 0 || card_index >= hand.count())
        throw Net8Protocol::protocol_error("Card index out of range");

    if (play) {
        hand.play(this, card_index, arg);
        if (hand.count() == 0) {
            m_protocol->send_to_room(player->get_socket(), "CHAT:" + player->get_name() + "won!");
            m_protocol->send_to_room(player->get_socket(), "Starting new game...");
            new_game();
        }

    }
}