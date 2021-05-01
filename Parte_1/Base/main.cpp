#include <iostream>
#include <vector>
#include "sir.hpp"
#include <stdexcept>
using namespace std;


Simulation::Population get_parameter(){  //Funzione che prende in input i parametri iniziali della simulazione
double beta, gamma;
Simulation::Data initial_state;
while(2){
    std::cin >> beta >> gamma >> initial_state.S >> initial_state.I >> initial_state.R;
  if (initial_state.S<0||initial_state.I<0||initial_state.R<0||beta<0||gamma<0)
   {
    throw std::runtime_error{"These parameters can't be less than 0"};
   }
  if (initial_state.S==0&&initial_state.I==0&&initial_state.R==0)
   {
    throw std::runtime_error{"These parameters can't all be 0"};
   }
   if (cin.fail())
   {
     throw std::runtime_error{"These parameters have to be numbers"};
     cin.clear();
     cin.ignore();
   }
   else {break;}
}
Simulation::Population population{beta,gamma,initial_state};
return population;
}


int main()
{
  try {
  Simulation::Population initial_population{get_parameter()};
  int simulation_t;
  while(2){
    std::cin>>simulation_t;
  if (simulation_t<=0)
  {
    throw std::runtime_error{"Time has to be more than 0"};
  }
  if (cin.fail())
   {
     throw std::runtime_error{"These parameters have to be numbers"};
     cin.clear();
     cin.ignore();
   }
   else {break;}
  }
  std::vector<Simulation::Population> population{Simulation::Simulate(simulation_t, initial_population)};
  Simulation::Print(population);
  }
  catch (std::runtime_error const& e){
    std::cerr << e.what() << '\n';
  }
  catch (...) {
    std::cerr << "Caught unknown exception" <<'\n';
    return EXIT_FAILURE;
  }
  return 0;
}