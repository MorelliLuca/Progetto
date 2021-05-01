#include <iostream>
#include <vector>
#include "sir.hpp"


Simulation::Population get_parameter(){  //Funzione che prende in input i parametri iniziali della simulazione
double beta, gamma;
Simulation::Data initial_state;
std::cin >> beta >> gamma >> initial_state.S >> initial_state.I >> initial_state.R;
Simulation::Population population{beta,gamma,initial_state};
return population;
}


int main()
{
  Simulation::Population initial_population{get_parameter()};
  int simulation_t;
  std::cin>>simulation_t;
  std::vector<Simulation::Population> population{Simulation::Simulate(simulation_t, initial_population)};
  Simulation::Print(population);
  return 0;
}