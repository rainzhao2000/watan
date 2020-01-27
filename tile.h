#ifndef _TILE_H_
#define _TILE_H_
#include <vector>
#include "subject.h"
#include "state.h"

class Criterion;

class Tile final : public Subject<TileState> {
  int value;
  Resource resource;
  std::vector<Criterion *> criteria;
  bool geese;
  public:
    // ctor
    Tile(int value, Resource resource, std::vector<Criterion *> criteria, bool geese = false);
    //dtor
    ~Tile();
    // declare and implement big 5 as needed
    int getVal() const;
    Resource getResource() const;
    std::vector<Criterion *> getCriteria();
    bool hasGeese() const;
    void setGeese(bool overrun);
};
#endif
