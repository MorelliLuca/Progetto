#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "plague.hpp"

TEST_CASE("Testing grid")
{
  Simulation::World world {2, 0.5, 0.05};
  CHECK (world.person(0,0)==Simulation::Person::S);
  CHECK (world.person(0,1)==Simulation::Person::S);
  CHECK (world.person(1,0)==Simulation::Person::S);
  CHECK (world.person(1,1)==Simulation::Person::S);
  Simulation::World next {Simulation::evolve(world)};
  CHECK (next==world);
}

TEST_CASE("Testing beta")
{
  Simulation::World world {3, 0.5, 0.05};
  world.person(0,1)=Simulation::Person::I;
  world.person(0,0)=Simulation::Person::I;
  world.person(0,2)=Simulation::Person::I;
  world.person(1,2)=Simulation::Person::I;
  world.person(1,0)=Simulation::Person::I;
  world.person(2,0)=Simulation::Person::I;
  world.person(2,1)=Simulation::Person::I;
  world.person(2,2)=Simulation::Person::I;
  Simulation::World next {Simulation::evolve(world)};
  CHECK (next.person(1,1)==Simulation::Person::I);
}

TEST_CASE("Testing gamma")
{
  Simulation::World world {1, 0.5, 1};
  world.person(0,0)=Simulation::Person::I;
  Simulation::World next {Simulation::evolve(world)};
  CHECK (next.person(0,0)==Simulation::Person::R);
}