#ifndef PLAGUE_HPP
#define PLAGUE_HPP

#include <cassert>
#include <vector>

namespace Simulation{

enum class Person { S,I,R };

class World {
  using Grid = std::vector<Person>;
  int side;
  Grid grid;
  double beta;
  double gamma;
  inline static constexpr Person removed_person = Person::R;

public:
  World(int N,double b, double g) : side{N}, grid(N * N, Person::S),beta{b},gamma{g}  {

      assert(beta>=0);
      assert(gamma>=0);
      assert(N>=0);

  }

  Person const &person(int r, int c) const {
    assert(r <= side && c <= side);
    assert(r >= -1 && c >= -1);
    if (r == -1 || c == -1 || r == side || c == side) {
      return removed_person;
    }
    int cursor = r * side + c;
    return grid[cursor];
  }

  Person &person(int r, int c) {
    assert(r < side && c < side);
    assert(r >= 0 && c >= 0);
    int cursor = r * side + c;
    return grid[cursor];
  }
  int size() const { return side; }
  
  
};

inline bool operator==(World const &left, World const &right) {
  bool result;
  for (int i{0}; i < left.size(); ++i) {
    for (int j{0}; j < left.size(); ++j) {
      result = left.person(i, j) == right.person(i, j);
      if (result == false) {
        break;
      }
    }
    if (result == false) {
      break;
    }
  }
  return result;
}

inline bool operator!=(World const &left, World const &right) {
  return !(left == right);
}

}



#endif