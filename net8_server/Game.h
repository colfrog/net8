//
// Created by laurent on 22/12/2025.
//

#ifndef NET8_GAME_H
#define NET8_GAME_H

#include <list>
#include <stdexcept>

#include "Deck.h"
#include "Pile.h"
#include "Player.h"

class Net8Protocol;

class Game {
public:
    Game(Net8Protocol *protocol, std::string name, int game_id);

    void reset();
    void new_game();

    void add_player(Player *player);
    void remove_player(Player *player);
    void join_game(Player *player);
    void part_game(Player *player);
    const std::list<const Player *> get_players() const;

    void set_name(std::string name);
    const std::string &get_name() const;
    bool is_active() const;
    void set_active(bool active);
    bool is_deletable() const;
    void set_deletable(bool deletable);
    int get_game_id() const;

    Deck &get_deck();
    const Deck &get_deck() const;
    Pile &get_pile();
    const Pile &get_pile() const;
    int get_player_count() const;
    bool fits_rules(const Card *card);

    void do_turn(Player *player, bool play, int card_index, const std::string &arg);

    class game_error : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };

private:
    Net8Protocol *m_protocol;
    std::string m_name;
    Deck m_deck{0};
    Pile m_pile;
    std::list<Player *> m_players;
    std::list<Player *> m_spectators;

    int m_game_id = 0;
    bool m_deletable = true;
    bool m_active = true;
};


#endif //NET8_GAME_H