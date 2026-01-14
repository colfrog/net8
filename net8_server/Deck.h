//
// Created by laurent on 22/12/2025.
//

#ifndef NET8_DECK_H
#define NET8_DECK_H

#include <random>
#include <vector>

#include "Card.h"
#include "Pile.h"

class Deck {
public:
    Deck(Game *game);
    ~Deck();

    void build();
    void clear();
    void shuffle();
    void rebuild();
    void reset();
    Card *draw();

private:
    Game *m_game;
    const Card *m_saved_card = nullptr;
    std::vector<Card *> m_cards;
    static std::mt19937 rng;
};


#endif //NET8_DECK_H