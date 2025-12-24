//
// Created by laurent on 22/12/2025.
//

#include "Pile.h"

#include <algorithm>

Card Pile::top_card() const {
    return *m_cards.front();
}

void Pile::add(const Card *card) {
    m_cards.push_front(card);
}

bool Pile::contains(const Card *card) const {
    return std::ranges::any_of(m_cards.begin(), m_cards.end(), [card](const Card *pcard) {
        return card == pcard;
    });
}

void Pile::clear() {
    m_cards.clear();
}