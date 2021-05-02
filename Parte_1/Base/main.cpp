#include <iostream>
#include <stdexcept>
#include <vector>
#include "sir.hpp"
#include <limits>

Simulation::Population get_parameter()
{  // Funzione che prende in input i parametri iniziali della simulazione
  double beta, gamma;
  Simulation::Data initial_state;
  std::cin >> beta >> gamma >> initial_state.S >> initial_state.I >> initial_state.R;
  if (initial_state.S < 0 || initial_state.I < 0 || initial_state.R < 0 || beta < 0 || gamma < 0) {
    throw std::invalid_argument{"These parameters can't be less than 0"};
  }
  
  if (std::cin.fail()) {
    throw std::invalid_argument{"These parameters have to be numbers"};
  }
  if (initial_state.S == 0 && initial_state.I == 0 && initial_state.R == 0) { throw std::invalid_argument{"These parameters can't all be 0"}; }
Simulation::Population population{beta, gamma, initial_state};
return population;
}

int main()
{
  while (1) {
    try {
      Simulation::Population initial_population{get_parameter()};
      int simulation_t;
      std::cin >> simulation_t;
      if (simulation_t <= 0) { throw std::invalid_argument{"Time has to be more than 0"}; }
      if (std::cin.fail()) {
        throw std::invalid_argument{"These parameters have to be numbers"};
        
      }

      std::vector<Simulation::Population> population{Simulation::Simulate(simulation_t, initial_population)};
      Simulation::Print(population);
      break;
    } catch (std::invalid_argument const& e) {
      std::cerr <<"\033[31mInavalid input:\033[0m "<< e.what() <<'\n';
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } catch (...) {
      std::cerr << "Caught unknown exception" << '\n';
      return EXIT_FAILURE;
    }
  }
  return 0;
}