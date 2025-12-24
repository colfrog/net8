//
// Created by laurent on 22/12/2025.
//

#ifndef NET8_CARD_H
#define NET8_CARD_H

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

    virtual void effect(Game *game) {}

private:
    Face m_face;
    Type m_type;
};


#endif //NET8_CARD_H