#include <sstream>
#include <random>
#include <chrono>
#include <set>
#include <fstream>
#include "watan.h"
#include "textinput.h"

Watan::Watan(unsigned int seed):
  in{new TextInput{}},
  turnOrder{{Colour::Blue, Colour::Red, Colour::Orange, Colour::Yellow}},
  gameSeed{seed} {
  // create init conditions
  std::stringstream ss;
  ss << static_cast<int>(this->turnOrder.at(0));
  std::string curTurn = ss.str();
  std::vector<std::string> initialConditions{
    curTurn,
    "0 0 0 0 0 g c",
    "0 0 0 0 0 g c",
    "0 0 0 0 0 g c",
    "0 0 0 0 0 g c",
    this->randomBoardString()
    // no geese at start of game
  };
  loaded = false;
  // init
  this->init(initialConditions); 
}

Watan::Watan(const std::string &loadFilename, unsigned int seed):
  in{new TextInput{}},
  turnOrder{{Colour::Blue, Colour::Red, Colour::Orange, Colour::Yellow}},
  gameSeed{seed} {

  std::stringstream ss;
  ss << static_cast<int>(this->turnOrder.at(0));
  std::string curTurn = ss.str();
  
  // load file
  std::ifstream file{loadFilename};
  std::vector<std::string> initialConditions;
  std::string line;
  getline(file, line);
  if (line.find(' ') == std::string::npos) {
    loaded = true;
    // Save file
    initialConditions = {};
    initialConditions.emplace_back(line);
    for (int i = 0 ; i < 5 ; i++) {
      getline(file, line);
      initialConditions.emplace_back(line);
    }
    if(getline(file, line)) {
      initialConditions.emplace_back(line);
    }
  } else {
    loaded = false;
    // Board file
    initialConditions = {
      curTurn,
      "0 0 0 0 0 g c",
      "0 0 0 0 0 g c",
      "0 0 0 0 0 g c",
      "0 0 0 0 0 g c",
      line};
  }
  // init
  this->init(initialConditions);
}


Watan::~Watan() {
  delete td;
  delete in;
  for(auto &stu : this->students) {
    delete stu.second;
  }
  delete board;
}

std::string Watan::randomBoardString() const {
  int probs[] = {3, 10, 5, 4, 10, 11, 3, 8, 2, 6, 8, 12, 5, 11, 4, 6, 9, 9};
  int resources[] = {4, 4, 4, 3, 3, 1};
  std::string board_str = "";
  /* COFFEINE 0
  TUTORIAL 4
  STUDY 3
  LAB 1
  LECTURE 2
  NETFLIX 5*/
  std::default_random_engine generator(this->gameSeed);
  std::uniform_int_distribution<int> probs_dist(0, 17);
  std::uniform_int_distribution<int> resources_dist(0, 5);
  for (int i = 0 ; i < 19 ; i++) {
    int random;
    while (true) {
      random = resources_dist(generator);
      if (resources[random] != 0) {
        board_str += std::to_string(random) + " ";
        resources[random]--;
        
        break;
      }
    }
    if (random == 5) {
      board_str += "0 ";
      continue;
    }
    while (true) {
      
      random = probs_dist(generator);
      if (probs[random] != -1) {
        board_str += std::to_string(probs[random]) + " ";
        probs[random] = -1;
        break;
      }
    }
  }
  
  // "0 3 1 10 3 5 1 4 5 7 3 10 2 11 0 3 3 8 0 2 0 6 1 8 4 12 1 5 4 11 2 4 4 6 2 9 2 9"
  return board_str;
}

void Watan::loadResources(std::map<Resource, int> &rmap, std::stringstream &data) {
  std::string word;
  for(auto it = rmap.begin();
      it != rmap.end() && (data >> word) && word != "g";
      ++it) {
    std::stringstream ss{word};
    if(!(ss >> it->second)) throw "invalid read of student resource";
  }
}

void Watan::loadGoals(Colour student, std::vector<Goal *> goals, std::stringstream &data) {
  std::string line = "";
  while (data >> line && line != "c" && line != "g") {
    goals.at(std::stoi(line))->setOwner(student);
  }
}

void Watan::loadCriteria(Colour student, std::vector<Criterion *> criteria, std::stringstream &data) {
  std::string line = "";
  while (data >> line && line != "c") {
    int index = std::stoi(line);
    this->board->improve(student, index, true);
    data >> line;
    if (std::stoi(line) == 2) {
      this->board->improve(student, index, true);
    } else {
      this->board->improve(student, index, true);
      this->board->improve(student, index, true);
    }
  }

}

void Watan::init(const std::vector<std::string> &initialConditions) { 
  std::stringstream curTurn{initialConditions.at(0)};
  std::stringstream blueData{initialConditions.at(1)};
  std::stringstream redData{initialConditions.at(2)};
  std::stringstream orangeData{initialConditions.at(3)};
  std::stringstream yellowData{initialConditions.at(4)};
  std::stringstream boardData{initialConditions.at(5)};

  delete this->td;
  this->td = new TextDisplay{initialConditions};
  curTurn >> this->firstTurn;

  // default student resources
  std::map<Resource, int> blueResources = {
    {Resource::CAFFEINE, 0},
    {Resource::LAB, 0},
    {Resource::LECTURE, 0},
    {Resource::STUDY, 0},
    {Resource::TUTORIAL, 0},
  };
  std::map<Resource, int> redResources = blueResources;
  std::map<Resource, int> orangeResources = blueResources;
  std::map<Resource, int> yellowResources = blueResources;

  // parse student resources
  this->loadResources(blueResources, blueData);
  this->loadResources(redResources, redData);
  this->loadResources(orangeResources, orangeData);
  this->loadResources(yellowResources, yellowData);

  // student costs
  std::map<Resource, int> assnCost = {
    {Resource::CAFFEINE, 1},
    {Resource::LAB, 1},
    {Resource::LECTURE, 1},
    {Resource::TUTORIAL, 1},
  };
  std::map<Resource, int> midCost = {
    {Resource::STUDY, 3},
    {Resource::LECTURE, 2}
  };
  std::map<Resource, int> examCost = {
    {Resource::CAFFEINE, 3},
    {Resource::LAB, 2},
    {Resource::LECTURE, 2},
    {Resource::STUDY, 2},
    {Resource::TUTORIAL, 1},
  };
  std::map<Resource, int> goalCost = {
    {Resource::STUDY, 1},
    {Resource::TUTORIAL, 1},
  };

  for(auto &student : this->students) {
    delete student.second;
  }
  delete this->board;

  // init students
  this->students = {
    {Colour::Blue, new Student{this, this->td, this->in, Colour::Blue,
      blueResources, assnCost, midCost, examCost, goalCost}},
    {Colour::Red, new Student{this, this->td, this->in, Colour::Red,
      redResources, assnCost, midCost, examCost, goalCost}},
    {Colour::Orange, new Student{this, this->td, this->in, Colour::Orange,
      orangeResources, assnCost, midCost, examCost, goalCost}},
    {Colour::Yellow, new Student{this, this->td, this->in, Colour::Yellow,
      yellowResources, assnCost, midCost, examCost, goalCost}},
  };

  // init goals
  std::vector<Goal *> goals;
  for(int i = 0; i < 72; ++i) {
    Goal *goal = new Goal{i};
    goals.emplace_back(goal);
    //add td as observer for goal
    goal->attach(this->td);
  }
  // load goals for students
  this->loadGoals(Colour::Blue, goals, blueData);
  this->loadGoals(Colour::Red, goals, redData);
  this->loadGoals(Colour::Orange, goals, orangeData);
  this->loadGoals(Colour::Yellow, goals, yellowData);

  // init criteria
  std::vector<Criterion *> criteria;
  for(int i = 0; i < 54; ++i) {
    Criterion *crit = new Criterion{i};
    criteria.emplace_back(crit);
    //add td as observer for criterion
    crit->attach(this->td);
  }
  // specify neighbours
  int neighbourCriteria[54][3] = {{1, 3, -1}, {0, 4, -1}, {3, 7, -1}, {0, 2, 8}, {1, 5, 9}, {4, 10, -1}, {7, 12, -1}, {2, 6, 13}, {3, 9, 14}, {4, 8, 15}, {5, 11, 16}, {10, 17, -1}, {6, 18, -1}, {7, 14, 19}, {8, 13, 20}, {9, 16, 21}, {10, 15, 22}, {11, 23, -1}, {12, 19, 24}, {13, 18, 25}, {14, 21, 26}, {15, 20, 27}, {16, 23, 28}, {17, 22, 29}, {18, 30, -1}, {19, 26, 31}, {20, 25, 32}, {21, 28, 33}, {22, 27, 34}, {23, 35, -1}, {24, 31, 36}, {25, 30, 37}, {26, 33, 38}, {27, 32, 39}, {28, 35, 40}, {29, 34, 41}, {30, 42, -1}, {31, 38, 43}, {32, 37, 44}, {33, 40, 45}, {34, 39, 46}, {35, 47, -1}, {36, 43, -1}, {37, 42, 48}, {38, 45, 49}, {39, 44, 50}, {40, 47, 51}, {41, 46, -1}, {43, 49, -1}, {44, 48, 52}, {45, 51, 53}, {46, 50, -1}, {49, 53, -1}, {50, 52, -1}};

  int neighbourGoals[54][3] = {{0, 1, -1}, {0, 2, -1}, {3, 5, -1}, {1, 3, 6}, {2, 4, 7}, {4, 8, -1}, {9, 12, -1}, {5, 9, 13}, {6, 10, 14}, {7, 10, 15}, {8, 11, 16}, {11, 17, -1}, {12, 20, -1}, {13, 18, 21}, {14, 18, 22}, {15, 19, 23}, {16, 19, 24}, {17, 25, -1}, {20, 26, 29}, {21, 26, 30}, {22, 27, 31}, {23, 27, 32}, {24, 28, 33}, {25, 28, 34}, {29, 37, -1}, {30, 35, 38}, {31, 35, 39}, {32, 36, 40}, {33, 36, 41}, {34, 42, -1}, {37, 43, 46}, {38, 43, 47}, {39, 44, 48}, {40, 44, 49}, {41, 45, 50}, {42, 45, 51}, {46, 54, -1}, {47, 52, 55}, {48, 52, 56}, {49, 53, 57}, {50, 53, 58}, {51, 59, -1}, {54, 60, -1}, {55, 60, 63}, {56, 61, 64}, {57, 61, 65}, {58, 62, 66}, {59, 62, -1}, {63, 67, -1}, {64, 67, 69}, {65, 68, 70}, {66, 68, -1}, {69, 71, -1}, {70, 71, -1}};

  //for every criteria
  for(int index = 0; index < 54; ++index){
    //add neighbour criteria
    for(int nbc = 0; nbc < 3; ++nbc){
      if(neighbourCriteria[index][nbc] != -1){
        criteria.at(index)->addNeighbour(criteria.at(neighbourCriteria[index][nbc]));
      }
    }

    //add neighbour goals
    for(int nbg = 0; nbg < 3; ++nbg){
      if(neighbourGoals[index][nbg] != -1){
        criteria.at(index)->addNeighbour(goals.at(neighbourGoals[index][nbg]));
      }
    }
  }

  // load criteria for students
  this->loadCriteria(Colour::Blue, criteria, blueData);
  this->loadCriteria(Colour::Red, criteria, redData);
  this->loadCriteria(Colour::Orange, criteria, orangeData);
  this->loadCriteria(Colour::Yellow, criteria, yellowData);

  // init tiles
  std::vector<Tile *> tiles{};

  int tileCriteria[19][6] = {{0, 1, 3, 4, 8, 9}, {2, 3, 7, 8, 13, 14}, {4, 5, 9, 10, 15, 16}, {6, 7, 12, 13, 18, 19}, {8, 9, 14, 15, 20, 21}, {10, 11, 16, 17, 22, 23}, {13, 14, 19, 20, 25, 26}, {15, 16, 21, 22, 27, 28}, {18, 19, 24, 25, 30, 31}, {20, 21, 26, 27, 32, 33}, {22, 23, 28, 29, 34, 35}, {25, 26, 31, 32, 37, 38}, {27, 28, 33, 34, 39, 40}, {30, 31, 36, 37, 42, 43}, {32, 33, 38, 39, 44, 45}, {34, 35, 40, 41, 46, 47}, {37, 38, 43, 44, 48, 49}, {39, 40, 45, 46, 50, 51}, {44, 45, 49, 50, 52, 53}};

  // link tiles to criteria and parse boardData
  for(int index = 0; index < 19; ++index){
    int resource;
    int value;
    std::string word;

    // get resource for tile
    boardData >> word;
    std::stringstream stringResource{word};
    if(!(stringResource >> resource)) throw "invalid read for tile resource";
    if(resource < 0 || 5 < resource) throw std::out_of_range("resource must be 0-5");

    // get value for tile
    boardData >> word;
    std::stringstream stringValue{word};
    if(!(stringValue >> value)) throw "invalid read for tile value";
    if((value < 2 || 12 < value) && value != 0) throw std::out_of_range("value must be 2-12 or 0 for NETFLIX");

    std::vector<Criterion *> criteriaHolder{};
    for(int nbc = 0; nbc < 6; ++nbc){
      criteriaHolder.emplace_back(criteria.at(tileCriteria[index][nbc]));
    }

    Tile *tileHolder = new Tile{value, static_cast<Resource>(resource), criteriaHolder};
    //add td as observer for tile
    tileHolder->attach(this->td);

    tiles.emplace_back(tileHolder);
  }

  // apply geese if given
  if(initialConditions.size() == 7) {
    std::stringstream geese{initialConditions.at(6)};
    int geeseTile = -1;
    if(!(geese >> geeseTile)) throw "invalid read for geese pos";
    if(geeseTile < 0 || 18 < geeseTile) {
      throw std::out_of_range("geese pos must be 0-18 or -1");
    }
    tiles.at(geeseTile)->setGeese(true);
  }

  this->attach(this->td); // add td as observer for watan
  for(auto &s : this->students) { // add td as observer for students
    s.second->attach(this->td);
  }


  // init board
  this->board = new Board{tiles, criteria, goals};
}

Colour Watan::whoWon() const {
  for(auto stu : this->students) {
    if(stu.second->hasWon()) {
      return stu.first;
    }
  }
  return Colour::NoColour;
}

bool Watan::start(){
  // firstTurn corresponds to an int value of enum Colour
  // curIndex corresponds to the index of turnOrder
  int curIndex = 0;
  for(size_t i = 0; i < this->turnOrder.size(); ++i) {
    if(static_cast<int>(this->turnOrder.at(i)) == this->firstTurn) {
      curIndex = i;
      break;
    }
  }

  if (!loaded) {
    // beginning of game
    // forward order 1st initial completion
    for(size_t i = 0; i < this->turnOrder.size(); ++i) {
      Student *curStudent = this->students.at(this->turnOrder.at(curIndex));
      this->setState(WatanState{curStudent->getColour()});
      this->notifyObservers();
      bool exit = curStudent->beginTurn(true);
      if (exit) {
        return false;
      }
      ++curIndex;
      if(curIndex >= this->turnOrder.size()) {
        curIndex = 0;
      }
    }
    // reverse order 2nd initial completion
    --curIndex;
    for(size_t i = 0; i < this->turnOrder.size(); ++i) {
      if(curIndex < 0) {
        curIndex = this->turnOrder.size() - 1;
      }
      Student *curStudent = this->students.at(this->turnOrder.at(curIndex));
      this->setState(WatanState{curStudent->getColour()});
      this->notifyObservers();
      bool exit = curStudent->beginTurn(true);
      if (exit) {
        return false;
      }
      --curIndex;
    }
    ++curIndex;
    if(curIndex >= this->turnOrder.size()){
      curIndex = 0;
    }
  }
  //while there is no winner
  while(whoWon() == Colour::NoColour){
    Student *curStudent = this->students.at(this->turnOrder.at(curIndex));
    this->setState(WatanState{curStudent->getColour()});
    this->notifyObservers();
    bool exit = curStudent->beginTurn(false);

    if (exit) {
      return false;
    }

    ++curIndex;
    if(curIndex >= turnOrder.size()){
      curIndex = 0;
    }
  }

  //game ended
  this->td->displayWinner(whoWon());
  this->td->promptPlayAgain();
  return this->in->getResponse() == Response::Yes;
}

void Watan::loseToGeese() const {
  std::vector<Colour> affectedStudents; // for output
  std::vector<std::map<Resource, int>> lost; // for output

  for(auto s : this->students) {
    // count up student's resources
    int resourceCount = 0;
    std::map<Resource, int> rmap = s.second->getResources();
    for(auto r : rmap) {
      resourceCount += r.second;
    }
    if(resourceCount >= 10) { // if exceeded, randomly remove a resource till half are removed
      std::map<Resource, int> loss{
        {Resource::CAFFEINE, 0},
        {Resource::LAB, 0},
        {Resource::LECTURE, 0},
        {Resource::STUDY, 0},
        {Resource::TUTORIAL, 0},
      };

      // generate ordered set of unique random indices (1...half)
      size_t half = resourceCount / 2;
      std::set<int> rset;
      unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
      std::default_random_engine generator(seed);
      std::uniform_int_distribution<int> distribution(1, half);
      while(rset.size() < half) {
        rset.emplace(distribution(generator));
      }

      // add resources to loss by iterating till each random index
      int i = 1;
      auto rindex = rset.begin();
      for(auto r : rmap) { // for each resource type
        for(int j = 0; j < r.second; ++j, ++i) { // for each quantity of resource
          if(i == *rindex) { // found one of our random indices
            ++loss.at(r.first); // increment corresponding resource in loss
            ++rindex; // move on to next random index
          }
        }
      }

      // apply loss
      s.second->loseResources(loss);

      // update output
      affectedStudents.emplace_back(s.second->getColour());
      lost.emplace_back(loss);
    }
  }
  this->td->displayGeeseRoll(affectedStudents, lost);
}

void Watan::steal(Colour robber, Colour target) const {
  // count target's resources
  int resourceCount = 0;
  std::map<Resource, int> rmap = this->students.at(target)->getResources();
  for(auto r : rmap) {
    resourceCount += r.second;
  }
  if(resourceCount > 0) {
    // generate random index from 1-resourceCount
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(1, resourceCount);
    int rindex = distribution(generator);

    // find resource to steal by iterating till our random index
    int i = 1;
    for(auto r : rmap) { // for each resource type
      for(int j = 0; j < r.second; ++j, ++i) { // for each quantity of resource
        if(i == rindex) { // found our random index
          std::map<Resource, int> amount{{r.first, 1}};
          this->students.at(target)->loseResources(amount);
          this->students.at(robber)->gainResources(amount);
          this->td->displayStealResult(r.first, target);
          return;
        }
      }
    }
  }
}

void Watan::distributeResources(int rolledNum) const{
  for(auto tile : this->board->getTiles()){
    if(tile->getVal() == rolledNum){
      //this tile gives resources!   
      for(auto criterion : tile->getCriteria()){
        Colour owner = criterion->getOwner();
        //if this criterion is owned by somebody
        if(owner != Colour::NoColour){
          
          //determine how many resources he gets
          int amt = static_cast<int>(criterion->getTier());
          std::map<Resource, int> increment = {{tile->getResource(), amt}};
          
          //increase player's resources
          this->students.at(owner)->gainResources(increment);
        }
      }
    }
  }
}

void Watan::giveTo(Colour student, const std::map<Resource, int> &amount) const {
  this->students.at(student)->gainResources(amount);
}

void Watan::takeFrom(Colour student, const std::map<Resource, int> &amount) const {
  this->students.at(student)->loseResources(amount);
}

std::vector<Colour> Watan::attemptPlaceGeese(Colour attacker, int tileIndex) const {
  std::vector<Colour> stealChoices;
  Tile *tile = this->board->placeGeese(tileIndex);
  if(tile) {
    for(auto &criterion : tile->getCriteria()) { // for each criterion
      if(criterion->getOwner() != Colour::NoColour) { // who has owner
        Student *s = this->students.at(criterion->getOwner());
        for(auto r : s->getResources()) { // for owner's resources
          if(r.second > 0) { // if they have non-zero resources
            stealChoices.emplace_back(s->getColour()); // add them as a steal choice
            break;
          }
        }
      }
    }
  } else {
    stealChoices.emplace_back(Colour::NoColour);
  }
  return stealChoices;
}

bool Watan::attemptAchieve(Colour owner, int goalIndex) const{
  return this->board->achieve(owner, goalIndex);
}

bool Watan::attemptImprove(Colour owner, int criterionIndex, bool initial) const{
  return this->board->improve(owner, criterionIndex, initial);
}

CriterionTier Watan::getCriterionTier(int criterionIndex) const{
  return this->board->getCriteria().at(criterionIndex)->getTier();
}

void Watan::save(const std::string &saveFilename, const bool rolled, const Colour curStudent) const {
  this->save(std::string{saveFilename}, rolled, curStudent);
}

void Watan::save(const std::string &&saveFilename, const bool rolled, 
const Colour curStudent) const {
  std::ofstream output(saveFilename);
  std::string line;
  switch(curStudent) {
    case Colour::Blue: line = (rolled ? std::to_string(1) : std::to_string(0)); 
    break;
    case Colour::Red: line = (rolled ? std::to_string(2) : std::to_string(1)); 
    break;
    case Colour::Orange: line = (rolled ? std::to_string(3) : std::to_string(2)); 
    break;
    case Colour::Yellow: line = (rolled ? std::to_string(0) : std::to_string(3)); 
    break;
    default: break;
  }
  output << line + "\n";
  output.close();

  std::vector<Goal *> goals = this->board->getGoals();
  std::vector<Criterion *> crits = this->board->getCriteria();
  std::vector<Tile *> tiles = this->board->getTiles();

  output.open(saveFilename, std::ios_base::app);
  for (Colour t: this->turnOrder) {
    line = "";
    std::map<Resource, int> res = students.at(t)->getResources();
    line += std::to_string(res.at(Resource::CAFFEINE));
    line += " " + std::to_string(res.at(Resource::LAB));
    line += " " + std::to_string(res.at(Resource::LECTURE));
    line += " " + std::to_string(res.at(Resource::STUDY));
    line += " " + std::to_string(res.at(Resource::TUTORIAL));
    line += " g";
    for (Goal *&g : goals) {
      if (g->getOwner() == t) {
        line += " " + std::to_string(g->getIndex());
      }
    }

    line += " c";
    for (size_t i = 0 ; i < crits.size() ; i++) {
      if (crits.at(i)->getOwner() == t) {
        line += " " + std::to_string(i);
        switch (crits.at(i)->getTier()) {
          case CriterionTier::Assignment: line += " 1"; break;
          case CriterionTier::Midterm: line += " 2"; break;
          case CriterionTier::Exam: line += " 3"; break;
          default: break;
        }
      }
    }
    output << line + "\n";
  }
  line = "";
  for (int i = 0 ; i < tiles.size() ; i++) {
    int resource;
    switch (tiles.at(i)->getResource()) {
      case Resource::CAFFEINE: resource = 0; break;
      case Resource::LAB: resource = 1; break;
      case Resource::LECTURE: resource = 2; break;
      case Resource::STUDY: resource = 3; break;
      case Resource::TUTORIAL: resource = 4; break;
      case Resource::NETFLIX: resource = 5; break;
    }

    line += (i == 0 ? "" : " ") + std::to_string(resource) + " " + 
    std::to_string(tiles.at(i)->getVal());
  }
  output << line + "\n";
  line = "";
  for (size_t i = 0 ; i < tiles.size() ; i++) {
    if (tiles.at(i)->hasGeese()) {
      line = std::to_string(i);
    }
  }

  if (line != "") {
    output << line + "\n";
  }
  output.close();
}
