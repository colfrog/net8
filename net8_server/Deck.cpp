//
// Created by laurent on 22/12/2025.
//

#include "Deck.h"
#include <algorithm>
#include <cmath>


std::mt19937 Deck::rng{std::random_device{}()};

Deck::Deck(int n_players) : m_n_players(n_players) {}

void Deck::build() {
    m_cards.clear();
    int num_decks = std::ceil(m_n_players/4);
    for (int k = 0; k < num_decks; k++) {
        for (int i = 0; i < static_cast<int>(Card::Type::COUNT); i++) {
            for (int j = 0; j < static_cast<int>(Card::Face::COUNT); j++) {
                m_cards.emplace_back(static_cast<Card::Type>(i), static_cast<Card::Face>(j));
            }
        }
    }
    shuffle();
}

void Deck::shuffle() {
    std::ranges::shuffle(m_cards.begin(), m_cards.end(), rng);
}

void Deck::rebuild(Pile pile) {
    pile.clear();
    build();
}

void Deck::set_n_players(int n_players) {
    m_n_players = n_players;
}

Card Deck::draw() {
    Card card = m_cards.back();
    m_cards.pop_back();
    return card;
}