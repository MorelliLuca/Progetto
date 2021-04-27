#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "sir.hpp"

TEST_CASE("Testing Struct Data")
{
  Simulation::Data data;
  CHECK(data.S == 0);
  CHECK(data.I == 0);
  CHECK(data.R == 0);
  Simulation::Data data_1{1, 2, 3};
  CHECK(data_1.S == 1);
  CHECK(data_1.I == 2);
  CHECK(data_1.R == 3);
}

TEST_CASE("Testing Class Population")
{
  Simulation::Data data{1, 2, 3};
  Simulation::Population population{0.1, .2, data};
  CHECK(population.I() == 2);
  CHECK(population.S() == 1);
  CHECK(population.R() == 3);
  CHECK(population.Total() == 6);
  CHECK(population.Beta() == .1);
  CHECK(population.Gamma() == .2);
}

TEST_CASE("Testing evolve function")
{
  SUBCASE("Beta>Gamma"){
  Simulation::Data data{7000, 8000, 1000};
  Simulation::Population population{0.1, .01, data};
  Simulation::Population next = Simulation::Evolve(population);
  CHECK(next.Total() == 16000);
  CHECK(next.Beta() == .1);
  CHECK(next.Gamma() == .01);
  CHECK(next.S()==6650);
  CHECK(next.I()==8270);
  CHECK(next.R()==1080);
  }
  SUBCASE("Beta<Gamma"){
  Simulation::Data data{7000, 8000, 1000};
  Simulation::Population population{.03, .3, data};
  Simulation::Population next = Simulation::Evolve(population);
  CHECK(next.Total() == 16000);
  CHECK(next.Beta() == .03);
  CHECK(next.Gamma() == .3);
  CHECK(next.S()==6895);
  CHECK(next.I()==5705);
  CHECK(next.R()==3400);
  }
  SUBCASE("N~S && Beta>Gamma"){
    Simulation::Data data{7000, 10, 0};
  Simulation::Population population{.3, .03, data};
  Simulation::Population next = Simulation::Evolve(population);
  CHECK(next.Total() == 7010);
  CHECK(next.Beta() == .3);
  CHECK(next.Gamma() == .03);
  CHECK(next.S()==6997);
  CHECK(next.I()==13);
  CHECK(next.R()==0);
  }
  SUBCASE("N~S && Beta<Gamma"){
    Simulation::Data data{7000, 10, 0};
  Simulation::Population population{.03, .3, data};
  Simulation::Population next = Simulation::Evolve(population);
  CHECK(next.Total() == 7010);
  CHECK(next.Beta() == .03);
  CHECK(next.Gamma() == .3);
  CHECK(next.S()==7000);
  CHECK(next.I()==7);
  CHECK(next.R()==3);
  }
}