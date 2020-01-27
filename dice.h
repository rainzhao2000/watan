#ifndef _DICE_H_
#define _DICE_H_
#include "subject.h"
#include "state.h"

class Dice : public Subject<DiceState> {
  virtual int getRollVal() = 0;
  public:
    Dice();
    virtual ~Dice() = default;
    int roll();
};
#endif
