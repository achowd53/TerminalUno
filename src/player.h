#ifndef PLAYER
#define PLAYER

#include "deck.h"
#include "hand.h"
#include "pool.h"
#include <string>
using namespace std;

class Player {
    public:
        Player(Deck* d, CardPool* pool, int random_id, int type) {
            deck = d;
            card_pool = pool;
            player_name = "Player" + to_string(random_id);
            hand = Hand(player_name);
            player_type = type;
        };
        Player(Deck* d, CardPool* pool, string name) { 
            deck = d;
            card_pool = pool;
            player_name = name;
            hand = Hand(name);
            player_type = 0;
        };
        friend ostream& operator<<(ostream& stream, const Player& player);
        void resetPlayer();
        void drawCards(int n);
        void drawCard();
        int cardsLeft();
        int getPlayerType();
        string getPlayerName();
        string getHandString();
        bool placeRandomCard(); //true if card succesfully places
        void placeCard(Card* place);
        void placeCard(int n);
        void placeCard(string str);
        bool validatePlay(Card* card, Card top_card);
        bool validatePlay(int n, Card top_card);
        bool validatePlay(string str, Card top_card);
        Card* getCardFromString(string str);
        Card* getCardFromInteger(int n);
        Hand getHand();
        void setNewHand(Hand* new_hand);
    private:
        Hand hand;
        Deck* deck;
        string player_name;
        CardPool* card_pool;
        int player_type; //0 is Player, 1 is CPU

};

ostream& operator<<(ostream& stream, const Player& player) {
    stream << player.player_name;
    return stream;
};

void Player::resetPlayer() {
    hand.resetHand();
};

void Player::drawCards(int n) {
    if (n != 0) {
        vector<Card> drawn_cards = (*deck).drawCards(n);
        hand.addCards(drawn_cards);
    };
};

void Player::drawCard() {
    Card draw = (*deck).drawCards(1).at(0);
    hand.addCard(draw);
};

int Player::cardsLeft() {
    return hand.numCards();
};

int Player::getPlayerType() {
    return player_type;
};

string Player::getPlayerName() {
    return player_name;
};

string Player::getHandString() {
    return hand.getHandString();
};

bool Player::placeRandomCard() {
    hand.shuffleHand();
    Card top_card = *((*card_pool).getTopCard());
    for (int i = 0; i < cardsLeft(); i++) {
        if (validatePlay(hand.at(i), top_card)) {
            placeCard(hand.at(i));
            return true;
        };
    };
    return false;
};

void Player::placeCard(Card* place) {
    place = hand.playCard(place);
    (*card_pool).addCard(place);
};

void Player::placeCard(int n) {
    Card* place = hand.at(n);
    placeCard(place);
};

void Player::placeCard(string str) {
    Card* place = hand.findCardByName(str);
    placeCard(place);
};

bool Player::validatePlay(Card* card, Card top_card) {
    return (*card).validPlay(top_card);
};

bool Player::validatePlay(int n, Card top_card) {
    if (n < 0 || n >= hand.numCards()) {
        return false;
    };
    Card* card = hand.at(n);
    return validatePlay(card, top_card);
};

bool Player::validatePlay(string str, Card top_card) {
    Card* card;
    try {
        card = hand.findCardByName(str);
    }
    catch (int e) {
        return false;
    };
    return validatePlay(card, top_card);
};

Card* Player::getCardFromString(string str) {
    return hand.findCardByName(str);
};

Card* Player::getCardFromInteger(int n) {
    return hand.at(n);
};

Hand Player::getHand() {
    return hand;
};

void Player::setNewHand(Hand* new_hand) {
    hand = *new_hand;
};

#endif