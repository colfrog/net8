//
// Created by laurent on 22/12/2025.
//

#ifndef NET8_GAME_H
#define NET8_GAME_H

#include <list>

#include "Deck.h"
#include "Pile.h"
#include "Player.h"

class Server; // Prototype to have a circular dependency Server->Game->Server

class Game {
public:
    Game(Server *server, std::string name);

    void reset();

    void add_player(Player *player);
    void remove_player(Player *player);
    const std::list<Player *> &get_players() const;

    void set_name(std::string name);
    const std::string &get_name() const;
    bool is_active() const;
    void set_active(bool active);
    bool is_deletable() const;
    void set_deletable(bool deletable);

    Deck &get_deck();
    Pile &get_pile();
    bool fits_rules(const Card *card);

private:
    Server *m_server;
    std::string m_name;
    Deck m_deck{0};
    Pile m_pile;
    std::list<Player *> m_players;

    bool m_deletable = true;
    bool m_active = true;
};


#endif //NET8_GAME_H