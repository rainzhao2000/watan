#ifndef _LOADED_DICE_H_
#define _LOADED_DICE_H_
#include "dice.h"

class WatanInput;
class TextDisplay;

class LoadedDice final : public Dice {
  WatanInput *in;
  TextDisplay *td;

  int getRollVal() override;
  public:
    LoadedDice(WatanInput *in, TextDisplay *td);
};
#endif
