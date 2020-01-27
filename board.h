#ifndef _BOARD_H_
#define _BOARD_H_
#include <vector>
#include "tile.h"
#include "criterion.h"
#include "goal.h"
#include "state.h"

class Board final {
  std::vector<Tile *> tiles;
  std::vector<Criterion *> criteria;
  std::vector<Goal *> goals;
  public:
    Board(std::vector<Tile *> tiles, std::vector<Criterion *> criteria, std::vector<Goal *> goals);
    ~Board();
    // declare and implement big 5 as needed 
    Tile *placeGeese(size_t tileIndex);
    bool achieve(Colour owner, int goalIndex);
    bool improve(Colour owner, int criterionIndex, bool initial);
    CriterionTier getCriterionTier(int criterionIndex) const;
    std::vector<Tile *> getTiles() const;
    std::vector<Criterion *> getCriteria() const;
    std::vector<Goal *> getGoals() const;
};

#endif
