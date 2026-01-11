//
// Created by laurent on 22/12/2025.
//

#include "Card.h"

Card::Card(Type t, Face f) : m_type(t), m_face(f) {}

bool Card::operator==(const Card &other) const {
    return m_face == other.m_face && m_type == other.m_type;
}

bool Card::face_sort_comparator(const Card &first, const Card &second) {
    return first.m_face < second.m_face;
}

bool Card::type_sort_comparator(const Card &first, const Card &second) {
    return first.m_type < second.m_type;
}

Card::Face Card::get_face() const {
    return m_face;
}

Card::Type Card::get_type() const {
    return m_type;
}