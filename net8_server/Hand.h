//
// Created by laurent on 10/01/2026.
//

#ifndef NET8_HAND_H
#define NET8_HAND_H

#include <vector>

#include "Card.h"

class Game;

class Hand {
public:
    Hand() = default;

    void draw_n(Game *game, int n);
    void draw(Game *game);
    bool play(Game *game, int card_index, const std::string &arg);
    void clear();
    int count() const;

    const std::vector<Card *> &get_cards() const;
    std::string to_string() const;

private:
    std::vector<Card *> m_cards;
};


#endif //NET8_HAND_H