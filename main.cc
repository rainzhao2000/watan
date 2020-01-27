#include <iostream>
#include "watan.h"
#include <chrono>

int main(int argc, char *argv[]) {
  Watan* game;
  
  if (argc == 3) {
    std::string str = argv[1];
    if (str == "-seed") {
      std::string seed = argv[2];
      game = new Watan(std::stoi(seed));
    } else if(str == "-load" || str == "-board") {
      std::string load = argv[2];
      game = new Watan(load);
    }
  } else if (argc > 1) {
    std::string str = argv[1];
    if (argv[1] == "-seed") {
      std::string seed = argv[2], load = argv[4];
      game = new Watan(argv[4], std::stoi(argv[2]));
    } else {
      std::string seed = argv[4], load = argv[2];
      game = new Watan(load, std::stoi(seed));
    }
  } else {
    game = new Watan(std::chrono::system_clock::now().time_since_epoch().count());
  }
  game->start();
  /*
  while(true) {
    bool replay = game->start();
    if(replay){
      game->init(SOME_PARAMETER);
    }else{
      break;
    }
  }*/
  //TODO
  delete game;
  return 0;
}
