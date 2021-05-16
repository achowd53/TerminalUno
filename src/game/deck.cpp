#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class Deck {
    vector<string> cards; //Cards in deck itself
    vector<string> p0Hand, p1Hand, p2Hand, p3Hand; //Hands of players
    vector<string> played_cards; //Cards in pool
    int num_players; //Number of players (2, 3, or 4)
    int player_turn; //Whose turn is it
    int direction; //1 is forward, 3 is back
    public:
        Deck(int playerCount)
            : num_players(playerCount){}
        void reset();
        void shuffle() {random_shuffle(cards.begin(), cards.end());}
        void drawCard();
        void deckEmpty();
        bool playCard(string card);
        void removeCard(string card);
        int checkWin();
        int cardsLeft() {return cards.size();}
};

void Deck::reset() { //Resets game state
    p0Hand.clear();
    p1Hand.clear();
    p2Hand.clear();
    p3Hand.clear();
    cards = {"red 1", "red 1", "red 2", "red 2", "red 3", "red 3", "red 4", "red 4", "red 5", "red 5", "red 6", "red 6", "red 7", "red 7", "red 8", "red 8", "red 9", "red 9", "red skip", "red skip", "red reverse", "red reverse", "red +2", "red +2", "yellow 1", "yellow 1", "yellow 2", "yellow 2", "yellow 3", "yellow 3", "yellow 4", "yellow 4", "yellow 5", "yellow 5", "yellow 6", "yellow 6", "yellow 7", "yellow 7", "yellow 8", "yellow 8", "yellow 9", "yellow 9", "yellow skip", "yellow skip", "yellow reverse", "yellow reverse", "yellow +2", "yellow +2", "green 1", "green 1", "green 2", "green 2", "green 3", "green 3", "green 4", "green 4", "green 5", "green 5", "green 6", "green 6", "green 7", "green 7", "green 8", "green 8", "green 9", "green 9", "green skip", "green skip", "green reverse", "green reverse", "green +2", "green +2", "blue 1", "blue 1", "blue 2", "blue 2", "blue 3", "blue 3", "blue 4", "blue 4", "blue 5", "blue 5", "blue 6", "blue 6", "blue 7", "blue 7", "blue 8", "blue 8", "blue 9", "blue 9", "blue skip", "blue skip", "blue reverse", "blue reverse", "blue +2", "blue +2", "red 0", "yellow 0", "green 0", "blue 0", "wild card", "wild card", "wild card", "wild card", "wild +4", "wild +4", "wild +4", "wild +4"};
    while (cards.back().find('wild')) shuffle();
    played_cards.push_back(cards.back());
    cards.pop_back();
    for (int i = 1; i<num_players+1; i++) {
        for (int j = 0; j < 7; j++) {
            player_turn = i;
            drawCard();
        }
    }
    player_turn = 1;
    direction = 1;
}
void Deck::deckEmpty() { //Returns in play cards back to deck
    if (cards.size() == 0) {
        copy(played_cards.begin(), played_cards.end() - 1, cards);
        played_cards = {played_cards.back()};
        shuffle();
    }
}
int Deck::checkWin() {
    if (num_players >= 2) {
        if (p0Hand.size() == 0) return 0;
        if (p1Hand.size() == 0) return 1;
    }
    if (num_players >= 3 && p2Hand.size() == 0) return 2;
    if (num_players >= 4 && p3Hand.size() == 0) return 3;
    return 4; // No one's won
}
void Deck::drawCard() { //Draw card to corresponding hand
    const auto card = cards.back();
    cards.pop_back();
    if (player_turn == 0) p0Hand.push_back(card);
    else if (player_turn == 1) p1Hand.push_back(card);
    else if (player_turn == 2) p2Hand.push_back(card);
    else p3Hand.push_back(card);
}
void Deck::removeCard(string card) { //Removes card from corresponding hand
    if (player_turn == 0) p0Hand.erase(find(p0Hand.begin(), p0Hand.end(), card));
    else if (player_turn == 1) p1Hand.erase(find(p1Hand.begin(), p1Hand.end(), card));
    else if (player_turn == 2) p2Hand.erase(find(p2Hand.begin(), p2Hand.end(), card));
    else p3Hand.erase(find(p3Hand.begin(), p3Hand.end(), card));
}
bool Deck::playCard(string card) {
    string top_card = played_cards.back(); //Get Top Card of Pile
    string played_card[2] = {card.substr(0, card.find(" ")), card.substr(card.find(" ")+1, card.size())};
    if (played_card[0] == "wild" || card.find(played_card[0]) || card.find(played_card[1])) {
        //Change top card from format (Ex: red +4, red card) to wild +4, wild card
        if (top_card.find("+4")) top_card = "wild +4";
        else if (top_card.find("card")) top_card = "wild card";
        removeCard(card);
        if (card.find("wild")) {
            //Select wild card color
            string color;
            cout << "Pick a color (red, yellow, green, blue)";
            cin >> color;
            if (card.find("card")) played_cards.push_back(color + " card");
            if (card.find("+4")) { //Make next player draw 4 and skip their turn
                player_turn = (player_turn+direction)%4;
                for (int i = 0; i < 4; i++) drawCard();
                played_cards.push_back(color + " +4");
            }
            player_turn = (player_turn+direction)%4;
        }
        else {
            if (card.find("+2")) {
                player_turn = (player_turn+direction)%4;
                for (int i = 0; i < 2; i++) drawCard();
            }
            else if (card.find("skip")) player_turn = (player_turn+direction)%4;
            else if (card.find("reverse")) direction = (direction+2)%4;
            player_turn = (player_turn+direction)%4;
            played_cards.push_back(card);
        }
        return true; //Card play was valid
    }
    else false; //Card play was invalid
}
int main () {
    Deck gamer(2);
    cout << gamer.cardsLeft();
}