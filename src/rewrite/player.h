#ifndef PLAYER
#define PLAYER

#include "deck.h"
#include "hand.h"
#include "pool.h"
#include <string>
using namespace std;

class Player {
    public:
        Player(Deck d, CardPool pool, string name = "", int t = 0) {
            srand(time (0));
            if (Card::stringsEqual("", name)) {
                name = "Player" + to_string(10000 + rand() % 90000);
            };
            deck = d;
            card_pool = pool;
            player_name = name;
            hand = Hand(name);
            player_type = t;
        };
        void resetPlayer();
        void drawCards(int n);
        void drawCard();
        int cardsLeft();
        int getPlayerType();
        bool placeRandomCard(); //true if card succesfully places
        void placeCard(Card place);
        void placeCard(int n);
        void placeCard(string str);
    private:
        Hand hand;
        Deck deck;
        string player_name;
        CardPool card_pool;
        int player_type; //0 is Player, 1 is CPU

};

void Player::resetPlayer() {
    hand.resetHand();
};

void Player::drawCards(int n) {
    vector<Card> drawn_cards = deck.drawCards(n);
    hand.addCards(drawn_cards);
};

void Player::drawCard() {
    Card draw = deck.drawCards(1).at(0);
    hand.addCard(draw);
};

int Player::cardsLeft() {
    return hand.numCards();
};

int Player::getPlayerType() {
    return player_type;
};

bool Player::placeRandomCard() {
    hand.shuffleHand();
    Card top_card = card_pool.getTopCard();
    for (int i = 0; i < cardsLeft(); i++) {
        if (hand.at(i).validPlay(top_card)) {
            placeCard(hand.at(i));
            return true;
        };
    };
    return false;
};

void Player::placeCard(Card place) {
    place = hand.playCard(place);
    card_pool.addCard(place);
};

void Player::placeCard(int n) {
    Card place = hand.at(n);
    placeCard(place);
};

void Player::placeCard(string str) {
    Card place = hand.findCardByName(str);
    placeCard(place);
};

#endif