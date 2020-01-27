#include "board.h"

Board::Board(std::vector<Tile *> tiles, std::vector<Criterion *> criteria, std::vector<Goal *> goals):
  tiles{tiles}, criteria{criteria}, goals{goals} {}

Board::~Board(){
  for(auto a:criteria){
    delete a;
  }

  for(auto a:tiles){
    delete a;
  }

  for(auto a:goals){
    delete a;
  }

  criteria.clear();
  tiles.clear();
  goals.clear();
}


std::vector<Tile *> Board::getTiles() const{
	return tiles;
}

std::vector<Criterion *> Board::getCriteria() const{
  return criteria;
}

std::vector<Goal *> Board::getGoals() const{
  return goals;
}

Tile *Board::placeGeese(size_t tileIndex) {
  if(tileIndex < tiles.size()){
    //check if dup geese location
    if(tiles.at(tileIndex)->hasGeese()){
      //setting on the same tile
      //failure
      return nullptr;
    }

    //cancel old geese
    for(auto t : tiles){
      if(t->hasGeese()){
        t->setGeese(false);
        break;
      }
    }

    //set new geese
    tiles.at(tileIndex)->setGeese(true);
    return tiles.at(tileIndex);
  }else{
    return nullptr;
  }
}


bool Board::achieve(Colour owner, int goalIndex){
  if(goals.at(goalIndex)->getOwner() != Colour::NoColour){
    return false;
  }

	for(auto c : criteria){
    for(auto g : c->getNeighbourGoals()){
      if(g->getIndex() == goalIndex){
        
        //this is target criteria to check for
        if(c->getOwner() == owner){

          //owner owns adjacent criterion, can achieve
          goals.at(goalIndex)->setOwner(owner);
          return true;
        }else{
          
          //check of other goals are achieved by "me"
          for(auto goal : c->getNeighbourGoals()){
            if(goal->getIndex() != goalIndex && goal->getOwner() == owner){
             
              //owner owns adjacent goal, can achieve
              goals.at(goalIndex)->setOwner(owner);
              return true;
            }
          }
        }
      }
	  }
  }

  return false;
}


bool Board::improve(Colour owner, int criterionIndex, bool initial){
  //check if board allows to complete, dont care about resources


  if(criteria.at(criterionIndex)->getOwner() == Colour::NoColour){
    //trying to complete this criteria
    //if I own any adjacent goals, then yes
    //else no
    if(initial){
      bool nearby = false;
      for(auto c : criteria.at(criterionIndex)->getNeighbourCriteria()){
        if(c->getOwner() != Colour::NoColour){
          nearby = true;
          break;
        }
      }
      if(nearby){
        //cant build here
        return false;
      }else{
        //yes
        criteria.at(criterionIndex)->setOwner(owner);
      
        return true;
      }
    }else{
      bool nearby = false;
      for(auto c : criteria.at(criterionIndex)->getNeighbourCriteria()){
        if(c->getOwner() != Colour::NoColour){
          nearby = true;
          break;
        }
      }
      if(nearby){
        //cant build here
        return false;
      }
      for(auto g : criteria.at(criterionIndex)->getNeighbourGoals()){
        if(g->getOwner() == owner){
          //yes
          criteria.at(criterionIndex)->setOwner(owner);
     
          return true;
        }
      }
    }
    return false;
  }else if(criteria.at(criterionIndex)->getOwner() == owner){
    //trying to improve this criteria that I own
    //resource check done in student before calling this method
    //check if already at max level
    if(criteria.at(criterionIndex)->getTier() == CriterionTier::Exam){
      return false;
    }else if(criteria.at(criterionIndex)->getTier() == CriterionTier::Midterm){
      criteria.at(criterionIndex)->setTier(CriterionTier::Exam);
      return true;
    }else if(criteria.at(criterionIndex)->getTier() == CriterionTier::Assignment){
      criteria.at(criterionIndex)->setTier(CriterionTier::Midterm);
      return true;
    }
  }

  //its owned by someone else, cant do nothing, return false
  return false;
}




