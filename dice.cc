#include "dice.h"

Dice::Dice(): Subject<DiceState>{} {}

int Dice::roll() {
  int diceVal = getRollVal();
  this->setState(DiceState{diceVal});
  this->notifyObservers();
  return diceVal;
}
