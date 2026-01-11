//
// Created by laurent on 22/12/2025.
//

#include "Deck.h"
#include <algorithm>
#include <cmath>

#include "Card8.h"

std::mt19937 Deck::rng{std::random_device{}()};

Deck::Deck(int n_players) : m_n_players(n_players) {}
Deck::~Deck() {
    clear();
}

void Deck::build() {
    clear();
    Card *card = nullptr;
    int num_decks = std::ceil(m_n_players/4);
    for (int k = 0; k < num_decks; k++) {
        for (int i = 0; i < static_cast<int>(Card::Type::COUNT); i++) {
            for (int j = 0; j < static_cast<int>(Card::Face::COUNT); j++) {
                if (j == 7 || j == 13) // 8 or joker
                    card = reinterpret_cast<Card *>(new Card8(static_cast<Card::Type>(i), Card::Face::JOKER));
                else
                    card = reinterpret_cast<Card *>(new Card(static_cast<Card::Type>(i), static_cast<Card::Face>(j)));
                m_cards.push_back(card);
            }
        }
    }
    shuffle();
}

void Deck::clear() {
    for (Card *card : m_cards)
        delete card;

    m_cards.clear();
}

void Deck::shuffle() {
    std::ranges::shuffle(m_cards.begin(), m_cards.end(), rng);
}

void Deck::rebuild(Pile &pile) {
    pile.clear();
    build();
}

void Deck::set_n_players(int n_players) {
    m_n_players = n_players;
}

Card *Deck::draw() {
    Card *card = m_cards.back();
    m_cards.pop_back();
    return card;
}