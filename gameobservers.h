#ifndef _GAME_OBSERVERS_H_
#define _GAME_OBSERVERS_H_
#include "observer.h"
#include "state.h"

template <typename StateType> class Subject;

class WatanObserver : public Observer<WatanState> {
  public:
    virtual void notify(Subject<WatanState> &whoFrom) = 0;
    virtual ~WatanObserver() = default;
};

class StudentObserver : public Observer<StudentState> {
  public:
    virtual void notify(Subject<StudentState> &whoFrom) = 0;
    virtual ~StudentObserver() = default;
};

class CriterionObserver : public Observer<CriterionState> {
  public:
    virtual void notify(Subject<CriterionState> &whoFrom) = 0;
    virtual ~CriterionObserver() = default;
};

class TileObserver : public Observer<TileState> {
  public:
    virtual void notify(Subject<TileState> &whoFrom) = 0;
    virtual ~TileObserver() = default;
};

class GoalObserver : public Observer<GoalState> {
  public:
    virtual void notify(Subject<GoalState> &whoFrom) = 0;
    virtual ~GoalObserver() = default;
};

class DiceObserver : public Observer<DiceState> {
  public:
  virtual void notify(Subject<DiceState> &whofrom) = 0;
  virtual ~DiceObserver() = default;
};
#endif
