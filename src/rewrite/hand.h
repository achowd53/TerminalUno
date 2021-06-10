#ifndef HAND
#define HAND

#include <iostream>
#include <cstdlib>
#include <chrono>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include "card.h"
using namespace std;

class Hand { //Hand is indexed from 0
    public:
        Hand(string name = "") {
            srand(time (0));
            player_name = name;
            resetHand();
        };
        friend ostream& operator<<(ostream& stream, const Hand& hand);
        string displayHand();
        Card playCard(Card card_to_play);
        void addCards(vector<Card> added_cards);
        void addCard(Card added_card);
        Card at(int n);
        Card findCardByName(string card_name);
        void resetHand();
        void shuffleHand();
        int numCards();
    private:
        string player_name;
        vector<Card> player_hand;
        int cards_left = 0;
        vector<Card>::iterator findCard(Card card_to_find);
};

ostream& operator<<(ostream& stream, const Hand& hand) {
    string text = "Content of Hand of Player " + hand.player_name + ":\n  ";
    for (auto i: hand.player_hand) {
        text += i.getCard() + ", ";
    };
    if (hand.cards_left) {
        text = text.substr(0, text.size() - 2) + "\n";
    };
    stream << text;
    return stream;
};

string Hand::displayHand() {
    string text = "";
    if (cards_left == 0) {
        return text;
    };
    for (auto i: player_hand) {
        text += i.getCard() + ", ";
    };
    text = text.substr(0, text.size() - 2) + "\n";
    return text;
};

void Hand::addCards(vector<Card> added_cards) {
    player_hand.reserve(added_cards.size());
    player_hand.insert(player_hand.end(), added_cards.begin(), added_cards.end());
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

Card Hand::at(int n) {
    return player_hand.at(n);
};

Card Hand::findCardByName(string card_name) {
    vector<Card>::iterator itr = player_hand.begin();
    for (itr; itr != player_hand.end(); ++itr) {
        if (Card::stringsEqual((*itr).getCard(), card_name)) {
            return *itr;
        };
    };
    throw invalid_argument("Function called with card name not existing in hand of player " + player_name);
};

void Hand::resetHand() {
    player_hand.clear();
    cards_left = 0;
};

void Hand::shuffleHand() {
    random_shuffle(player_hand.begin(), player_hand.end()); 
};

int Hand::numCards() {
    return cards_left;
};

vector<Card>::iterator Hand::findCard(Card card_to_find) {
    vector<Card>::iterator itr = player_hand.begin();
    for (itr; itr != player_hand.end(); ++itr) {
        if ((*itr).getCard() == card_to_find.getCard()) {
            return itr;
        };
    };
    return player_hand.end()+1;
};

#endif