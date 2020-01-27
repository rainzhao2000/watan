#ifndef _WATAN_INPUT_H_
#define _WATAN_INPUT_H_
#include <string>
#include <vector>

enum class Command { DiceLoad, DiceFair, Roll, Board, Status, Criteria,
 Achieve, Complete, Improve, Trade, Next, Save, Help, Quit};

enum class Response { Yes, No };

enum class Colour;

struct TradeInfo;

class WatanInput {
  public:
    virtual ~WatanInput() = 0;
    virtual Command getCommand() = 0;
    virtual int getLoadedDice(const int min, const int max) = 0;
    virtual int getCriterionPlacement(const int min, const int max) = 0;
    virtual int getGoalPlacement(const int min, const int max) = 0;
    virtual int getTile(const int min, const int max) = 0;
    virtual TradeInfo getTrade() = 0;
    virtual Response getResponse() = 0;
    virtual std::string getSaveFile() = 0;
    virtual Colour getStealChoice(const std::vector<Colour> choices) = 0;
};
#endif
