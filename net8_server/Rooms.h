//
// Created by laurent on 29/12/2025.
//

#ifndef NET8_ROOMS_H
#define NET8_ROOMS_H

#include <vector>
#include <map>

#include "Game.h"

class Rooms
{
public:
    Rooms(Net8Protocol *protocol);

    int add_room(const std::string &name);
    Game *get_room(int game_id);
    const Game *get_room(int game_id) const;

    void player_join(Player *player);
    void player_part(Player *player);

    void add_player(Player *player);
    void remove_player(Player *player);
    void transfer_player(Player *player, int game_id);
    std::list<const Player *> get_players(int game_id) const;

    std::string to_string() const;

private:
    Net8Protocol *m_protocol;
    std::vector<Game> m_rooms;
    std::map<Player *, Game *> m_room_of_player;
};


#endif //NET8_ROOMS_H