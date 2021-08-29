#ifndef COLOR
#define COLOR

#include <string>
#include <vector>
using namespace std;

class Color { //Terminal Coloring implemented with ANSI codes
    public:
        Color() {
            initColors();
        };
        string colorTerminal(string color_this, int color_index, bool use_color);
    private:
        void initColors(); //Initialize terminal coloring options
        vector<string> escape_colors;
        string reset_color = "\033[0m";
};

string Color::colorTerminal(string color_this, int color_index, bool use_color) {
    if (!use_color) { //If not using color, return as normal
        return color_this; 
    }
    //Otherwise add ANSI code to front and back to color terminal text
    return escape_colors[color_index] + color_this + reset_color;
};

void Color::initColors() {
    string red = "\033[0;31m";
    string green = "\033[0;32m";
    string yellow = "\033[0;33m";
    string blue = "\033[0;34m";
    string magenta = "\033[0;35m";
    string cyan = "\033[0;36m";
    string white = "\033[0;37m";
    string bright_red = "\033[0;91m";
    string bright_blue = "\033[0;94m";
    string bright_cyan = "\033[0;96m";
    escape_colors = {white, bright_red, green, yellow, blue, magenta, cyan, red, bright_blue, bright_cyan};
};

#endif