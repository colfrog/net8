//
// Created by laurent on 22/12/2025.
//

#ifndef NET8_PLAYER_H
#define NET8_PLAYER_H

#include <string>

class Player {
public:
    Player(int socket, std::string name);

    void set_name(std::string name);
    const std::string &get_name() const;
    void set_game_id(int game_id);
    int get_game_id() const;
    void set_playing(bool playing);
    bool get_playing() const;

    int get_socket() const;

    bool operator==(const Player &player) const;

private:
    int m_socket;
    std::string m_name;
    int m_game_id = 0;
    bool m_playing = false;
};


#endif //NET8_PLAYER_H