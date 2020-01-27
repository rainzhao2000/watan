#ifndef _STATE_H_
#define _STATE_H_
#include <map>
#include <vector>
#include <iostream>
#include <string>

enum class Resource {
  CAFFEINE = 0,
  LAB = 1,
  LECTURE = 2,
  STUDY = 3,
  TUTORIAL = 4,
  NETFLIX = 5
};

enum class Colour {
  NoColour = -1,
  Blue = 0,
  Red = 1,
  Orange = 2,
  Yellow = 3
};

// for printing Resources
std::ostream &operator<<(std::ostream &out, const Resource &r);

// working with strings and Resources
std::string operator+(const std::string &lhs, const Resource &rhs);

// working with Resources and strings
std::string operator+(const Resource &lhs, const std::string &rhs);

// so we can print Colours
std::ostream &operator<<(std::ostream &out, const Colour &c);

// working with strings and Colours
std::string operator+(const std::string &lhs, const Colour &rhs);

// working with Colours and strings
std::string operator+(const Colour &lhs, const std::string &rhs);

enum class CriterionTier { Default = 0, Assignment = 1, Midterm = 2, Exam = 3 };

struct TradeInfo {
  Colour target;
  Resource offer;
  Resource want;
  TradeInfo(
    Colour target = Colour::NoColour,
    Resource offer = Resource::CAFFEINE,
    Resource want = Resource::CAFFEINE
  ): target{target}, offer{offer}, want{want} {}
};

struct Improvement {
  int index;
  CriterionTier oldTier;
  CriterionTier newTier;
  Improvement(
    int index = -1,
    CriterionTier oldTier = CriterionTier::Default,
    CriterionTier newTier = CriterionTier::Default
  ): index{index}, oldTier{oldTier}, newTier{newTier} {}
};

struct State {
  State() = default;
  virtual ~State() = default;
};

struct WatanState : public State {
  Colour curTurn;
  WatanState() = default;
  WatanState(Colour curTurn): curTurn{curTurn} {}
};

struct StudentState : public State {
  Colour appliedTo;
  std::map<Resource, int> resourceChange;
  StudentState() = default;
  StudentState(Colour appliedTo, std::map<Resource, int> resourceChange):
    appliedTo{appliedTo}, resourceChange{resourceChange} {}
};

struct DiceState : public State {
  int diceVal;
  DiceState() = default;
  DiceState(int diceVal): diceVal{diceVal} {}
};

struct CriterionState : public State {
  Colour newOwner;
  int index; // New
  CriterionTier newTier;
  CriterionState() = default;
  CriterionState(Colour newOwner, int index, CriterionTier newTier):
    newOwner{newOwner}, index{index}, newTier{newTier} {}
};

struct TileState : public State {
  int index; //Added index. Why value
  bool hasGeese;
  TileState() = default;
  TileState(int index, bool hasGeese): index{index}, hasGeese{hasGeese} {}
};

struct GoalState : public State {
  int index;
  Colour newOwner;
  GoalState() = default;
  GoalState(int index, Colour newOwner): index{index}, newOwner{newOwner} {}
};
#endif
