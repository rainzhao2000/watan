#ifndef _FAIR_DICE_H_
#define _FAIR_DICE_H_
#include "dice.h"

class FairDice final : public Dice {
  int getRollVal() override;
};
#endif
