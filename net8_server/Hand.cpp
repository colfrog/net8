//
// Created by laurent on 10/01/2026.
//

#include "Hand.h"

#include <algorithm>

#include "Game.h"

void Hand::draw(Game *game) {
    m_cards.push_back(game->get_deck().draw());
    std::ranges::sort(m_cards.begin(), m_cards.end(), Card::face_sort_comparator);
    std::ranges::sort(m_cards.begin(), m_cards.end(), Card::type_sort_comparator);
}

bool Hand::play(Game *game, int card_index, const std::string &arg) {
    if (card_index < 0 || card_index >= m_cards.size())
        return false;

    if (game->fits_rules(m_cards[card_index])) {
        game->get_pile().add(m_cards[card_index]);
        m_cards[card_index].effect(game, arg);
        m_cards.erase(m_cards.begin() + card_index);
        return true;
    }

    return false;
}

const std::vector<Card> &Hand::get_cards() const {
    return m_cards;
}