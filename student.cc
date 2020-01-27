#include "student.h"
#include "watan.h"
#include "wataninput.h"
#include "textdisplay.h"
#include "fairdice.h"
#include "loaddice.h"

Student::Student(
  Watan *watan,
  TextDisplay *td,
  WatanInput *in,
  Colour colour,
  const std::map<Resource, int> &resources,
  const std::map<Resource, int> &assignmentCost,
  const std::map<Resource, int> &midtermCost,
  const std::map<Resource, int> &examCost,
  const std::map<Resource, int> &goalCost
): watan{watan},
  td{td},
  in{in},
  colour{colour},
  resources{resources},
  assignmentCost{assignmentCost},
  midtermCost{midtermCost},
  examCost{examCost},
  goalCost{goalCost},
  dice{new FairDice{}} {
    dice->attach(td);
    points = 0;
  }

Student::~Student() {
  delete this->dice;
}

void Student::negateResourceMap(
  const std::map<Resource, int> &rmap,
  std::map<Resource, int> &negated
) const {
  for(std::pair<Resource, int> pair : rmap) {
    negated.emplace(pair.first, -1 * pair.second);
  }
}

bool Student::canAfford(const std::map<Resource, int> &cost) const {
  for(std::pair<Resource, int> pair : cost) {
    try { // find a resource we cant afford
      if(this->resources.at(pair.first) - pair.second < 0) return false;
    } catch(std::out_of_range) { // or if the resource is undefined
      if(pair.second) {
        return false;
      }
    }
  }
  return true;
}

bool Student::attemptAchieve() {
  if(!this->watan) throw "no game instance available";
  if(!this->td || !this->in) throw "no i/o available";

  if(!this->canAfford(this->goalCost)) {
    this->td->displayInsufficientResources();
    return false;
  }

  int goalIndex = this->in->getGoalPlacement(0, 71);
  if(!this->watan->attemptAchieve(this->colour, goalIndex)) {
    this->td->displayInvalidBuild();
    return false;
  }

  std::map<Resource, int> resourceChange;
  negateResourceMap(this->goalCost, resourceChange);
  this->loseResources(this->goalCost);
  this->td->displayBoard();
  return true;
}

bool Student::attemptImprove(bool complete, bool initial) {
  if(!this->watan) throw "no game instance available";
  if(!this->td || !this->in) throw "no i/o available";

  // determine cost
  const std::map<Resource, int> *cost = nullptr;
  int criterionIndex = this->in->getCriterionPlacement(0, 53);
  CriterionTier oldTier = this->watan->getCriterionTier(criterionIndex);
  if(complete && !initial) { // completing
    switch(oldTier) {
      case CriterionTier::Default: cost = &(this->assignmentCost); break;
      default:
        this->td->displayInvalidBuild();
        return false;
    }
  } else if(!complete && !initial) { // improving
    switch(oldTier) {
      case CriterionTier::Assignment: cost = &(this->midtermCost); break;
      case CriterionTier::Midterm: cost = &(this->examCost); break;
      default:
        this->td->displayInvalidBuild();
        return false;
    }
  }

  if(!initial && !this->canAfford(*cost)) {
    this->td->displayInsufficientResources();
    return false;
  }

  if(!this->watan->attemptImprove(this->colour, criterionIndex, initial)) {
    this->td->displayInvalidBuild();
    return false;
  }

  if(!initial) {
    this->loseResources(*cost);
    this->td->displayBoard();
  }
  points++;
  return true;
}

void Student::moveGeese() {
  this->watan->loseToGeese();
  while(true) {
    int tileIndex = this->in->getTile(0, 18);
    std::vector<Colour> stealChoices = this->watan->attemptPlaceGeese(this->colour, tileIndex);
    if(stealChoices.size()) { // valid tileIndex
      if(stealChoices.at(0) == Colour::NoColour) { // no students to steal from
        this->td->displayNoStealChoices(this->colour);
        return;
      }
      this->td->displayStealChoices(stealChoices);
      // get input colour to steal from
      Colour target = this->in->getStealChoice(stealChoices);
      this->watan->steal(this->colour, target);
      return;
    }
  }
}

void Student::trade() {
  if(!this->watan) throw "no game instance available";
  if(!this->td || !this->in) throw "no i/o available";

  TradeInfo deal = this->in->getTrade();
  if(!this->canAfford({{deal.offer, 1}})) {
    this->td->displayInsufficientResources();
    return;
  }

  this->td->promptTrade(deal);
  if(this->in->getResponse() == Response::Yes) {
    this->loseResources({{deal.offer, 1}});
    this->gainResources({{deal.want, 1}});
    this->watan->takeFrom(deal.target, {{deal.want, 1}});
    this->watan->giveTo(deal.target, {{deal.offer, 1}});
  }
}

Colour Student::getColour() const {
  return this->colour;
}

std::map<Resource, int> Student::getResources() const {
  return this->resources;
}

bool Student::beginTurn(bool initialCompletion) {
  if(!this->watan) throw "no game instance available";
  if(!this->td || !this->in) throw "no i/o available";

  this->td->displayBoard();

  if(initialCompletion) {
    while(true) {
      this->td->promptInitialAssignment();
      Command c = this->in->getCommand();
      if (c == Command::Complete) {
        if(this->attemptImprove(true, true)) {
          return false;
        }
      } else if (c == Command::Quit) {
        return true;
      }
    }
  } else {
    this->td->displayCurStudent();
    bool rolled = false;
    while(true) {
      Command c = this->in->getCommand();
      switch(c) {
        case Command::DiceLoad: {
          Dice *temp = this->dice;
          this->dice = new LoadedDice{this->in, this->td};
          this->dice->attach(this->td);
          delete temp;
          break;
        }
        case Command::DiceFair: {
          Dice *temp = this->dice;
          this->dice = new FairDice{};
          this->dice->attach(this->td);
          delete temp;
          break;
        }
        case Command::Roll: {
          if(rolled) break;
          int diceVal = this->dice->roll();
          if(diceVal == 7) {
            this->moveGeese();
          } else {
            this->watan->distributeResources(diceVal);
          }
          rolled = true;
          break;
        }
        case Command::Board: this->td->displayBoard(); break;
        case Command::Status: this->td->displayStatus(); break;
        case Command::Criteria: this->td->displayCriteria(); break;
        case Command::Achieve: this->attemptAchieve(); break;
        case Command::Complete: this->attemptImprove(true, false); break;
        case Command::Improve: this->attemptImprove(false, false); break;
        case Command::Trade: this->trade(); break;
        case Command::Next: if (rolled) {return false;}; break;
        case Command::Save:
          this->watan->save(this->in->getSaveFile(), rolled, this->colour);
          break;
        case Command::Help: this->td->displayHelp(); break;
        case Command::Quit: return true;
      }
    }
  }
}

void Student::gainResources(const std::map<Resource, int> &amount) {
  for(std::pair<Resource, int> pair : amount) {
    try {
      this->resources.at(pair.first) += pair.second;
    } catch(std::out_of_range) {
      this->resources.emplace(pair);
    }
  }
  this->setState(StudentState{this->colour, amount});
  this->notifyObservers();
}

void Student::loseResources(const std::map<Resource, int> &amount) {
  for(std::pair<Resource, int> pair : amount) {
    try {
      this->resources.at(pair.first) -= pair.second;
    } catch(std::out_of_range) {
      if(pair.second) {
        throw std::out_of_range("err: " + this->colour + "failed to pay " + pair.first + "; DNE");
      }
    }
  }
  std::map<Resource, int> loss;
  negateResourceMap(amount, loss);
  this->setState(StudentState{this->colour, loss});
  this->notifyObservers();
}

bool Student::hasWon() const {
  return this->points == 10;
}
