#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <algorithm>
using namespace std;

class Deck {
    public:    
        vector<string> p0Hand, p1Hand, p2Hand, p3Hand; 
        vector<string> pHand[4] = {p0Hand, p1Hand, p2Hand, p3Hand}; //Hands of players
        int winner; //Winner of game
        int player_turn; //Whose turn is it
        Deck(int playerCount)
            : num_players(playerCount){}
        void reset();
        void botTurn();
        void playerTurn();
        int checkWin();
        void incrementTurn();
    private: 
        vector<string> cards; //Cards in deck itself
        vector<string> played_cards; //Cards in pool
        int num_players; //Number of players (2, 3, or 4)
        bool skipped; //If true, skip current player's turn
        int direction; //1 is forward, 3 is back
        void shuffle();
        void drawCard();
        void deckEmpty();
        bool playCard(string card);
        void removeCard(string card);
        int cardsLeft();
        void printHand();
        bool findString(string card, string card_trait);
};
void Deck::reset() { //Resets game state
    p0Hand.clear();
    p1Hand.clear();
    p2Hand.clear();
    p3Hand.clear();
    played_cards.clear();
    cards = {"red 1", "red 1", "red 2", "red 2", "red 3", "red 3", "red 4", "red 4", "red 5", "red 5", "red 6", "red 6", "red 7", "red 7", "red 8", "red 8", "red 9", "red 9", "red skip", "red skip", "red reverse", "red reverse", "red +2", "red +2", "yellow 1", "yellow 1", "yellow 2", "yellow 2", "yellow 3", "yellow 3", "yellow 4", "yellow 4", "yellow 5", "yellow 5", "yellow 6", "yellow 6", "yellow 7", "yellow 7", "yellow 8", "yellow 8", "yellow 9", "yellow 9", "yellow skip", "yellow skip", "yellow reverse", "yellow reverse", "yellow +2", "yellow +2", "green 1", "green 1", "green 2", "green 2", "green 3", "green 3", "green 4", "green 4", "green 5", "green 5", "green 6", "green 6", "green 7", "green 7", "green 8", "green 8", "green 9", "green 9", "green skip", "green skip", "green reverse", "green reverse", "green +2", "green +2", "blue 1", "blue 1", "blue 2", "blue 2", "blue 3", "blue 3", "blue 4", "blue 4", "blue 5", "blue 5", "blue 6", "blue 6", "blue 7", "blue 7", "blue 8", "blue 8", "blue 9", "blue 9", "blue skip", "blue skip", "blue reverse", "blue reverse", "blue +2", "blue +2", "red 0", "yellow 0", "green 0", "blue 0", "wild card", "wild card", "wild card", "wild card", "wild +4", "wild +4", "wild +4", "wild +4"};
    shuffle();
    while (findString(cards.back(), "wild")) shuffle();
    played_cards.push_back(cards.back());
    cards.pop_back();
    for (int i = 0; i<num_players; i++) {
        player_turn = i;
        for (int j = 0; j < 7; j++) drawCard();
    }
    player_turn = 0;
    direction = 1;
    skipped = false;
    winner = -1;
    cout << "Reset complete\n\n";
}
void Deck::shuffle() { //Randomly shuffles deck
    random_shuffle(cards.begin(), cards.end()); 
}
void Deck::deckEmpty() { //Returns in play cards back to deck
    if (cards.size() == 0) {
        cards.insert(cards.end(), played_cards.begin(), played_cards.end() - 1);
        played_cards = {played_cards.back()};
        shuffle();
    }
}

void Deck::playerTurn() { //Turn of player
    bool card_played = false;
    bool card_drawed = false;
    string card;
    cout << "The top card is " << played_cards.back() << "\n"; 
    cout << "Your current hand is " << "\n";
    printHand();
    while (!card_played) {
        cout << "Enter the name of the card you wish to play or enter 'draw' to draw a card or 'end turn' if you've already drawn:" << endl;
        getline(cin, card);
        cout << endl;
        if (card.compare("draw") == 0 && !card_drawed) {
            drawCard();
            cout << "You have drawn a card." << "\n";
            cout << "The top card is " << played_cards.back() << "\n"; 
            cout << "Your current hand is " << "\n";
            printHand();
            card_drawed = true;
        }
        else if (card.compare("end turn") == 0 && card_drawed) {
            cout << "You have chosen to skip your turn.\n";
            card_played = true;
        }
        else if (count(pHand[0].begin(),pHand[0].end(),card)){
            if (playCard(card)) {
                card_played = true;
            }
            else {
                cout << "Invalid card has been played, try again.\n\n";
                cout << "The top card is " << played_cards.back() << "\n"; 
                cout << "Your current hand is " << "\n";
                printHand();
            }
        }
        else {
            cout << "Invalid option has been chosen.\n\n";
            cout << "The top card is " << played_cards.back() << "\n"; 
            cout << "Your current hand is " << "\n";
            printHand();
        }
    }
}
void Deck::botTurn() { //Turn of bot
    int bot_turn = player_turn;
    random_shuffle(pHand[player_turn].begin(), pHand[player_turn].end());
    bool card_played = false;
    bool card_drawed = false;
    while (!card_played && !card_drawed) {
        for (auto i: pHand[player_turn]) {
            if (playCard(i)) {
                card_played = true;
                break;
            }
        }
        if (card_drawed) break;
        if (!card_played) {
            cout << "Player " << bot_turn << " has drawn a card.\n";
            drawCard();
            card_drawed = true;
        }
    }
    cout << "Player " << bot_turn << " has " << pHand[bot_turn].size() << " cards remaining.\n";
}
int Deck::checkWin() { //Check if anyone's won yet
    if (num_players == 2 && pHand[0].size() == 0) return 0;
    if (num_players == 2 && pHand[1].size() == 0) return 1;
    if (num_players == 3 && pHand[2].size() == 0) return 2;
    if (num_players == 4 && pHand[3].size() == 0) return 3;
    return -1; // No one's won
}
int Deck::cardsLeft() { //Checks how many cards left in the deck
    return cards.size();
}
void Deck::incrementTurn() {
    player_turn = (player_turn+direction)%num_players;
}
void Deck::drawCard() { //Draw card to corresponding hand
    const auto card = cards.back();
    cards.pop_back();
    pHand[player_turn].push_back(card);
}
void Deck::removeCard(string card) { //Removes card from corresponding hand
    pHand[player_turn].erase(find(pHand[player_turn].begin(), pHand[player_turn].end(), card));
}
bool Deck::playCard(string card) {
    string top_card = played_cards.back(); //Get Top Card of Pile
    string played_card[2] = {card.substr(0, card.find(" ")), card.substr(card.find(" ")+1, card.size())};
    if (played_card[0].compare("wild") == 0 || findString(top_card, played_card[0]) || findString(top_card, played_card[1])) {
        removeCard(card);
        cout << "Player " << player_turn << " has played a " << card << endl;
        //Change top card from format (Ex: red +4, red card) to wild +4, wild card
        if (findString(top_card, "+4")) top_card = "wild +4";
        else if (findString(top_card, "card")) top_card = "wild card";
        if (findString(card, "wild")) {
            //Select wild card color
            string color;
            if (player_turn == 0) {
                cout << "Pick a color (red, yellow, green, blue)" << endl;
                cin >> color;
            }
            else if (player_turn != 0) {
                string choices[4] = {"red", "yellow", "blue", "green"};
                color = choices[rand()%4+1];
            }
            cout << "Player " << player_turn << " has chosen the color " << color << endl;
            if (findString(card, "card")) played_cards.push_back(color + " card");
            else if (findString(card, "+4")){ //Make next player draw 4 and skip their turn
                player_turn = (player_turn+direction)%num_players;
                for (int i = 0; i < 4; i++) drawCard();
                played_cards.push_back(color + " +4");
                cout << "\nPlayer " << player_turn << " has had their turn skipped.\n";
                cout << "Player " << player_turn << " has " << pHand[player_turn].size() << " cards now.\n";
            }
        }
        else {
            if (findString(card, "+2")) {
                player_turn = (player_turn+direction)%num_players;
                for (int i = 0; i < 2; i++) drawCard();
                cout << "\nPlayer " << player_turn << " has had their turn skipped.\n";
                cout << "Player " << player_turn << " has " << pHand[player_turn].size() << " cards now.\n";
            }
            else if (findString(card, "skip")) {
                player_turn = (player_turn+direction)%num_players;
                cout << "\nPlayer " << player_turn << " has had their turn skipped.\n";
            }
            else if (findString(card, "reverse")) {
                cout << "Reversing turn order.\n";
                if (direction == 1) direction = num_players - 1;
                else direction = 1;
            }
            played_cards.push_back(card);
        }
        return true; //Card play was valid
    }
    return false; //Card play was invalid
}
void Deck::printHand() { //Prints turn player's hand
    for (auto i: pHand[player_turn]) cout << i << ", ";
    cout << endl;
}
bool Deck::findString(string card, string card_trait) { //Checks if string card_trait exists in the card string
    string played_card[2] = {card.substr(0, card.find(" ")), card.substr(card.find(" ")+1, card.size())};
    return (played_card[0].compare(card_trait) == 0 || played_card[1].compare(card_trait) == 0);
}

int main () { //Command Line version of game using stdIn and stdOut
    srand(time (0));
    int num_players = 3; //Number of players
    Deck gamer(num_players);
    gamer.reset();
    while (1) {
        cout << "The turn belongs to player " << gamer.player_turn << "\n";
        if (gamer.player_turn == 0) gamer.playerTurn();
        else gamer.botTurn();
        cout << endl;
        gamer.winner = gamer.checkWin();
        if (gamer.winner != -1) cout << "\nPlayer " << gamer.winner << " has won the game of UNO, congraluations!\n";
        gamer.incrementTurn();
    }
}