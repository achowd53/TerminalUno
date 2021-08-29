#ifndef POOL
#define POOL

#include "deck.h"

class CardPool { //Top of Pile is at end of played_cards
    public:
        friend ostream& operator<<(ostream& stream, const CardPool& pool);
        CardPool(Deck* d) {
            deck = d;
            played_cards = {(*d).startingCard()};
        };
        Card* getTopCard(); //Get last card in played_cards
        vector<Card> getTopCards(); //Returns last played 2 Cards (in order of last card played, second from last) in played_cards
        void addCard(Card* card); //Add card to played_cards
        void emptyCards(); //Shuffle all played_cards other than last 2 into deck
        void resetPool(); //Clear played_cards
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

#endif