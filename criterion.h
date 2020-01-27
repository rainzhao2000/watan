#ifndef _CRITERION_H_
#define _CRITERION_H_
#include <vector>
#include "subject.h"
#include "state.h"

class Goal;

class Criterion final : public Subject<CriterionState> {
  Colour completedBy;
  std::vector<Criterion *> neighbourCriteria;
  std::vector<Goal *> neighbourGoals;
  CriterionTier tier;
  int index;
  public:
    //ctor
    Criterion(int index);
    //dtor
    ~Criterion();
    // declare and implement big 5 as needed
    Colour getOwner() const;
    std::vector<Criterion *> getNeighbourCriteria() const;
    std::vector<Goal *> getNeighbourGoals() const;
    CriterionTier getTier() const;
    void setOwner(Colour newOwner);
    void addNeighbour(Criterion *criterion);
    void addNeighbour(Goal *goal);
    void setTier(CriterionTier newTier);
};
#endif
