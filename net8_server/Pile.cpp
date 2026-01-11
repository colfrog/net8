//
// Created by laurent on 22/12/2025.
//

#include "Pile.h"

#include <algorithm>

const Card *Pile::top_card() const {
    return m_cards.front();
}

void Pile::add(const Card *card) {
    m_cards.push_front(card);
}

void Pile::clear() {
    m_cards.clear();
}