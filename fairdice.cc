#include <random>
#include <chrono>
#include "fairdice.h"

int FairDice::getRollVal() {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_int_distribution<int> distribution(2, 12);
  return distribution(generator);
}
