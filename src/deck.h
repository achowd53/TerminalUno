#ifndef DECK
#define DECK

#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <algorithm>
#include "card.h"
using namespace std;

class Deck { //Back of deck is the top card
    public:
        Deck() {
            srand(time (0));
            resetDeck();
        };
        friend ostream& operator<<(ostream& stream, const Deck& d);
        void resetDeck();
        void shuffleDeck();
        vector<Card> drawCards(int num_draws);
        Card drawCard();
        Card startingCard();
        vector<Card> deck;
        int num_cards;
};

ostream& operator<<(ostream& stream, const Deck& d) {
    string text = "Contents of Deck:\n";
    for (Card card: d.deck) {
        text += "   " + card.getCard() + "\n";
    };  
    stream << text;
    return stream;
}

void Deck::resetDeck() {
    deck.clear();
    vector<string> cards = {"Red 1", "Red 1", "Red 2", "Red 2", "Red 3", "Red 3", "Red 4", "Red 4", "Red 5", "Red 5", "Red 6", "Red 6", "Red 7", "Red 7", "Red 8", "Red 8", "Red 9", "Red 9", "Red Skip", "Red Skip", "Red Reverse", "Red Reverse", "Red +2", "Red +2", "Yellow 1", "Yellow 1", "Yellow 2", "Yellow 2", "Yellow 3", "Yellow 3", "Yellow 4", "Yellow 4", "Yellow 5", "Yellow 5", "Yellow 6", "Yellow 6", "Yellow 7", "Yellow 7", "Yellow 8", "Yellow 8", "Yellow 9", "Yellow 9", "Yellow Skip", "Yellow Skip", "Yellow Reverse", "Yellow Reverse", "Yellow +2", "Yellow +2", "Green 1", "Green 1", "Green 2", "Green 2", "Green 3", "Green 3", "Green 4", "Green 4", "Green 5", "Green 5", "Green 6", "Green 6", "Green 7", "Green 7", "Green 8", "Green 8", "Green 9", "Green 9", "Green Skip", "Green Skip", "Green Reverse", "Green Reverse", "Green +2", "Green +2", "Blue 1", "Blue 1", "Blue 2", "Blue 2", "Blue 3", "Blue 3", "Blue 4", "Blue 4", "Blue 5", "Blue 5", "Blue 6", "Blue 6", "Blue 7", "Blue 7", "Blue 8", "Blue 8", "Blue 9", "Blue 9", "Blue Skip", "Blue Skip", "Blue Reverse", "Blue Reverse", "Blue +2", "Blue +2", "Red 0", "Yellow 0", "Green 0", "Blue 0", "Wild Card", "Wild Card", "Wild Card", "Wild Card", "Wild +4", "Wild +4", "Wild +4", "Wild +4"};
    num_cards = 0;
    for (int j = 0; j < 2; j++) {//Number of duplicates of deck
        num_cards += 108;
        for (string i: cards) {
            Card new_card = Card(i);
            deck.push_back(new_card);
        };
    };
    shuffleDeck();
};

void Deck::shuffleDeck() {
    random_shuffle(deck.begin(), deck.end()); 
};

vector<Card> Deck::drawCards(int num_draws) {
    vector<Card> draws;
    for (int i = 0; i < num_draws; i++) {
        draws.push_back(drawCard());
    };
    num_cards -= num_draws;
    return draws;
};

Card Deck::drawCard() {
    Card card = deck.back();
    deck.pop_back();
    num_cards -= 1;
    return card;
};

Card Deck::startingCard() {
    while (Card::stringsEqual(deck.back().getColor(), "Wild")) {
        shuffleDeck();
    };
    return drawCard();
};

#endif