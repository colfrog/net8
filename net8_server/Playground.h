//
// Created by laurent on 02/01/2026.
//

#ifndef NET8_PLAYGROUND_H
#define NET8_PLAYGROUND_H

#include "Rooms.h"
#include "Players.h"

class Playground {
public:
    Playground(Server *server);

    int add_room(const std::string &name);

    void add_player(int socket, const std::string &name);
    void remove_player(int socket);
    void transfer_player(int socket, int game_id);
    const std::list<Player *> get_players(int game_id) const;
private:
    Server *m_server;
    Rooms m_rooms;
    Players m_players;
};


#endif //NET8_PLAYGROUND_H