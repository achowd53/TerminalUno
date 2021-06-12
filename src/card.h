#ifndef CARD
#define CARD

#include <string>
using namespace std;

class Card {
    public:
        Card(string card = "Blank Card") {
            card_name = card;
            string card_parts[2] = {card.substr(0, card.find(" ")), card.substr(card.find(" ")+1, card.size())};
            card_color = card_parts[0];
            card_number = card_parts[1];
        };
        friend ostream& operator<<(ostream& stream, const Card& card);
        string getCardString();
        string getColor();
        string getNumber();
        string getCard();
        void setWildColor(string color);
        bool validPlay(Card top_card);
        static bool stringsEqual(string a, string b);
    private:
        string card_name;
        string card_color;
        string card_number;
        string wild_color = "Wild"; // Only applicable if its a wild card
        string getWildColor();
};

ostream& operator<<(ostream & stream, const Card& card) {
    if (!Card::stringsEqual(card.wild_color, "Wild")) {
        stream << card.wild_color << " ";
    };
    stream << card.card_name;
    return stream;
};

string Card::getCardString() {
    string text;
    if (!Card::stringsEqual(wild_color, "Wild")) {
        text += wild_color + " ";
    };
    text += card_name;
    return text;
};

string Card::getCard() {
    return card_name;
};

string Card::getColor() {
    if (stringsEqual(card_color, "Wild")) {
        return getWildColor();
    };
    return card_color;
};

string Card::getNumber() {
    return card_number;
};

void Card::setWildColor(string color) {
    wild_color = color;
};

bool Card::validPlay(Card top_card) {
    return (stringsEqual(card_color, "Wild") || 
            stringsEqual(top_card.getColor(), getColor()) || 
            stringsEqual(top_card.getNumber(), getNumber()));
};

bool Card::stringsEqual(string a, string b) {
    return a.compare(b) == 0;
};

string Card::getWildColor() {
    return wild_color;
};

#endif