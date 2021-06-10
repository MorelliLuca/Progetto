#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "plague.hpp"

TEST_CASE("Testing grid")
{
  Simulation::World world{2, 0.5, 0.05,0};
  CHECK(world.person(0, 0) == Simulation::Person::S);
  CHECK(world.person(0, 1) == Simulation::Person::S);
  CHECK(world.person(1, 0) == Simulation::Person::S);
  CHECK(world.person(1, 1) == Simulation::Person::S);
  Simulation::World next{Simulation::evolve(world)};
  CHECK(next == world);
}

TEST_CASE("Testing beta")
{
  Simulation::World world{3, 0.5, 0.05,0};
  world.person(0, 1) = Simulation::Person::I;
  world.person(0, 0) = Simulation::Person::I;
  world.person(0, 2) = Simulation::Person::I;
  world.person(1, 2) = Simulation::Person::I;
  world.person(1, 0) = Simulation::Person::I;
  world.person(2, 0) = Simulation::Person::I;
  world.person(2, 1) = Simulation::Person::I;
  world.person(2, 2) = Simulation::Person::I;
  Simulation::World next{Simulation::evolve(world)};
  CHECK(next.person(1, 1) == Simulation::Person::I);
}

TEST_CASE("Testing gamma")
{
  Simulation::World world{1, 0.5, 1,0};
  world.person(0, 0) = Simulation::Person::I;
  Simulation::World next{Simulation::evolve(world)};
  CHECK(next.person(0, 0) == Simulation::Person::R);
}


TEST_CASE("Testing theta")
{
  Simulation::World world{1, 0, 1,1};
  world.start_vax();
  Simulation::World next{Simulation::evolve(world)};
  CHECK(next.person(0, 0) == Simulation::Person::V);
}

TEST_CASE("Testing get_S/I/R/E function")
{
  Simulation::World world{3, 0.5, 0.05,0};
  world.person(0, 1) = Simulation::Person::I;
  world.person(0, 0) = Simulation::Person::I;
  world.person(0, 2) = Simulation::Person::I;
  world.person(1, 2) = Simulation::Person::E;
  world.person(1, 0) = Simulation::Person::R;
  world.person(2, 0) = Simulation::Person::R;
  CHECK(world.get_E() == 1);
  CHECK(world.get_I() == 3);
  CHECK(world.get_R() == 2);
  CHECK(world.get_S() == 3);
}

TEST_CASE("Testing find_E")
{
  Simulation::World world{3, 0.5, 0.05,0};
  world.person(1, 1) = Simulation::Person::E;
  world.person(2, 0) = Simulation::Person::E;
  std::vector<int> E_coord{world.find_E()};
  CHECK(E_coord[0] == 1);
  CHECK(E_coord[1] == 1);
  CHECK(E_coord[2] == 2);
  CHECK(E_coord[3] == 0);
}

TEST_CASE("Testing swap")
{
  Simulation::World world{3, 0.5, 0.05,0};
  world.person(1, 1) = Simulation::Person::I;
  world.person(2, 0) = Simulation::Person::R;
  Simulation::swap(world, 1, 1, 2, 0);
  CHECK(world.person(1, 1) == Simulation::Person::R);
  CHECK(world.person(2, 0) == Simulation::Person::I);
}


TEST_CASE("Testing Mask Vax and Lockdown")
{
 Simulation::World world{3, 0.5, 0.05,0};
 CHECK(world.mask_status()==Simulation::Mask::OFF); 
 CHECK(world.lockdown_status()==Simulation::Lockdown::OFF); 
 CHECK(world.vax_status()==Simulation::Vax::OFF); 
 world.change_lockdown();
 world.change_mask();
 world.start_vax();
 CHECK(world.mask_status()==Simulation::Mask::ON); 
 CHECK(world.lockdown_status()==Simulation::Lockdown::ON); 
 CHECK(world.vax_status()==Simulation::Vax::ON); 
 world.change_lockdown();
 world.change_mask();
 world.start_vax();
 CHECK(world.mask_status()==Simulation::Mask::OFF); 
 CHECK(world.lockdown_status()==Simulation::Lockdown::OFF); 
 CHECK(world.vax_status()==Simulation::Vax::ON); 
}