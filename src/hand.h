#ifndef HAND
#define HAND

#include <iostream>
#include <cstdlib>
#include <chrono>
#include <algorithm>
#include <vector>
#include <map>
#include "card.h"
using namespace std;

class Hand { //Hand is indexed from 0
    public:
        Hand(string name = "") {
            srand(time (0));
            resetHand();
        };
        friend ostream& operator<<(ostream& stream, const Hand& hand); //Get Content of player_hand as stream output
        string displayHand(); //Get Content of player_hand as a String 
        vector<Card> getCards(); //Get Content of player_hand as a Vector of Cards
        Card* playCard(Card* card_to_play); //Remove Card from player_hand and return it
        void addCards(vector<Card> added_cards); //Add Cards to player_hand from Vector of Cards
        void addCard(Card added_card); //Add Card to player_hand
        string highestColor(); //Return Color with highest number of cards in player_hand from Red, Yellow, Blue, Green
        Card* at(int n); //Return Card at index n in player_hand
        Card* findCardByName(string card_name); //Find and return card in player_hand by its card_name or throw error
        void resetHand(); //Clear player_hand
        void shuffleHand(); //Shuffle order of Cards in player_hand
        int numCards(); //Return cards_left
    private:
        vector<Card> player_hand;
        int cards_left = 0;
        vector<Card>::iterator findCard(Card* card_to_find); //Find iterator location of card in hand
        Card selected_card; //Selected card to play
};

ostream& operator<<(ostream& stream, const Hand& hand) {
    string text = "Content of Hand of Player:\n  ";
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

vector<Card> Hand::getCards() {
    return player_hand;
};

void Hand::addCards(vector<Card> added_cards) {
    player_hand.reserve(added_cards.size());
    player_hand.insert(player_hand.end(), added_cards.begin(), added_cards.end());
    cards_left += added_cards.size();
};

void Hand::addCard(Card added_card) {
    player_hand.push_back(added_card);
    cards_left += 1;
};

string Hand::highestColor() {
    int count_colors[] = {0, 0, 0, 0};
    string index_to_colors[] = {"Red", "Yellow", "Blue", "Green"};
    map<string, int> colors_to_index = {{"Red", 0}, {"Yellow", 1}, {"Blue", 2}, {"Green", 3}};
    for (int i = 0; i < player_hand.size(); i++) {
        string color = player_hand[i].getColor();
        if (Card::stringsEqual(color, "Red") || Card::stringsEqual(color, "Yellow") || 
            Card::stringsEqual(color, "Blue") || Card::stringsEqual(color, "Green")) {
            count_colors[colors_to_index[color]] += 1;
        };
    };
    return index_to_colors[distance(count_colors,std::max_element(count_colors, count_colors + 4))];
};

Card* Hand::playCard(Card* card_to_play) {
    vector<Card>::iterator loc = findCard(card_to_play);
    selected_card = *loc;
    player_hand.erase(loc);
    cards_left -= 1;
    return &selected_card;
};

Card* Hand::at(int n) {
    return &(player_hand.at(n));
};

Card* Hand::findCardByName(string card_name) {
    vector<Card>::iterator itr = player_hand.begin();
    for (itr; itr != player_hand.end(); ++itr) {
        if (Card::stringsEqual((*itr).getCard(), card_name)) {
            return &(*itr);
        };
    };
    throw -1;
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

vector<Card>::iterator Hand::findCard(Card* card_to_find) {
    vector<Card>::iterator itr = player_hand.begin();
    for (itr; itr != player_hand.end(); ++itr) {
        if ((*itr).getCard() == (*card_to_find).getCard()) {
            return itr;
        };
    };
    return player_hand.end()+1;
};

#endif