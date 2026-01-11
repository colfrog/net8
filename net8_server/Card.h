//
// Created by laurent on 22/12/2025.
//

#ifndef NET8_CARD_H
#define NET8_CARD_H
#include <string>

class Game;

class Card {
public:
    enum class Face {
        ACE,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
        NINE,
        TEN,
        JACK,
        QUEEN,
        KING,
        JOKER,
        COUNT
    };

    enum class Type {
        HEART,
        DIAMOND,
        SPADE,
        CLUB,
        COUNT
    };

    Card(Type t, Face f);

    virtual void effect(Game *game, const std::string &arg) {}
    virtual bool custom_rules(Game *game) const {
        return false;
    }

    Face get_face() const;
    Type get_type() const;

    bool operator==(const Card &other) const;
    static bool face_sort_comparator(const Card *first, const Card *second);
    static bool type_sort_comparator(const Card *first, const Card *second);

private:
    Face m_face;
    Type m_type;
};


#endif //NET8_CARD_H