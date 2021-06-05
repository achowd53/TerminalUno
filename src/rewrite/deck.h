#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <chrono>
#include <algorithm>
#include "Card.h"
using namespace std;

class Deck {
    public:
        Deck() {
            srand(time (0));
            resetDeck();
            shuffleDeck();
        }
        friend ostream& operator<<(ostream& stream, const Deck& d);
        void resetDeck();
        void shuffleDeck();
    private:
        vector<Card> deck;
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
    for (auto i: cards) {
        Card new_card = Card(i);
        deck.push_back(new_card);
    };
};

void Deck::shuffleDeck() {
    random_shuffle(deck.begin(), deck.end()); 
};