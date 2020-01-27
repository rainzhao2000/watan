#include <sstream>
#include "textdisplay.h"
#include "subject.h"
#include "state.h"
using namespace std;



const string name[6] = {"CAFFEINE", "LAB", "STUDY", 
"LECTURE", "TUTORIAL", "NETFLIX"};

TextDisplay::TextDisplay(vector<string> initialConditions): TextDisplay() {
    if (initialConditions.size() != 7 && initialConditions.size() != 6) {
        cout << "Invalid Save File" << endl;
        return;
    }
    if (stoi(initialConditions.at(0)) == 0) {
        curStudent = Colour::Blue;
    } else if (stoi(initialConditions.at(0)) == 1) {
        curStudent = Colour::Red;
    } else if (stoi(initialConditions.at(0)) == 2) {
        curStudent = Colour::Orange;
    } else if (stoi(initialConditions.at(0)) == 3) {
        curStudent = Colour::Yellow;
    }

    for (int i = 0 ; i < 4 ; i++) {
        string player;
        switch(i) {
            case 0: player = 'B'; break;
            case 1: player = 'R'; break;
            case 2: player = 'O'; break;
            case 3: player = 'Y'; break;
        }

        stringstream ss(initialConditions.at(i+1));
        string word;

        ss >> word;
        studentResources.at(i).at(Resource::CAFFEINE) = stoi(word);
        
        ss >> word;
        studentResources.at(i).at(Resource::LAB) = stoi(word);

        ss >> word;
        studentResources.at(i).at(Resource::LECTURE) = stoi(word);

        ss >> word;
        studentResources.at(i).at(Resource::STUDY) = stoi(word);

        ss >> word;
        studentResources.at(i).at(Resource::TUTORIAL) = stoi(word);

        ss >> word;
        if (word != "g") {
            cerr << "Invalid save file" << endl;
        }
        
        ss >> word;
        while (word != "c") {
            
            boardGoals.at(stoi(word)) = player + " ";
        }

        while (ss >> word) {
            int index = stoi(word);
            ss >> word;
            string crit_tier;
            switch (stoi(word)) {
                case 1: crit_tier = 'A'; break;
                case 2: crit_tier = 'M'; break;
                case 3: crit_tier = 'E'; break;
            }
            boardCriteria.at(index) = player + crit_tier;

            if (studentCriteria.at(i).size() == 0) {
                studentCriteria.at(i).push_back({stoi(word), 1});
            }

            if (studentCriteria.at(i).at(studentCriteria.at(i).size() - 1).at(0) == stoi(word)) {
                studentCriteria.at(i).at(studentCriteria.at(i).size() - 1).at(1)++;
            } else {
                studentCriteria.at(i).push_back({stoi(word), 1});
            }
        }
    }
    stringstream ss(initialConditions.at(5));
    int num;
    for (int i = 0 ; i < 19 ; i++) {
        ss >> num;
        switch (num) {
            case 0: boardTiles.at(i) = "CAFFEINE"; break;
            case 1: boardTiles.at(i) = "LAB"; break;
            case 2: boardTiles.at(i) = "LECTURE"; break;
            case 3: boardTiles.at(i) = "STUDY"; break;
            case 4: boardTiles.at(i) = "TUTORIAL"; break;
            case 5: boardTiles.at(i) = "NETFLIX"; break;
        }
        ss >> num;
        if (num == 0 || num == 7) {
            boardProb.at(i) = "  ";
            continue;
        }
        boardProb.at(i) = (num < 10 ? " " + to_string(num) : to_string(num));
    }
    if (initialConditions.size() == 7) {
        geesePos = stoi(initialConditions.at(6));
    } else {
        geesePos = -1;
    }
}

TextDisplay::TextDisplay() {
    //Setting up the default board layout
    vector<int> probs = {3, 10, 5, 4, 0, 10, 11, 3, 8, 2, 6, 8, 12, 5, 11, 4, 6, 9, 9};
    for (int i = 0 ; i < 54 ; i++) {
        if (i < 10) {
            boardCriteria.push_back(" " + to_string(i));
        } else {
            boardCriteria.push_back(to_string(i));
        }
        
    }
    for (int i = 0 ; i < 72 ; i++) {
        if (i < 10) {
            boardGoals.push_back(" " + to_string(i));
        } else {
            boardGoals.push_back(to_string(i));
        }
    }
    for (int i = 0 ; i < 19 ; i++) {
        if (probs.at(i) == 0) {
            boardProb.push_back("  ");
        } else if (probs.at(i) < 10) {
            boardProb.push_back(" " + to_string(probs.at(i)));
        }
        else {
            boardProb.push_back(to_string(probs.at(i)));
        }
    }

    for (int i = 0 ; i < 19 ; i++) {
        if (i == 7 || i == 9 || i == 10) {
            boardTiles.push_back(name[0]);
        } else if (i == 1 || i == 3 || i == 11 || i == 13) {
            boardTiles.push_back(name[1]);
        } else if (i == 4) {
            boardTiles.push_back(name[5]);
        } else if (i == 5 || i == 8) {
            boardTiles.push_back(name[2]);
        } else if (i == 12 || i == 14 || i == 16) {
            boardTiles.push_back(name[4]);
        } else {
            boardTiles.push_back(name[3]);
        }
    }

    //Setting the resourse count to zero or each player:
    for (int i = 0 ; i < 4 ; i++){
        map<Resource, int> temp_1 = {{Resource::CAFFEINE, 0},
        {Resource::LAB, 0}, {Resource::LECTURE, 0}, {Resource::STUDY, 0},
        {Resource::TUTORIAL, 0}};
        vector<vector<int>> temp_3 = {{}};
        studentResources.push_back(temp_1);
        studentCriteria = {{}, {}, {}, {}};
    }

    geesePos = -1;
}

void TextDisplay::displayWinner(const Colour winner) const {
    cout << "Student " << winner << " won!" << endl;
}

void TextDisplay::displayCurStudent() const {
    cout << "Student " << curStudent << "'s turn." << endl;
    cout << curStudent << " has ";

    int index, sum_crit = 0;
    if (curStudent == Colour::Blue) {
        index = 0;
    } else if (curStudent == Colour::Red) {
        index = 1;
    } else if (curStudent == Colour::Orange) {
        index = 2;
    } else if (curStudent == Colour::Yellow) {
        index = 3;
    }

    for (auto &i : studentCriteria.at(index)) {
        sum_crit += i.at(1);
    }
    cout << sum_crit << " course criteria, " << 
    studentResources.at(index).at(Resource::CAFFEINE) << " caffeines, " <<
    studentResources.at(index).at(Resource::LAB) << " labs, " <<
    studentResources.at(index).at(Resource::LECTURE) << " lectures, " <<
    studentResources.at(index).at(Resource::STUDY) << " studies, and " <<
    studentResources.at(index).at(Resource::TUTORIAL) << " tutorials." << endl;
    
}

void TextDisplay::displayStatus() const {
    int sum_crit;
    for (int i = 0 ; i < 4 ; i++) {
        sum_crit = 0;
        for (auto &j : studentCriteria.at(i)) {
            sum_crit += j.at(1);
        }
        
        switch (i) {
            case 0: cout << Colour::Blue; break;
            case 1: cout << Colour::Red; break;
            case 2: cout << Colour::Orange; break;
            case 3: cout << Colour::Yellow; break;
        }

        cout << " has ";
        


        cout << sum_crit << " course criteria, " << 
        studentResources.at(i).at(Resource::CAFFEINE) << " caffeines, " <<
        studentResources.at(i).at(Resource::LAB) << " labs, " <<
        studentResources.at(i).at(Resource::LECTURE) << " lectures, " <<
        studentResources.at(i).at(Resource::STUDY) << " studies, and " <<
        studentResources.at(i).at(Resource::TUTORIAL) << " tutorials." << endl;
    }
}

void TextDisplay::displayCriteria() const {
    cout << curStudent << " has completed:" << endl;

    int index;
    if (curStudent == Colour::Blue) {
        index = 0;
    } else if (curStudent == Colour::Red) {
        index = 1;
    } else if (curStudent == Colour::Orange) {
        index = 2;
    } else if (curStudent == Colour::Yellow) {
        index = 3;
    }

    for (auto &i : studentCriteria.at(index)) {
        cout << i.at(1) << " " << i.at(0) << endl;
    }

}

void TextDisplay::promptLoadedDice() const {
    cout << "Input a roll between 2 and 12:\t";
}

void TextDisplay::promptInitialAssignment() const {
    cout << "Student, " << curStudent << ", where do you want to complete an Assignment?" << endl;
    cout << "Type \"complete <index>\" to choose a criterion or \"quit\" to quit:" << endl;
}

void TextDisplay::displayStealChoices(const vector<Colour> students) const {
    cout << "Student " << curStudent << " can choose to steal from:" << endl;
    for (Colour c : students) {
        cout << c << endl;
    }
    cout << endl;
    cout << "Choose a student to steal from" << endl;
}

void TextDisplay::promptTrade(TradeInfo deal) const {
    cout << curStudent << " offers " << deal.target << " one " << deal.offer <<
     " for one " << deal.want << endl;
    cout << "Does " << deal.target << " accept this offer?" << endl;
}

void TextDisplay::promptPlayAgain() const {
    cout << "Would you like to play again?" << endl;
}

void TextDisplay::displayHelp() const {
    cout << "Valid commands:" << endl;
    cout << "board" << endl;
    cout << "status" << endl << "criteria" << endl;
    cout << "achieve <goal>" << endl;
    cout << "complete <criterion>" << endl;
    cout << "improve <criterion>" << endl;
    cout << "trade <colour> <give> <take>" << endl;
    cout << "next" << endl << "save <file>" << endl;
    cout << "help" << endl;
}

void TextDisplay::draw_top(int& c, int& t, int& g) const {
    for (int i = 0 ; i < 35 ; i++) {
        cout << " ";
    }
    cout << "|" + boardCriteria.at(c) + "|--" + boardGoals.at(g) + "--|" + boardCriteria.at(c + 1) + "|" << endl;
    c += 2;
    g++;
    for (int i = 0 ; i < 35 ; i++) {
        cout << " ";
    }

    cout << "/";
    for (int i = 0 ; i < 12 ; i++) {
        cout << " ";
    }
    cout << "\\" << endl;
    for (int i = 0 ; i < 33 ; i ++) {
        cout << " ";
    }
    cout << boardGoals.at(g);
    g++;
    cout << "       0     " << boardGoals.at(g) << endl;
    g++;

    
    for (int i = 0 ; i < 33 ; i++) {
        cout << " ";
    }

    cout << "/";
    cout << "     ";
    cout << boardTiles[t];
    for (unsigned i = 0 ; i < 11 - boardTiles[t].size() ; i++) {
        cout << " ";
    }
    t++;
    cout << "\\";
    cout << endl;

    for (int i = 0 ; i < 20 ; i++) {
        cout << " ";
    }
    cout << "|" << boardCriteria.at(c) << "|--" << boardGoals.at(g) << "--|" << boardCriteria.at(c + 1) << "|";
    g++;
    c += 2;
    for (int i = 0 ; i < 7 ; i ++) {
        cout << " ";
    }
    cout << boardProb.at(t - 1) << "       |" << boardCriteria.at(c) << "|--" << boardGoals.at(g) << "--|" << boardCriteria.at(c + 1) << "|" << endl;
    c += 2;
    g++;
    for (int i = 0 ; i < 20 ; i++) {
        cout << " ";
    }
    cout << "/";
    for (int i = 0 ; i < 12 ; i++) {
        cout << " ";
    }
    cout << "\\";
    cout << "     ";
    cout << (geesePos == t - 1 ? "GEESE" : "     ");
    cout << "      ";
    cout << "/";
    for (int i = 0 ; i < 12 ; i++) {
        cout << " ";
    }
    cout << "\\" << endl;
    for (int i = 0 ; i < 18 ; i++) {
        cout << " ";
    }
    cout << boardGoals.at(g);
    g++;

    cout << "      " << (t < 10 ? " " + to_string(t) : to_string(t)) << "     " << boardGoals.at(g);
    g++;
    cout << "             " << boardGoals.at(g);
    g++;
    cout << "      " << (t + 1 < 10 ? " " + to_string(t + 1) : to_string(t + 1)) << "     " << boardGoals.at(g) << endl;
    g++;


    for (int i = 0 ; i < 18 ; i++) {
        cout << " ";
    }
    cout << "/";
    cout << "     ";
    cout << boardTiles[t];
    for (unsigned i = 0 ; i < 11 - boardTiles[t].size() ; i++) {
        cout << " ";
    }
    t++;
    cout << "\\";
    for (int i = 0 ; i < 12 ; i++) {
        cout << " ";
    }
    cout << "/";
    cout << "     ";
    cout << boardTiles[t];
    for (unsigned i = 0 ; i < 11 - boardTiles[t].size() ; i++) {
        cout << " ";
    }
    t++;
    cout << "\\";
    cout << endl;
}

void TextDisplay::draw_bottom(int& c, int& t, int& g) const {
    for (int i = 0 ; i < 5 ; i++) {
        cout << " ";
    }
    for (int i = 0 ; i < 2 ; i++) {
        cout << "|" << boardCriteria.at(c) << "|--" << boardGoals.at(g) << "--|" << boardCriteria.at(c + 1) << "|";
        c+= 2;
        g++;
        cout << "       " << boardProb.at(t - 2 + i) << "       ";
    }
    cout << "|" << boardCriteria.at(c) << "|--" << boardGoals.at(g) << "--|" << boardCriteria.at(c + 1) << "|";
    c+= 2;
    g++;
    cout << endl;

    for (int i = 0 ; i < 18 ; i++) {
        cout << " ";
    }

    cout << "\\";
    cout << "     ";
    cout << (geesePos == t - 2 ? "GEESE" : "     ");
    cout << "      ";
    cout << "/";
    for (int i = 0 ; i <  12 ; i++) {
        cout << " ";
    }
    cout << "\\";
    cout << "     ";
    cout << (geesePos == t - 1 ? "GEESE" : "     ");
    cout << "      ";
    cout << "/";

    cout << endl;

    for (int i = 0 ; i < 18 ; i++) {
        cout << " ";
    }
    cout << boardGoals.at(g);
    g++;

    for (int i = 0 ; i < 13 ; i++) {
        cout << " ";
    }
    cout << boardGoals.at(g);
    g++;
    cout << "      " << t << "     ";
    cout << boardGoals.at(g);
    g++;
    for (int i = 0 ; i < 13 ; i++) {
        cout << " ";
    }
    cout << boardGoals.at(g);
    g++;
    cout << endl;

    for (int i = 0 ; i < 20 ; i++) {
        cout << " ";
    }
    cout << "\\";
    for (int i = 0 ; i < 12 ; i++) {
        cout << " ";
    }
    cout << "/";
    cout << "     ";
    cout << boardTiles[t];
    for (unsigned i = 0 ; i < 11 - boardTiles[t].size() ; i++) {
        cout << " ";
    }
    t++;
    cout << "\\";
    for (int i = 0 ; i < 12 ; i++) {
        cout << " ";
    }
    cout << "/";
    cout << endl;

    for (int i = 0 ; i < 20 ; i++) {
        cout << " ";
    }
    cout << "|" << boardCriteria.at(c) << "|--" << boardGoals.at(g) << "--|" << boardCriteria.at(c + 1) << "|";
    c += 2;
    g++;
    for (int i = 0 ; i < 7 ; i ++) {
        cout << " ";
    }
    cout << boardProb.at(t - 1) << "       |" << boardCriteria.at(c) << "|--" << boardGoals.at(g) << "--|" << boardCriteria.at(c + 1) << "|" << endl;
    c += 2;
    g++;

    for (int i = 0 ; i < 33 ; i++) {
        cout << " ";
    }
    cout << "\\";
    cout << "     ";
    cout << (geesePos == t - 1 ? "GEESE" : "     ");
    cout << "      ";
    cout << "/";
    cout << endl;

    for (int i = 0 ; i < 33 ; i++) {
        cout << " ";
    }

    cout << boardGoals.at(g);
    g++;
    for (int i = 0 ; i < 13 ; i++) {
        cout << " ";
    }
    cout << boardGoals.at(g);
    g++;
    cout << endl;

    for (int i = 0 ; i < 35 ; i++) {
        cout << " ";
    }
    cout << "\\";
    for (int i = 0 ; i < 12 ; i++) {
        cout << " ";
    }
    cout << "/";
    cout << endl;

    for (int i = 0 ; i < 35 ; i++) {
        cout << " ";
    }
    

    cout << "|" + boardCriteria.at(c) + "|--" + boardGoals.at(g) + "--|" + boardCriteria.at(c + 1) + "|" << endl;
    c += 2;
    g++;
    
}

void TextDisplay::draw_row(int& c, int& t, int& g) const {
    for (int i = 0 ; i < 5 ; i++) {
        cout << " ";
    }
    for (int i = 0 ; i < 2 ; i++) {
        cout << "|" << boardCriteria.at(c) << "|--" << boardGoals.at(g) << "--|" << boardCriteria.at(c + 1) << "|";
        c+= 2;
        g++;
        cout << "       " << boardProb.at(t - 2 + i) << "       ";
    }
    cout << "|" << boardCriteria.at(c) << "|--" << boardGoals.at(g) << "--|" << boardCriteria.at(c + 1) << "|";
    c+= 2;
    g++;
    cout << endl;

    for (int i = 0 ; i < 5 ; i++) {
        cout << " ";
    }

    for (int j = 0 ; j < 2 ; j++) {
        cout << "/";
        for (int i = 0 ; i < 12 ; i++) {
            cout << " ";
        }
        cout << "\\";
        cout << "     ";
        cout << (geesePos == t - 2 + j ? "GEESE" : "     ");
        cout << "      ";
    }
    cout << "/";
    for (int i = 0 ; i < 12 ; i++) {
        cout << " ";
    }
    cout << "\\";
    cout << endl;
    
    cout << "   ";
    cout << boardGoals.at(g);
    g++;
    cout << "      " << (t < 10 ? " " + to_string(t) : to_string(t)) << "     " << boardGoals.at(g);
    g++;
    for (int i = 0 ; i < 13 ; i++) {
        cout << " ";
    }
    cout << boardGoals.at(g);
    g++;
    cout << "      " << (t + 1 < 10 ? " " + to_string(t + 1) : to_string(t + 1)) << "     " << boardGoals.at(g);
    g++;
    for (int i = 0 ; i < 13 ; i++) {
        cout << " ";
    }
    cout << boardGoals.at(g);
    g++;
    cout << "      " << (t + 2 < 10 ? " " + to_string(t + 2) : to_string(t + 2)) << "     " << boardGoals.at(g);
    g++;
    cout << endl;


    cout << "   ";
    cout << "/";
    cout << "     ";
    cout << boardTiles[t];
    for (unsigned i = 0 ; i < 11 - boardTiles[t].size() ; i++) {
        cout << " ";
    }
    cout << "\\";
    t++;
    for (int i = 0 ; i < 12 ; i++) {
        cout << " ";
    }
    cout << "/";
    cout << "     ";
    cout << boardTiles[t];
    for (unsigned i = 0 ; i < 11 - boardTiles[t].size() ; i++) {
        cout << " ";
    }
    cout << "\\";
    t++;
    for (int i = 0 ; i < 12 ; i++) {
        cout << " ";
    }
    cout << "/";
    cout << "     ";
    cout << boardTiles[t];
    for (unsigned i = 0 ; i < 11 - boardTiles[t].size() ; i++) {
        cout << " ";
    }
    cout << "\\";
    t++;
    cout << endl;

    for (int i = 0 ; i < 2; i++) {
        cout << "|";
        cout << boardCriteria.at(c) << "|";
        c++;
        cout << "       " << boardProb.at(t-3 + i);
        cout << "       |" << boardCriteria.at(c) << "|--" << boardGoals.at(g) << "--";
        c++;
        g++;
    }
    cout << "|";
    cout << boardCriteria.at(c) << "|";
    c++;
    cout << "       " << boardProb.at(t-1);
    cout << "       |" << boardCriteria.at(c) << "|";
    c++;
    cout << endl;

    cout << "   ";
    cout << "\\";
    cout << "     ";
    cout << (geesePos == t - 3 ? "GEESE" : "     ");
    cout << "      ";
    cout << "/";
    for (int i = 0 ; i < 12 ; i++) {
        cout << " ";
    }
    cout << "\\";
    cout << "     ";
    cout << (geesePos == t - 2 ? "GEESE" : "     ");
    cout << "      ";
    cout << "/";
    for (int i = 0 ; i < 12 ; i++) {
        cout << " ";
    }
    cout << "\\";
    cout << "     ";
    cout << (geesePos == t - 1 ? "GEESE" : "     ");
    cout << "      ";
    cout << "/";
    for (int i = 0 ; i < 12 ; i++) {
        cout << " ";
    }
    cout << endl;

    cout << "   ";
    cout << boardGoals.at(g);
    g++;
    for (int i = 0 ; i < 13 ; i++) {
        cout << " ";
    }
    cout << boardGoals.at(g);
    g++;
    cout << "      ";
    cout << (t < 10 ? " " + to_string(t) : to_string(t));
    cout << "     ";
    cout << boardGoals.at(g);
    g++;
    for (int i = 0 ; i < 13 ; i++) {
        cout << " ";
    }
    cout << boardGoals.at(g);
    g++;
    cout << "      ";
    cout << (t + 1 < 10 ? " " + to_string(t + 1) : to_string(t + 1));
    cout << "     ";cout << boardGoals.at(g);
    g++;
    for (int i = 0 ; i < 13 ; i++) {
        cout << " ";
    }
    cout << boardGoals.at(g);
    g++;
    cout << endl;

    for (int i = 0 ; i < 5 ; i++) {
        cout << " ";
    }

    for (int j = 0 ; j < 2 ; j++) {
        cout << "\\";
        for (int i = 0 ; i < 12 ; i++) {
            cout << " ";
        }
        cout << "/";
        cout << "     ";
        cout << boardTiles[t];
        for (unsigned i = 0 ; i < 11 - boardTiles[t].size() ; i++) {
            cout << " ";
        }
        t++;
    }
    cout << "\\";
    for (int i = 0 ; i < 12 ; i++) {
        cout << " ";
    }
    cout << "/";
    cout << endl;


}

void TextDisplay::displayBoard() const{
    int crit_cout = 0, tile_count = 0, goal_count = 0;
    draw_top(crit_cout, tile_count, goal_count);
    draw_row(crit_cout, tile_count, goal_count);
    draw_row(crit_cout, tile_count, goal_count);
    draw_row(crit_cout, tile_count, goal_count);
    draw_bottom(crit_cout, tile_count, goal_count);
}

void TextDisplay::displayGainedResource(const map<Resource, int> resources,
const Colour student) const {
    cout << "Student " << student << " gained:" << endl;
    for (auto &r : resources) {
        cout << r.second << " " << r.first << endl;
    }
}

void TextDisplay::displayNoGainedResources() const {
    cout << "No student gained resources." << endl;
}

void TextDisplay::displayInvalidBuild() const {
  cout << "You cannot build here." << endl;
}

void TextDisplay::displayInvalidCommand() const {
    cout << "Invalid command." << endl;
}

void TextDisplay::displayInsufficientResources() const {
    cout << "You do not have enough resources." << endl;
}

void TextDisplay::displayInvalidInput() const {
    cout << "Invalid input." << endl;
}

void TextDisplay::displayNoStealChoices(Colour attacker) const {
  cout << "Student " << attacker << " has no students to steal from." << endl;
}

void TextDisplay::displayStealResult(const Resource stolen,
const Colour student) const {
    cout << "Student " << curStudent << " steals " << stolen <<
     " from student " << student << "." << endl;
}

void TextDisplay::displayGeeseRoll(const vector<Colour> students,
const vector<map<Resource, int>> lost) const {
    for (unsigned i = 0 ; i < students.size() ; i++) {
        cout << "Student " << students.at(i) << " losses ";
        int sum = 0;
        for (auto &r : lost.at(i)) {
            sum += r.second;
        }
        cout << sum << " to the geese. They lose:" << endl;
        for (auto &r : lost.at(i)) {
            cout << r.second << " " << r.first << endl;
        }
    }
    cout << "Choose where to place GEESE." << endl;
}

void TextDisplay::notify(Subject<WatanState> &whoFrom) {
    curStudent = whoFrom.getState().curTurn;
}

void TextDisplay::notify(Subject<StudentState> &whoFrom) {
    int index;
    if (whoFrom.getState().appliedTo == Colour::Blue) {
        index = 0;
    } else if (whoFrom.getState().appliedTo == Colour::Red) {
        index = 1;
    } else if (whoFrom.getState().appliedTo == Colour::Orange) {
        index = 2;
    } else if (whoFrom.getState().appliedTo == Colour::Yellow) {
        index = 3;
    }
    for (auto &r: whoFrom.getState().resourceChange) {
        studentResources.at(index).at(r.first) += r.second;
    }
}

void TextDisplay::notify(Subject<CriterionState> &whoFrom) {
    int index;
    string player, crit;
    if (whoFrom.getState().newOwner == Colour::Blue) {
        index = 0;
        player = 'B';
    } else if (whoFrom.getState().newOwner == Colour::Red) {
        index = 1;
        player = 'R';
    } else if (whoFrom.getState().newOwner == Colour::Orange) {
        index = 2;
        player = 'O';
    } else if (whoFrom.getState().newOwner == Colour::Yellow) {
        index = 3;
        player = 'Y';
    }

    int crit_index;
    if (whoFrom.getState().newTier == CriterionTier::Assignment) {
        crit_index = 1;
        crit = 'A';
    } else if (whoFrom.getState().newTier == CriterionTier::Midterm) {
        crit_index = 2;
        crit = 'M';
    } else if (whoFrom.getState().newTier == CriterionTier::Exam) {
        crit_index = 3;
        crit = 'E';
    }

    
    

    if (studentCriteria.at(index).size() != 0 && studentCriteria.at(index).at(studentCriteria.at(index).size() - 1).at(0)
     == crit_index) {
        studentCriteria.at(index).at(studentCriteria.at(index).size() - 1).at(1)++;
    } else {
        studentCriteria.at(index).push_back({crit_index, 1});
    }

    boardCriteria.at(whoFrom.getState().index) = player + crit;
}

void TextDisplay::notify(Subject<TileState> &whoFrom) {
    if (whoFrom.getState().hasGeese) {
        geesePos = whoFrom.getState().index;
    }
}

void TextDisplay::notify(Subject<GoalState> &whoFrom) {
    string player;
    switch(whoFrom.getState().newOwner) {
        case Colour::Blue: player = 'B'; break;
        case Colour::Red: player = 'R'; break;
        case Colour::Orange: player = 'O'; break;
        case Colour::Yellow: player = 'Y'; break;
        case Colour::NoColour: break;
    }
    boardGoals.at(whoFrom.getState().index) = player + " ";
}

void TextDisplay::notify(Subject<DiceState> &whoFrom) {
    int value = whoFrom.getState().diceVal;
    cout << "Dice rolled: " << value << "." << endl;
}
