//
// Created by laurent on 10/01/2026.
//

#ifndef NET8_HAND_H
#define NET8_HAND_H

#include <vector>

#include "Card.h"

class Game;

class Hand
{
public:
    Hand() = default;

    void draw(Game *game);
    bool play(Game *game, int card_index, const std::string &arg);

    const std::vector<Card> &get_cards() const;

private:
    std::vector<Card> m_cards;
};


#endif //NET8_HAND_H