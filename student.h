#ifndef _STUDENT_H_
#define _STUDENT_H_
#include <map>
#include "subject.h"
#include "state.h"

class WatanInput;
class TextDisplay;
class Watan;
class Criterion;
class Dice;

class Student final : public Subject<StudentState> {
  Watan *watan;
  TextDisplay *td;
  WatanInput *in;
  const Colour colour;
  std::map<Resource, int> resources;
  const std::map<Resource, int> assignmentCost;
  const std::map<Resource, int> midtermCost;
  const std::map<Resource, int> examCost;
  const std::map<Resource, int> goalCost;
  int points;
  Dice *dice;

  // negateResourceMap(rmap, negated): stores the negated quantities of rmap in negated
  // Effects: mutates negated
  void negateResourceMap(const std::map<Resource, int> &rmap, std::map<Resource, int> &negated) const;

  // canAfford(cost): determines if cost can be afforded with this->resources
  // Requires: quantities in cost >= 0
  bool canAfford(const std::map<Resource, int> &cost) const;

  // attemptAchieve(): return successfulness of attempt to achieve goal at input index
  // Effects: may mutate this->resources
  bool attemptAchieve();

  // attemptImprove(complete, initial): return successfulness of attempt to complete/improve criterion at input index, supresses costs for initial
  // Effects: may mutate this->resources
  bool attemptImprove(bool complete, bool initial);

  // moveGeese(): activates geese
  void moveGeese();

  // trade(): initiates trade
  void trade();
  public:
    Student(
      Watan *watan,
      TextDisplay *td,
      WatanInput *in,
      Colour colour,
      const std::map<Resource, int> &resources,
      const std::map<Resource, int> &assignmentCost,
      const std::map<Resource, int> &midtermCost,
      const std::map<Resource, int> &examCost,
      const std::map<Resource, int> &goalCost
    );
    ~Student();
    // declare and implement other big 5 if necessary
    
    Colour getColour() const;
    std::map<Resource, int> getResources() const;
    bool beginTurn(bool initialCompletion);

    // gainResources(amount): adds quantities in amount to this->resources
    // Requires: quantities in amount >= 0
    // Effects: mutates this->resources
    void gainResources(const std::map<Resource, int> &amount);

    // loseResources(amount): deducts quantities in amount from this->resources
    // Requires: quantities in amount >= 0
    // Effects: mutates this->resources
    void loseResources(const std::map<Resource, int> &amount);

    // hasWon(): returns whether student won
    bool hasWon() const;
};
#endif
