//
// Created by laurent on 10/01/2026.
//

#include "Hand.h"

#include <algorithm>
#include <sstream>

#include "Game.h"
void Hand::draw_n(Game *game, int n) {
    for (int i = 0; i < n; i++)
        m_cards.push_back(game->get_deck().draw());
    std::ranges::sort(m_cards.begin(), m_cards.end(), Card::face_sort_comparator);
    std::ranges::sort(m_cards.begin(), m_cards.end(), Card::type_sort_comparator);
}

void Hand::draw(Game *game) {
    m_cards.push_back(game->get_deck().draw());
    std::ranges::sort(m_cards.begin(), m_cards.end(), Card::face_sort_comparator);
    std::ranges::sort(m_cards.begin(), m_cards.end(), Card::type_sort_comparator);
}

bool Hand::play(Game *game, int card_index, const std::string &arg) {
    if (game->fits_rules(m_cards[card_index])) {
        game->get_pile().add(m_cards[card_index]);
        m_cards[card_index]->effect(game, arg);
        m_cards.erase(m_cards.begin() + card_index);
        return true;
    } else {
        throw Game::game_error("Card " + m_cards[card_index]->to_string() + " cannot be played on " + game->get_pile().top_card()->to_string());
    }

    return false;
}

void Hand::clear() {
    m_cards.clear();
}

int Hand::count() const {
    return m_cards.size();
}

const std::vector<Card *> &Hand::get_cards() const {
    return m_cards;
}

std::string Hand::to_string() const {
    std::stringstream ss;
    int i = 0;
    for (Card *card: m_cards) {
        ss << card->to_string();
        if (i++ != m_cards.size() - 1)
            ss << ";";
    }

    return ss.str();
}