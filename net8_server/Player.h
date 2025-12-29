//
// Created by laurent on 22/12/2025.
//

#ifndef NET8_PLAYER_H
#define NET8_PLAYER_H

#include <string>

class Player {
public:
    Player(int socket, int game_id);

    void set_name(std::string name);
    void set_game_id(int game_id);
    int get_game_id();

    int get_socket();

private:
    std::string m_name;
    int m_socket;
    int m_game_id;
    bool m_playing;
};


#endif //NET8_PLAYER_H