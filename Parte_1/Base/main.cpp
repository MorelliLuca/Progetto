#include <iostream>
#include <vector>
#include "sir.hpp"

int main()
{
  double beta, gamma;
  Simulation::Data initial_state;
  int simulation_t;
  std::cin >> beta >> gamma >> initial_state.S >> initial_state.I >> initial_state.R >> simulation_t;
  Simulation::Population initial_population{beta, gamma, initial_state};
  std::vector<Simulation::Population> population{Simulation::Simulate(simulation_t, initial_population)};
  Simulation::Print(population);
}