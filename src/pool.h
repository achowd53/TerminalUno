#ifndef POOL
#define POOL

#include "deck.h"

class CardPool { //Top of Pile is at back
    public:
        friend ostream& operator<<(ostream& stream, const CardPool& pool);
        CardPool(Deck* d) {
            deck = d;
            played_cards = {(*d).drawCard()};
        };
        Card* getTopCard();
        vector<Card> getTopCards(); //Returns Top 2 Cards (in order of top card, second from top)
        void addCard(Card* card);
        void emptyCards();
        void resetPool();
        void applyWildColor(string color);
    private:
        vector<Card> played_cards;
        Deck* deck;
};

ostream& operator<<(ostream& stream, const CardPool& pool) {
    stream << pool.played_cards.back();
    return stream;
};

Card* CardPool::getTopCard() {
    return &(played_cards.back());
};

vector<Card> CardPool::getTopCards() {
    vector<Card> top_cards = {played_cards.at(-1), played_cards.at(-2)};
    return top_cards;
};

void CardPool::addCard(Card* card) {
    played_cards.push_back(*card);
};

void CardPool::emptyCards() {
    Deck d = *deck;
    d.deck.insert(d.deck.end(), played_cards.begin(), played_cards.end() - 2);
    played_cards = {played_cards.at(-2), played_cards.back()};
    d.shuffleDeck();
};

void CardPool::resetPool() {
    played_cards.clear();
};

void CardPool::applyWildColor(string color) {
    played_cards.back().setWildColor(color);
};

#endif