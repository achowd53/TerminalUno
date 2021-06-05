#include <iostream>
#include <vector>
#include <string>
#include "card.h"
using namespace std;

class Hand {
    public:
        Hand(string name = "") {
            player_name = name;
        };
        friend ostream& operator<<(ostream& stream, const Hand& hand);
        string displayHand();
        void addCard(Card added_card);
        Card playCard(Card card_to_play);
        void resetHand();
    private:
        string player_name;
        vector<Card> player_hand;
        int cards_left = 0;
        vector<Card>::iterator findCard(Card card_to_find);
};

ostream& operator<<(ostream& stream, const Hand& hand) {
    string text = "Content of Hand of Player " + hand.player_name + ":\n";
    for (Card i: hand.player_hand) {
        text += i.getCard() + ", ";
    };
    if (hand.cards_left) {
        text = text.substr(0, text.size() - 2) + "\n";
    };
    stream << text;
    return stream;
}

string Hand::displayHand() {
    string text = "";
    if (cards_left == 0) {
        return text;
    };
    for (Card i: player_hand) {
        text += i.getCard() + ", ";
    };
    text = text.substr(0, text.size() - 2) + "\n";
    return text;
};

void Hand::addCard(Card added_card) {
    player_hand.push_back(added_card);
    cards_left += 1;
};

Card Hand::playCard(Card card_to_play) {
    vector<Card>::iterator loc = findCard(card_to_play);
    Card played_card = (*loc).clone();
    player_hand.erase(loc);
    return played_card;
};

void Hand::resetHand() {
    player_hand = {};
    cards_left = 0;
};

vector<Card>::iterator Hand::findCard(Card card_to_find) {
    vector<Card>::iterator itr = player_hand.begin();
    for (itr; itr != player_hand.end(); ++itr) {
        if ((*itr).getCard() == card_to_find.getCard()) {
            return itr;
        };
    };
};