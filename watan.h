#ifndef _WATAN_H_
#define _WATAN_H_
#include <vector>
#include <map>
#include <string>
#include "subject.h"
#include "state.h"
#include "textdisplay.h"
#include "wataninput.h"
#include "student.h"
#include "board.h"

class Watan final : public Subject<WatanState> {
  TextDisplay *td = nullptr;
  WatanInput *in = nullptr;
  int firstTurn;
  std::vector<Colour> turnOrder;
  std::map<Colour, Student *> students;
  int gameSeed;
  Board *board = nullptr;
  bool loaded;

  void loadResources(std::map<Resource, int> &rmap, std::stringstream &data);
  void loadGoals(Colour student, std::vector<Goal *> goals, std::stringstream &data);
  void loadCriteria(Colour student, std::vector<Criterion *> criteria, std::stringstream &data);
  std::string randomBoardString() const;
  Colour whoWon() const;
  public:
    Watan(unsigned int seed = 1);
    Watan(const std::string &loadFilename, unsigned int seed = 1);
    ~Watan();
    void init(const std::vector<std::string> &initialConditions);
    bool start();
    void loseToGeese() const;
    void steal(Colour robber, Colour target) const;
    void distributeResources(int rolledNum) const;
    void giveTo(Colour student, const std::map<Resource, int> &amount) const;
    void takeFrom(Colour student, const std::map<Resource, int> &amount) const;
    std::vector<Colour> attemptPlaceGeese(Colour attacker, int tileIndex) const;
    bool attemptAchieve(Colour owner, int goalIndex) const;
    bool attemptImprove(Colour owner, int criterionIndex, bool initial) const;
    CriterionTier getCriterionTier(int criterionIndex) const;
    void save(const std::string &saveFilename, const bool rolled, const Colour curStudent) const;
    void save(const std::string &&saveFilename, const bool rolled, const Colour curStudent) const;
};
#endif
