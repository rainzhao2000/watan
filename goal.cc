#include "goal.h"

Goal::Goal(int index): index{index}{
  achievedBy = Colour::NoColour;
}

Goal::~Goal() {}

int Goal::getIndex() const{
	return index;
}

Colour Goal::getOwner() const{
	return achievedBy;
}

void Goal::setOwner(Colour newOwner){
	achievedBy = newOwner;
  this->setState(GoalState{index, achievedBy});
  this->notifyObservers();
}
