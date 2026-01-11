//
// Created by laurent on 29/12/2025.
//

#ifndef NET8_ROOMS_H
#define NET8_ROOMS_H

#include <vector>
#include <map>

#include "Game.h"

class Server;

class Rooms
{
public:
    Rooms(Server *server);

    int add_room(const std::string &name);

    void add_player(Player *player);
    void remove_player(Player *player);
    void transfer_player(Player *player, int game_id);
    const std::list<Player *> &get_players(int game_id) const;

private:
    Server *m_server;
    std::vector<Game> m_rooms;
    std::map<Player *, Game *> m_room_of_player;
};


#endif //NET8_ROOMS_H