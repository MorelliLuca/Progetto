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
  Simulation::Population population{0.1, .2, data, "No", 0};
  CHECK(population.I() == 2);
  CHECK(population.S() == 1);
  CHECK(population.R() == 3);
  CHECK(population.Total() == 6);
  CHECK(population.Beta() == .1);
  CHECK(population.Gamma() == .2);
}

TEST_CASE("Testing Simulate Function with quarantine")
{
  Simulation::Data data{7000, 8000, 1000};
  Simulation::Population initial_population{0.1, .01, data, "No", 0};
  std::vector<Simulation::Population> simulated{Simulation::Simulate(10, initial_population, 0)};
  CHECK(simulated[0].S() == initial_population.S());
  CHECK(simulated[0].I() == initial_population.I());
  CHECK(simulated[0].R() == initial_population.R());
  CHECK(simulated[10].Beta() == 0.05);
  CHECK(simulated[10].Quarantine() == "Yes");
}

TEST_CASE("Testing with vaccines from the start")
{
  Simulation::Data data{7000, 8000, 1000};
  Simulation::Population initial_population{0.1, .01, data, "No", 2};
  std::vector<Simulation::Population> simulated{Simulation::Simulate(3, initial_population, 0)};
  CHECK(simulated[0].N_vax() == 2);
  CHECK(simulated[1].S() == 6823);
}

TEST_CASE("Testing with vaccines from day 2")
{
  Simulation::Data data{7000, 8000, 1000};
  Simulation::Population initial_population{0.1, .01, data, "No", 2};
  std::vector<Simulation::Population> simulated{Simulation::Simulate(3, initial_population, 2)};
  CHECK(simulated[0].N_vax() == 0);
  CHECK(simulated[2].N_vax() == 2);
  CHECK(simulated[1].S() == 6825);
  CHECK(simulated[2].S() == 6652);
}