//
// Created by laurent on 10/01/2026.
//

#ifndef NET8_CARD8_H
#define NET8_CARD8_H

#include "Card.h"

class Card8 : Card {
    using Card::Card;
    ~Card8() override = default;

    virtual void effect(Game *game, const std::string &arg);
    virtual bool custom_rules(Game *game) const;
};


#endif //NET8_CARD8_H