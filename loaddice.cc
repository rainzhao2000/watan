#include "loaddice.h"
#include "wataninput.h"
#include "textdisplay.h"

LoadedDice::LoadedDice(WatanInput *in, TextDisplay *td): in{in}, td{td} {}

int LoadedDice::getRollVal() {
  if(this->in && this->td) {
    while(true) {
      this->td->promptLoadedDice();
      int diceVal = this->in->getLoadedDice(2, 12);
      if(2 <= diceVal && diceVal <= 12) {
        return diceVal;
      }
      this->td->displayInvalidInput();
    }
  }
  throw "no i/o available";
}
