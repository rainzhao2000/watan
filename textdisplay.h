#ifndef _TEXT_DISPLAY_H_
#define _TEXT_DISPLAY_H_
#include <map>
#include <vector>
#include <string>
#include "gameobservers.h"
#include "state.h"

template <typename StateType> class Subject;

class TextDisplay final : public WatanObserver, public StudentObserver, 
public CriterionObserver, public TileObserver, public GoalObserver,
public DiceObserver {
  Colour curStudent;
  std::vector<std::map<Resource, int>> studentResources; // Change
  std::vector<std::vector<std::vector<int>>> studentCriteria; // For each student in order:
  // Assignment, Midterm, Exam
  std::vector<std::string> boardGoals;
  std::vector<std::string> boardCriteria;
  std::vector<std::string> boardTiles;
  std::vector<std::string> boardProb;
  int geesePos;
  public:
    TextDisplay(std::vector<std::string> initialConditions);
    // declare and implement big 5 if needed
    void notify(Subject<WatanState> &whoFrom) override;
    void notify(Subject<StudentState> &whoFrom) override;
    void notify(Subject<CriterionState> &whoFrom) override;
    void notify(Subject<TileState> &whoFrom) override;
    void notify(Subject<GoalState> &whoFrom) override;
    void notify(Subject<DiceState> &whoFrom) override;
    void displayWinner(const Colour winner) const;
    void displayCurStudent() const; //
    void promptInitialAssignment() const; //
    void displayBoard() const; //
    void displayStatus() const; //
    void displayCriteria() const; //
    void promptLoadedDice() const; //
    void promptTrade(TradeInfo deal) const; //
    void displayGainedResource(const std::map<Resource, int> resources, 
    const Colour student) const; // CHANGED
    void displayNoGainedResources() const; // NEW
    //void displayAchieveAttempt() const; // No need
    //void displayCompleteAttempt() const; // No need
    //void displayImproveAttempt() const; // No need
    void displayInsufficientResources() const; // NEW
    void displayInvalidInput() const; // NEW
    void displayInvalidBuild() const;
    void displayInvalidCommand() const; // NEW
    void promptPlayAgain() const; //
    void displayHelp() const; //
    //void displayGeeseAttempt() const; // No need
    //void promptSteal() const; // No need
    void displayNoStealChoices(Colour attacker) const;
    void displayStealChoices(const std::vector<Colour> students) const; //
    void displayStealResult(const Resource stolen, 
    const Colour student) const; // NEW
    //void displayGeeseSteal(const Colour student, )
    void displayGeeseRoll(const std::vector<Colour> students, 
    const std::vector<std::map<Resource, int>> lost) const;
  private:
    TextDisplay();
    void draw_top(int& c, int& t, int& g) const; // NEW
    void draw_row(int& c, int& t, int& g) const; // NEw
    void draw_bottom(int& c, int& t, int& g) const; // NEw
};
#endif
