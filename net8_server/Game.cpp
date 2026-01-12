//
// Created by laurent on 22/12/2025.
//

#include "Game.h"
#include "Net8Protocol.h"

Game::Game(Net8Protocol *protocol, std::string name, int game_id) : m_protocol{protocol}, m_name{std::move(name)}, m_game_id{game_id} {
    m_protocol->announce_new_room(this);
}

void Game::reset() {
    m_deck.reset();
}

void Game::new_game() {
    m_deck.reset();
    Card *top_card = m_deck.draw();
    m_pile.add(top_card);
    for (Player *player : m_players) {
        player->get_hand().clear();
        player->get_hand().draw_n(this, 8);
        m_protocol->announce_hand(player);
    }
    m_protocol->announce_top_card(this);
    m_protocol->announce_turn(m_players.front());
}

void Game::add_player(Player *player) {
    m_spectators.push_back(player);
    player->set_playing(false);
    m_protocol->announce_spectator(player);
}

void Game::remove_player(Player *player) {
    if (player->is_playing()) {
        bool is_current = player == m_players.front();
        m_players.remove(player);
        m_protocol->announce_leave(player);
        if (is_current)
            m_protocol->announce_turn(m_players.front());
    } else {
        m_spectators.remove(player);
        m_protocol->announce_leave(player);
    }

    if (m_players.empty() && m_spectators.empty() && m_deletable) {
        m_active = false;
        m_protocol->announce_inactive_room(this);
    }
}

void Game::join_game(Player *player) {
    if (!player->is_playing()) {
        auto it = std::ranges::find(m_spectators.begin(), m_spectators.end(), player);
        if (it != m_spectators.end()) {
            m_players.push_back(player);
            m_spectators.erase(it);
            m_protocol->announce_join(player);
        }
    }
}

void Game::part_game(Player *player) {
    if (player->is_playing()) {
        auto it = std::ranges::find(m_players.begin(), m_players.end(), player);
        if (it != m_players.end()) {
            m_spectators.push_back(player);
            m_players.erase(it);
            m_protocol->announce_part(player);
        }
    }
}

const std::list<const Player *> Game::get_players() const {
    std::list<const Player *> all_players;
    for (Player *player : m_players)
        all_players.push_back(player);

    for (Player *player : m_spectators)
        all_players.push_back(player);

    return all_players;
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

int Game::get_game_id() const {
    return m_game_id;
}

Deck &Game::get_deck() {
    return m_deck;
}

const Deck &Game::get_deck() const {
    return m_deck;
}

Pile &Game::get_pile() {
    return m_pile;
}

const Pile &Game::get_pile() const {
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
    } else {
        hand.draw(this);
        m_protocol->announce_hand(player);
    }

    m_players.pop_front();
    m_players.push_back(player);
    m_protocol->announce_turn(m_players.front());
    m_protocol->announce_top_card(this);
}