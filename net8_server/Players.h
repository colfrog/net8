//
// Created by laurent on 02/01/2026.
//

#ifndef NET8_PLAYERS_H
#define NET8_PLAYERS_H

#include <map>
#include <list>

#include "Player.h"

class Players {
public:
    void add_player(int socket, const std::string &name);

    Player *get(int socket);
    void remove(Player *player);
    bool exists(int socket) const;

private:
    std::list<Player> m_players;
    std::map<int, Player *> m_player_of_socket;
};


#endif //NET8_PLAYERS_H
