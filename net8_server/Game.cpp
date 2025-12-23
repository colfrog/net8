//
// Created by laurent on 22/12/2025.
//

#include "Game.h"

int Game::current_id = 0;

Game::Game(Server *server, std::string name) : m_server(server), m_name(std::move(name)) {
    m_game_id = current_id++;
}

void Game::remove_player(int socket) {}