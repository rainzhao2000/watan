#ifndef _SUBJECT_H_
#define _SUBJECT_H_
#include <vector>
#include "observer.h"

template <typename StateType> class Subject {
  StateType state;
  std::vector<Observer<StateType>*> observers;
  protected:
    void setState(StateType newS);
  public:
    Subject() = default;
    void attach(Observer<StateType> *o);
    void detach(Observer<StateType> *o);
    void notifyObservers();
    StateType getState() const;
    virtual ~Subject() = 0;
};

template <typename StateType>
Subject<StateType>::~Subject() {}

template <typename StateType>
void Subject<StateType>::setState(StateType newS) {
  this->state = newS;
}

template <typename StateType>
void Subject<StateType>::attach(Observer<StateType> *o) {
  this->observers.emplace_back(o);
}

template <typename StateType>
void Subject<StateType>::detach(Observer<StateType> *o) {
  for (auto it = this->observers.begin(); it != this->observers.end(); ++it) {
    if (it == o) {
      this->observers.erase(o);
    }
  }
}

template <typename StateType>
void Subject<StateType>::notifyObservers() {
  for (auto o : this->observers) {
    o->notify(*this);
  }
}

template <typename StateType>
StateType Subject<StateType>::getState() const {
  return this->state;
}
#endif
