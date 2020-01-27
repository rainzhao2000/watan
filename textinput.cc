#include <iostream>
#include "textinput.h"
#include "state.h"
using namespace std;

Command TextInput::getCommand() {
    string command;
    while (true) {
        cin >> command;
        if (command == "board") {
            return Command::Board;
            break;
        } else if (command == "status") {
            return Command::Status;
            break;
        } else if (command == "roll") {
            return Command::Roll;
            break;
        } else if (command == "criteria") {
            return Command::Criteria;
            break;
        } else if (command == "achieve") {
            return Command::Achieve;
            break;
        } else if (command == "complete") {
            return Command::Complete;
            break;
        } else if (command == "improve") {
            return Command::Improve;
            break;
        } else if (command == "trade") {
            return Command::Trade;
            break;
        } else if (command == "next") {
            return Command::Next;
            break;
        } else if (command == "save") {
            return Command::Save;
            break;
        } else if (command == "help") {
            return Command::Help;
            break;
        } else if (command == "load") {
            return Command::DiceLoad;
            break;
        } else if (command == "fair") {
            return Command::DiceFair;
            break;
        } else if (command == "quit") {
            return Command::Quit;
            break;
        }
        cin.ignore(1, '\n');
        if (cin.eof()) {
            return Command::Quit;
        }
    }
}

int TextInput::getLoadedDice(const int min, const int max) {
    int value;
    while (true) {
        if (cin >> value) {
            if (value > max || value < min) {
                continue;
            }
            return value;
        } else {
            cin.clear();
            cin.ignore(1, '\n');
        }
    }
}

int TextInput::getCriterionPlacement(const int min, const int max) {
    int index;
    while (true) {
        if (cin >> index) {
            if (index > max || index < min) {
                continue;
            }
            return index;
        } else {
            cin.clear();
            cin.ignore(1, '\n');
        }
    }
}

int TextInput::getGoalPlacement(const int min, const int max) {
    int index;
    while (true) {
        if (cin >> index) {
            if (index > max || index < min) {
                continue;
            }
            return index;
        } else {
            cin.clear();
            cin.ignore(1, '\n');
        }
    }
}

int TextInput::getTile(const int min, const int max) {
    int index;
    while (true) {
        if (cin >> index) {
            if (index > max || index < min) {
                continue;
            }
            return index;
        } else {
            cin.clear();
            cin.ignore(1, '\n');
        }
    }
}

TradeInfo TextInput::getTrade() {
    string input;
    Resource r_1, r_2;
    Colour c;

    while (true) {
        cin >> input;
        if (input == "Blue" || input == "blue") {
            c = Colour::Blue;
        } else if (input == "Red" || input == "red") {
            c = Colour::Red;
        } else if (input == "Orange" || input == "orange") {
            c = Colour::Orange;
        } else if (input == "Yellow" || input == "yellow") {
            c = Colour::Yellow;
        } else {
            getline(cin, input);
            continue;
        }


        cin >> input;
        if (input == "TUTORIAL" || input == "tutorial") {
            r_1 = Resource::TUTORIAL;
        } else if (input == "LECTURE" || input == "lecture") {
            r_1 = Resource::LECTURE;
        } else if (input == "STUDY" || input == "study") {
            r_1 = Resource::STUDY;
        } else if (input == "CAFFEINE" || input == "caffeine") {
            r_1 = Resource::CAFFEINE;
        } else if (input == "LAB" || input == "lab") {
            r_1 = Resource::LAB;
        } else {
            getline(cin, input);
            continue;
        }

        cin >> input;
        if (input == "TUTORIAL" || input == "tutorial") {
            r_2 = Resource::TUTORIAL;
        } else if (input == "LECTURE" || input == "lecture") {
            r_2 = Resource::LECTURE;
        } else if (input == "STUDY" || input == "study") {
            r_2 = Resource::STUDY;
        } else if (input == "CAFFEINE" || input == "caffeine") {
            r_2 = Resource::CAFFEINE;
        } else if (input == "LAB" || input == "lab") {
            r_2 = Resource::LAB;
        } else {
            getline(cin, input);
            continue;
        }

        break;
    }
    return {c, r_1, r_2};
}

Response TextInput::getResponse() {
    string input;

    while (true) {
        cin >> input;

        if (input == "Yes" || input == "yes") {
            return Response::Yes;
        } else if (input == "No" || input =="no") {
            return Response::No;
        }
        cin.clear();
        cin.ignore(1, '\n');
    }
}

string TextInput::getSaveFile() {
    string input;
    cin >> input;
    return input;
}

Colour TextInput::getStealChoice(const vector<Colour> choices) {
    string input;
    Colour c;
    while (true) {
        cin >> input;
        if (input == "Blue" || input == "blue") {
            c = Colour::Blue;
        } else if (input == "Red" || input == "red") {
            c = Colour::Red;
        } else if (input == "Orange" || input == "orange") {
            c = Colour::Orange;
        } else if (input == "Yellow" || input == "yellow") {
            c = Colour::Yellow;
        } else {
            getline(cin, input);
            continue;
        }

        for (Colour cl : choices) {
            if (cl == c) {
                return c;
            }
        }
        cin.ignore(1, '\n');
        
    }
}
