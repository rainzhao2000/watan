#include "tile.h"
#include "criterion.h"

Tile::Tile(int value, Resource resource, std::vector<Criterion *> criteria, bool geese):
      value{value}, resource{resource}, criteria{criteria}, geese{geese} {}

Tile::~Tile(){}

int Tile::getVal() const{
	return value;
}

Resource Tile::getResource() const{
	return resource;
}

std::vector<Criterion *> Tile::getCriteria(){
	return criteria;
}

bool Tile::hasGeese() const{
	return geese;
}

void Tile::setGeese(bool overrun){
	geese = overrun;
  this->setState(TileState{value, geese});
  this->notifyObservers();
}

