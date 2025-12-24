//
// Created by laurent on 22/12/2025.
//

#ifndef NET8_GAME_H
#define NET8_GAME_H

#include <list>

#include "Deck.h"
#include "Player.h"

class Server; // Prototype to have a circular dependency Server->Game->Server

class Game {
public:
    Game(Server *server, std::string name);

    void add_player(int socket);
    void remove_player(int socket);

private:
    Server *m_server;
    int m_game_id;
    std::string m_name;
    Deck m_deck{0};
    std::list<Player> m_players;

    static int current_id;
};


#endif //NET8_GAME_H