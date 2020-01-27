#include "criterion.h"
#include "goal.h"

Criterion::Criterion(int index) {
  this->index = index;
  tier = CriterionTier::Default;
  completedBy = Colour::NoColour;
}

Criterion::~Criterion(){}

Colour Criterion::getOwner() const{
	return completedBy;
}

std::vector<Criterion *> Criterion::getNeighbourCriteria() const{
	return neighbourCriteria;
}

std::vector<Goal *> Criterion::getNeighbourGoals() const{
	return neighbourGoals;
}

CriterionTier Criterion::getTier() const{
	return tier;
}

void Criterion::setOwner(Colour newOwner){
	completedBy = newOwner;
  setTier(CriterionTier::Assignment);
}

void Criterion::addNeighbour(Criterion *criterion) {
  this->neighbourCriteria.emplace_back(criterion);
}
void Criterion::addNeighbour(Goal *goal) {
  this->neighbourGoals.emplace_back(goal);
}

void Criterion::setTier(CriterionTier newTier){
	tier = newTier;
  this->setState(CriterionState{completedBy, index, newTier});
  this->notifyObservers();
}
