//
// Created by laurent on 22/12/2025.
//

#ifndef NET8_PILE_H
#define NET8_PILE_H

#include <list>

#include "Card.h"

class Pile {
public:
    Pile() = default;

    [[nodiscard]] Card top_card() const;
    void add(const Card *card);
    bool contains(const Card *card) const;
    void clear();

private:
    std::list<const Card *> m_cards;
};


#endif //NET8_PILE_H