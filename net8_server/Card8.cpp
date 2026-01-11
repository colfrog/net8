//
// Created by laurent on 10/01/2026.
//

#include "Card8.h"

void Card8::effect(Game *game, const std::string &arg) {
    if (arg == "hearts")
        m_type = Type::HEART;
    else if (arg == "diamonds")
        m_type = Type::DIAMOND;
    else if (arg == "clubs")
        m_type = Type::CLUB;
    else if (arg == "spades")
        m_type = Type::SPADE;
}

bool Card8::custom_rules(Game* game) const {
    return true; // Always playable
}
