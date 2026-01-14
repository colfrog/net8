//
// Created by laurent on 02/01/2026.
//

#ifndef NET8_PLAYGROUND_H
#define NET8_PLAYGROUND_H

#include "Rooms.h"
#include "Players.h"

class Playground {
public:
    Playground(Net8Protocol *protocol);

    int add_room(const std::string &name);
    Game *get_room(int socket);
    const Game *get_room(int socket) const;

    void player_join(int socket);
    void player_part(int socket);

    void add_player(int socket, const std::string &name);
    void remove_player(int socket);
    void transfer_player(int socket, int game_id);
    Player *get_player(int socket);
    const Player *get_player(int socket) const;
    const std::list<const Player *> get_players(int game_id) const;
    const Rooms &get_rooms() const;

private:
    Net8Protocol *m_protocol;
    Rooms m_rooms;
    Players m_players;
};


#endif //NET8_PLAYGROUND_H