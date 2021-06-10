#ifndef POOL
#define POOL

#include "deck.h"

class CardPool { //Top of Pile is at back
    public:
        friend ostream& operator<<(ostream& stream, const CardPool& pool);
        Card getTopCard();
        void addCard(Card card);
        void emptyCards(Deck d);
    private:
        vector<Card> played_cards;
};

ostream& operator<<(ostream& stream, const CardPool& pool) {
    stream << pool.played_cards.back();
    return stream;
};

Card CardPool::getTopCard() {
    return played_cards.at(0);
};

void CardPool::addCard(Card card) {
    played_cards.push_back(card);
};

void CardPool::emptyCards(Deck d) {
    d.deck.insert(d.deck.end(), played_cards.begin(), played_cards.end() - 1);
    played_cards = {played_cards.back()};
    d.shuffleDeck();
};

#endif