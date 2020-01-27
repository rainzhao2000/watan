#ifndef _GOAL_H_
#define _GOAL_H_
#include "subject.h"
#include "state.h"

class Goal final : public Subject<GoalState> {
  int index;
  Colour achievedBy;
  public:
    Goal(int index);
    ~Goal();
    // declare and implement big 5 as needed
    int getIndex() const;
    Colour getOwner() const;
    void setOwner(Colour newOwner);
};
#endif
