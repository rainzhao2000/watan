#include "state.h"

// for printing Resources
std::ostream &operator<<(std::ostream &out, const Resource &r) {
  switch(r) {
    case Resource::CAFFEINE: out << "CAFFEINE"; break;
    case Resource::LAB: out << "LAB"; break;
    case Resource::LECTURE: out << "LECTURE"; break;
    case Resource::STUDY: out << "STUDY"; break;
    case Resource::TUTORIAL: out << "TUTORIAL"; break;
    case Resource::NETFLIX: out << "NETFLIX"; break;
  }
  return out;
}

// working with strings and Resources
std::string operator+(const std::string &lhs, const Resource &rhs) {
  switch(rhs) {
    case Resource::CAFFEINE: return lhs + "CAFFEINE";
    case Resource::LAB: return lhs + "LAB";
    case Resource::LECTURE: return lhs + "LECTURE";
    case Resource::STUDY: return lhs + "STUDY";
    case Resource::TUTORIAL: return lhs + "TUTORIAL";
    case Resource::NETFLIX: return lhs + "NETFLIX";
    default: return lhs;
  }
}

// working with Resources and strings
std::string operator+(const Resource &lhs, const std::string &rhs) {
  switch(lhs) {
    case Resource::CAFFEINE: return "CAFFEINE" + rhs;
    case Resource::LAB: return "LAB" + rhs;
    case Resource::LECTURE: return "LECTURE" + rhs;
    case Resource::STUDY: return "STUDY" + rhs;
    case Resource::TUTORIAL: return "TUTORIAL" + rhs;
    case Resource::NETFLIX: return "NETFLIX" + rhs;
    default: return rhs;
  }
}

// so we can print Colours
std::ostream &operator<<(std::ostream &out, const Colour &c) {
  switch(c) {
    case Colour::NoColour: out << "NoColour"; break;
    case Colour::Blue: out << "Blue"; break;
    case Colour::Red: out << "Red"; break;
    case Colour::Orange: out << "Orange"; break;
    case Colour::Yellow: out << "Yellow"; break;
  }
  return out;
}

// working with strings and Colours
std::string operator+(const std::string &lhs, const Colour &rhs) {
  switch(rhs) {
    case Colour::NoColour: return lhs + "NoColour";
    case Colour::Blue: return lhs + "Blue";
    case Colour::Red: return lhs + "Red";
    case Colour::Orange: return lhs + "Orange";
    case Colour::Yellow: return lhs + "Yellow";
    default: return lhs;
  }
}

// working with Colours and strings
std::string operator+(const Colour &lhs, const std::string &rhs) {
  switch(lhs) {
    case Colour::NoColour: return "NoColour" + rhs;
    case Colour::Blue: return "Blue" + rhs;
    case Colour::Red: return "Red" + rhs;
    case Colour::Orange: return "Orange" + rhs;
    case Colour::Yellow: return "Yellow" + rhs;
    default: return rhs;
  }
}
