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
    Rooms();

    void add_room(std::string name);


private:
    std::vector<Game> m_rooms;
    std::map<int, Game*> m_room_of_socket;
};


#endif //NET8_ROOMS_H