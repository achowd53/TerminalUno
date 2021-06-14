#include <random>
#include "player.h"
#include "color_terminal.h"

class Terminal {
    public:
        Deck deck = Deck();
        CardPool pool = CardPool(&deck);
        Color ansi = Color();
        vector<Player> players;
        Terminal() {
            srand(time (0));
            initGame();
        };
        int checkWin(); //Return 0 if no one's won yet or return index of player that has won
        Player getPlayer(int n);
        void playerTurn();
    private:
        void initGame();
        void createPlayers();   
        void incrementTurn();
        void checkDeck(); //restack deck if cards are getting low
        bool wildColorValid(string color);
        void specialCardRules(int num_actions);
        string color(string str);
        int stacked_draws_num = 0;
        int stacked_draws_type = -1; //0 is +2s and 1 is +4s
        int num_players;
        int turn;
        bool skip_turn = false;
        bool reversed_order = false;
        //Custom Settings
        bool stacking = false;
        bool seven_swap = false;
        bool zero_rotate = false;
        bool use_color = false;
};

int Terminal::checkWin() { //Return -1 if no one's won yet or return index of player that has won
    for (int i = 0; i < num_players; i++) {
        if (players[i].cardsLeft() == 0) {
            return i;
        };
    };
    return -1;
};

Player Terminal::getPlayer(int n) {
    return players[n];
};

void Terminal::initGame() {
    string answer = "blank";
    cout << ansi.colorTerminal("Is this text in color? (Answer 'yes' or 'no')", 9, true);
    while (getline(cin, answer) && !Card::stringsEqual(answer, "yes") && !Card::stringsEqual(answer, "no")) {};
    if (Card::stringsEqual(answer, "yes")) {
        use_color = true;
    };
    if (Card::stringsEqual(answer, "yes")) {
        stacking = true;
    };
    createPlayers();
    turn = rand() % num_players;
    //Each player draws 7 to start off the game
    for (int i = 0; i < num_players; i++) {
        players[i].drawCards(7);
    };
    answer = "blank";
    cout << "Enter either 'yes' or 'no' to determine whether the following custom rules should be enabled.\n";
    cout << "Enable stacking rule? ";
    while (getline(cin, answer) && !Card::stringsEqual(answer, "yes") && !Card::stringsEqual(answer, "no")) {};
    if (Card::stringsEqual(answer, "yes")) {
        stacking = true;
    };
    answer = "blank";
    cout << "Enable sevens rule? ";
    while (getline(cin, answer) && !Card::stringsEqual(answer, "yes") && !Card::stringsEqual(answer, "no")) {};
    if (Card::stringsEqual(answer, "yes")) {
        seven_swap = true;
    };
    answer = "blank";
    cout << "Enable zeroes rule? ";
    while (getline(cin, answer) && !Card::stringsEqual(answer, "yes") && !Card::stringsEqual(answer, "no")) {};
    if (Card::stringsEqual(answer, "yes")) {
        zero_rotate = true;
    };
    cout << "Game Rules Setup Complete.\n\n";
};

void Terminal::createPlayers() {
    //Create Players
    string player_name;
    cout << "Enter Player Name: ";
    getline(cin, player_name);
    Player self = Player(&deck, &pool, player_name);
    players.push_back(self);
    int cpus = 0;
    bool valid_cpus = false;
    while (!valid_cpus) {
        try {
            string temp;
            valid_cpus = true;
            cout << "Number of CPUs (1-7): ";
            getline(cin, temp);
            cpus = stoi(temp);
            if (cpus < 1 || cpus > 7) throw invalid_argument("Invalid argument");
        }
        catch (invalid_argument e) {
            valid_cpus = false;
            cout << "An invalid option for 'Number of CPUs' has been selected. Please reselect.\n";
        };
    };
    int cpu_tag_add = 10000;
    int cpu_tag;
    for (int i = 0; i < cpus; i++) {
        for (int j = 0; j < 10; j++) {
            cpu_tag = cpu_tag_add + rand() % 10000;
        };
        Player new_cpu = Player(&deck, &pool, cpu_tag, 1);
        cpu_tag_add += 10000;
        players.push_back(new_cpu);
    };
    num_players = cpus + 1;
    cout << "Player Setup Complete.\n\n";
};

void Terminal::playerTurn() {
    //If skip_turn, skip current players turn
    if (skip_turn) {
        skip_turn = false;
        Player* current_player = &(players.at(turn));
        cout << color((*current_player).getPlayerName() + "'s Turn has been skipped.\n");
        cout << color((*current_player).getPlayerName() + " has " + to_string((*current_player).cardsLeft()) + " cards remaining.\n\n");
        incrementTurn();
    };
    //Print whose turn it is
    Player* current_player = &(players.at(turn));
    cout << color("It is " + (*current_player).getPlayerName() + "'s Turn") << endl;
    //Print out what the top card is
    Card top_card = *(pool.getTopCard());
    cout << color("Top Card: " + top_card.getCardString() + "\n\n");
    //If self player, print out hand and ask to select card
    Card placed_card;
    bool uno_called = false;
    int actions_made = 0;
    if ((*current_player).getPlayerType() == 0) {
        bool valid_play = false;
        while (!valid_play) {
            cout << color("Your Current Hand: " + (*current_player).getHandString()) << endl;
            string card;
            cout << color("Input a Move (or enter 'help' to get a list of possible moves):\n");
            while (getline(cin, card) && card.length() == 0){};
            if (Card::stringsEqual(card, "help")) {
                string help_text = "Help Guide for Moves you can currently enter!\n\n";
                vector<Card> player_hand = players[turn].getHand().getCards();
                for (int i = 0; i < player_hand.size(); i++) {
                    string card_name = player_hand.at(i).getCard();
                    help_text += "Enter '" + to_string(i) + "' or '" + card_name + "' to play the " + card_name + " card.\n";
                };
                help_text += "\nEnter 'draw' to draw a card from the deck.\n";
                help_text += "Enter 'top' to be reminded of the top card on the pile.\n";
                help_text += "Enter 'cards' to be reminded of how many cards left each player has.\n";
                help_text += "Enter 'uno' if you are going to have 1 card at the end of your turn or draw 4.\n\n";
                cout << ansi.colorTerminal(help_text, 9, use_color) << endl;
                continue;
            };
            if (Card::stringsEqual(card, "draw")) {
                cout << color((*current_player).getPlayerName() + " has drawn a card") << endl;
                (*current_player).drawCard();
                actions_made += 1;
                continue;
            };
            if (Card::stringsEqual(card, "top")) {
                cout << color("Top Card: " + top_card.getCardString() + "\n") << endl;
                continue;
            };
            if (Card::stringsEqual(card, "cards")) {
                string help_text = "Number of Cards Each Player Has Left: \n";
                for (int i = 0; i < num_players; i++) {
                    help_text += "  " + to_string(i) + ". " + players[i].getPlayerName() + " - " + to_string(players[i].cardsLeft()) + " cards left\n";
                };
                help_text += "\n";
                cout << ansi.colorTerminal(help_text, 8, use_color);
                continue;
            };
            if (Card::stringsEqual(card, "uno")) {
                uno_called = true;
                cout << color((*current_player).getPlayerName() + " has called UNO.\n");
                continue;
            };
            try {
                int x = stoi(card);
                if ((*current_player).validatePlay(x, top_card)) {
                    //If valid play, placeCard
                    (*current_player).placeCard(x);
                    placed_card = *(pool.getTopCard());
                    valid_play = true;
                    actions_made += 1;
                }
                else {
                    //Otherwise throw error and retry
                    cout << color("Invalid Move. Please reenter a valid move or enter 'help' to get a list of valid moves.\n");
                    continue;
                };
            }
            catch (invalid_argument e) {
                if ((*current_player).validatePlay(card, top_card)) {
                    //If valid play, placeCard
                    (*current_player).placeCard(card);
                    valid_play = true;
                    placed_card = *(pool.getTopCard());
                    actions_made += 1;
                }
                else {
                    //Otherwise throw error and retry
                    cout << color("Invalid Move. Please reenter a valid move or enter 'help' to get a list of valid moves.\n");
                    continue;
                };
            };
        };
    }
    else { //If not player, random select card and draw until it works
        bool valid_play = false;
        while (!valid_play) {
            valid_play = (*current_player).placeRandomCard();
            if (!valid_play) {
                actions_made += 1;
                cout << color((*current_player).getPlayerName() + " has drawn a card.\n");
                (*current_player).drawCard();
            };
        };
        actions_made += 1;
        placed_card = *(pool.getTopCard());
    };
    cout << color((*current_player).getPlayerName() + " has placed a " + placed_card.getCardString()) << endl;
    //Special function for placed card depending on card
    specialCardRules(actions_made);
    //Check if player has called UNO or failed to call it
    if ((*current_player).getPlayerType() == 0) {
        if ((*current_player).cardsLeft() == 1) {
            if (!uno_called) {
                cout << color((*current_player).getPlayerName() + " has failed to call UNO with 1 card remaining.\n");
                (*current_player).drawCards(4);
            };
        }
        else if ((*current_player).cardsLeft() > 1) {
            if (uno_called) {
                cout << color((*current_player).getPlayerName() + " has miscalled UNO with more than 1 card remaining.\n");
                (*current_player).drawCards(4);
            };
        };
    };
    //Print out number of cards in that players hand and transition turn
    cout << color((*current_player).getPlayerName() + " has " + to_string((*current_player).cardsLeft()) + " cards remaining.\n\n");
    incrementTurn();
};

bool Terminal::wildColorValid(string color) {
    return (Card::stringsEqual(color,"Red") || Card::stringsEqual(color,"Blue")
            || Card::stringsEqual(color,"Yellow") || Card::stringsEqual(color,"Green"));
};

void Terminal::specialCardRules(int num_actions) {
    Player* player = &players.at(turn);
    Card top_card = *(pool.getTopCard());
    if (num_actions > 1 || !stacking) { //If you didn't just place a card or stacking is disabled
        if (stacked_draws_num > 0) {
            cout << color((*player).getPlayerName() + " has drawn " + to_string(stacked_draws_num) + " cards.\n");
        };
        (*player).drawCards(stacked_draws_num);
        stacked_draws_num = 0;
        stacked_draws_type = -1;
    }
    else { //If you just placed a card in your turn, stacking is on, and you failed to stack
        if ((stacked_draws_type == 0 && !Card::stringsEqual(top_card.getNumber(), "+2")) ||
            (stacked_draws_type == 1 && !Card::stringsEqual(top_card.getNumber(), "+4"))) {
            if (stacked_draws_num > 0) {
                cout << color((*player).getPlayerName() + " has drawn " + to_string(stacked_draws_num) + " cards.\n");
            };
            (*player).drawCards(stacked_draws_num);
            stacked_draws_num = 0;
            stacked_draws_type = -1;
        };
    };
    if (Card::stringsEqual(top_card.getNumber(), "+2")) {
        stacked_draws_num += 2;
        stacked_draws_type = 0;
    };
    if (Card::stringsEqual(top_card.getNumber(), "+4")) {
        stacked_draws_num += 4;
        stacked_draws_type = 1;
    };
    if (seven_swap && Card::stringsEqual(top_card.getNumber(), "7")) {
        int swap_with = 0;
        if ((*player).getPlayerType() == 0) {
            string text = "List of Players\n";
            for (int i = 1; i < num_players; i++) {
                text += "  " + to_string(i) + ". " + players[i].getPlayerName() + " - " + to_string(players[i].cardsLeft()) + " cards left\n";
            };
            cout << text;
            while (swap_with == 0) {
                cout << "Select a player to swap hands with: ";
                try {
                    getline(cin, text);
                    swap_with = stoi(text);
                }
                catch (invalid_argument e) {
                    cout << "Invalid player entered, please enter a number between 1 and " << num_players - 1 << endl;
                };
            };
        }
        else {
            int has_cards = 108;
            for (int i = 0; i < num_players; i++) {
                if (players[i].cardsLeft() < has_cards && i != turn) {
                    swap_with = i;
                    has_cards = players[i].cardsLeft();
                };
            };
        };
        vector<Hand> hands = {players[turn].getHand(), players[swap_with].getHand()};
        players[turn].setNewHand(&hands[1]);
        players[swap_with].setNewHand(&hands[0]);
        cout << color(players[turn].getPlayerName() + " has chosen to swap hands with " + players[swap_with].getPlayerName() + "\n");
    };
    if (zero_rotate && Card::stringsEqual(top_card.getNumber(), "0")) {
        vector<Hand> hands;
        for (int i = 0; i < num_players; i++) {
            hands.push_back(players[i].getHand());
        };
        for (int i = 0; i < num_players; i++) {
            int hand_from;
            if (reversed_order) {
                hand_from = i-1;
                if (hand_from == -1) {
                    hand_from = num_players - 1;
                };
            }
            else {
                hand_from = (i+1) % num_players;
            };
            players[i].setNewHand(&hands[hand_from]);
        };
        cout << color("Hands have been rotated.\n");
    };
    if (Card::stringsEqual(top_card.getNumber(), "Skip")) {
        skip_turn = true;
    };
    if (Card::stringsEqual(top_card.getNumber(), "Reverse")) {
        reversed_order = !reversed_order;
    };
    if (Card::stringsEqual(top_card.getColor(), "Wild")) {
        if ((*player).getPlayerType() == 0) {
            string col = "";
            cout << color("Choose Color (Red, Yellow, Blue, Green): ");
            getline(cin, col);
            while (!wildColorValid(col)) {
                cout << color("Invalid Color Chosen.\nPlease Choose Color From (Red, Yellow, Blue, Green): ");
                getline(cin, col);
            };
            cout << color((*player).getPlayerName() + " has selected the color " + col) << endl;
            (*(pool.getTopCard())).setWildColor(col);
        }
        else {
            vector<string> colors = {"Red", "Yellow", "Blue", "Green"};
            string col = colors[rand()%4];
            cout << color((*player).getPlayerName() + " has selected the color " + col + "\n");
            (*(pool.getTopCard())).setWildColor(col);
        };
    };
};

void Terminal::incrementTurn() {
    if (reversed_order) {
        turn -= 1;
        if (turn == -1) {
            turn = num_players - 1;
        };
    }
    else {
        turn = (turn + 1) % num_players;
    };
};

void Terminal::checkDeck() {
    if (deck.num_cards <= 20) {
        pool.emptyCards();
    };
};

string Terminal::color(string str) {
    return ansi.colorTerminal(str, turn, use_color);
};

int main() {
    Terminal game = Terminal();
    while (game.checkWin() == -1) {
        game.playerTurn();
    };
    cout << game.getPlayer(game.checkWin());
    return -1;  
}
//Things left to do in this class:
//  std::bad_alloc error 
