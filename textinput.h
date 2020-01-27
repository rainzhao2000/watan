#ifndef _TEXTINPUT_H_
#define _TEXTINPUT_H_

#include "wataninput.h"

class TextInput final : public WatanInput {
  public:
    ~TextInput() = default;
    Command getCommand() override;
    int getLoadedDice(const int min, const int max) override;
    int getCriterionPlacement(const int min, const int max) override;
    int getGoalPlacement(const int min, const int max) override;
    int getTile(const int min, const int max) override;
    TradeInfo getTrade() override;
    Response getResponse() override;
    std::string getSaveFile() override;
    Colour getStealChoice(const std::vector<Colour> choices) override;
};
#endif
