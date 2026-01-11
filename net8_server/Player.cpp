//
// Created by laurent on 22/12/2025.
//

#include "Player.h"

Player::Player(int socket, std::string name) : m_socket{socket}, m_name{std::move(name)} {}

void Player::set_name(std::string name) {
    m_name = std::move(name);
}

const std::string &Player::get_name() const {
    return m_name;
}

void Player::set_game_id(int game_id) {
    m_game_id = game_id;
}

int Player::get_game_id() const {
    return m_game_id;
}

void Player::set_playing(bool playing) {
    m_playing = playing;
    if (m_playing == false)
        m_hand = Hand();
}

bool Player::get_playing() const {
    return m_playing;
}

int Player::get_socket() const {
    return m_socket;
}

bool Player::operator==(const Player &player) const {
    return m_socket == player.m_socket;
}