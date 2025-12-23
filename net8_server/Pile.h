//
// Created by laurent on 22/12/2025.
//

#ifndef NET8_PILE_H
#define NET8_PILE_H

#include <list>

#include "Card.h"

class Pile {
public:
    Pile();

    bool in_pile(Card *card);
    void clear_pile();

private:
    std::list<Card *> cards;
};


#endif //NET8_PILE_H