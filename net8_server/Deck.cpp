//
// Created by laurent on 22/12/2025.
//

#include "Deck.h"
#include <algorithm>
#include <cmath>

#include "Card8.h"
#include "Game.h"

std::mt19937 Deck::rng{std::random_device{}()};

Deck::Deck(Game *game) : m_game(game) {}
Deck::~Deck() {
    m_game->get_pile().clear();
    clear();
    delete m_saved_card;
}

void Deck::build() {
    if (m_saved_card != nullptr)
        delete m_saved_card;
    m_saved_card = nullptr;

    Pile &pile = m_game->get_pile();
    m_saved_card = pile.top_card();
    pile.clear();
    clear();
    if (m_saved_card != nullptr)
        pile.add(m_saved_card);

    Card *card = nullptr;
    int num_decks = std::ceil(m_game->get_player_count()/4.);
    for (int k = 0; k < num_decks; k++) {
        for (int i = 0; i < static_cast<int>(Card::Type::COUNT); i++) {
            for (int j = 0; j < static_cast<int>(Card::Face::COUNT) - 1; j++) {
                if (j == 7) // 8
                    card = reinterpret_cast<Card *>(new Card8(static_cast<Card::Type>(i), static_cast<Card::Face>(j)));
                else
                    card = reinterpret_cast<Card *>(new Card(static_cast<Card::Type>(i), static_cast<Card::Face>(j)));
                m_cards.push_back(card);
            }
        }

        for (int i = 0; i < 2; i++) { // jokers
            card = reinterpret_cast<Card *>(new Card8(Card::Type::HEART, Card::Face::JOKER));
            m_cards.push_back(card);
        }
    }

    shuffle();
    if (m_saved_card == nullptr)
        pile.add(draw());
}

void Deck::clear() {
    for (const Card *card : m_cards)
        if (card != m_saved_card)
            delete card;

    m_cards.clear();
}

void Deck::shuffle() {
    std::ranges::shuffle(m_cards.begin(), m_cards.end(), rng);
}

void Deck::rebuild() {
    build();
}

void Deck::reset() {
    m_game->get_pile().clear(); // So we don't save the top card
    build();
}

Card *Deck::draw() {
    if (m_cards.empty()) {
        rebuild();
    }

    Card *card = m_cards.back();
    m_cards.pop_back();
    return card;
}
