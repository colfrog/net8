//
// Created by laurent on 22/12/2025.
//

#ifndef NET8_DECK_H
#define NET8_DECK_H

#include <vector>

#include "Card.h"
#include "Pile.h"

class Deck {
public:
    Deck(int n_players);

    void build();
    void shuffle();
    void rebuild(Pile pile);
    void set_n_players(int n_players);

private:
    int m_n_players;
    std::vector<Card> m_cards;
};


#endif //NET8_DECK_H