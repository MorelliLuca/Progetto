#include "plague.hpp"
#include <cassert>
#include <random>
#include <vector>

namespace Simulation {

std::mt19937 gen;
std::uniform_real_distribution<> dis(0, 1);

inline int I_near(World const& world, int r, int c)
{
  int result = 0;
  for (int i : {-1, 0, 1}) {
    for (int j : {-1, 0, 1}) {
      if (world.person(r + i, c + j) == Person::I) { ++result; }
    }
  }
  return result;
}

inline Person check_next_status(World const& world, int r, int c)
{
  Person const& person{world.person(r, c)};
  double beta{world.get_beta()};
  double gamma{world.get_gamma()};
  int const near_I{I_near(world, r, c)};
  switch (person) {
    case Person::S:
      if (near_I!=0&&dis(gen) <= (beta + (beta/8.) * near_I)) { return Person::I; }
      break;
    case Person::I:
      if (dis(gen) <= gamma) { return Person::R; }
      break;
    case Person::R:
      return Person::R;
  }
  return person;
}
World evolve(World const& current_world)
{
  int const N = current_world.get_side();
  World next{N, current_world.get_beta(), current_world.get_gamma()};
  for (int i = 0; i != N; ++i) {
    for (int j = 0; j != N; ++j) { next.person(i, j) = check_next_status(current_world, i, j); }
  }
  return next;
}

}  // namespace Simulation