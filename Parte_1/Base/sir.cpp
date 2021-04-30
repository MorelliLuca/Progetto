#include "sir.hpp"
#include <cassert>
#include <iomanip>
#include <iostream>
#include <vector>

namespace Simulation {

// Funzione Evolve-Calcola lo stato successivo della popolazione

Population const Evolve(Population const& initial_population)
{
  double gamma{initial_population.Gamma()};
  double beta{initial_population.Beta()};
  double N{static_cast<double>(initial_population.Total())};
  Population next{beta, gamma};
  next.S() = static_cast<int>(initial_population.S() - beta * (initial_population.S() / N) * initial_population.I() + 0.5);
  assert(next.S() >= 0);
  next.I() =
      static_cast<int>(initial_population.I() + beta * (initial_population.S() / N) * initial_population.I() - gamma * initial_population.I() + 0.5);
  assert(next.I() >= 0);
  next.R() = static_cast<int>(initial_population.R() + gamma * initial_population.I() + 0.5);
  assert(next.R() >= 0);

  if (next.Total() != initial_population.Total() && next.S() != 0)  // Correzione fluttuazione del numero totale di persone
  {
    next.S() += initial_population.Total() - next.Total();
  }
  if (next.Total() != initial_population.Total() && next.S() == 0) { next.R() += initial_population.Total() - next.Total(); }

  if (next.S() <= 0)  // Condizione S>0
  {
    next.I() += -next.S();
    next.S() = 0;
  }

  if (next.I() <= 0)  // Condizione I>0
  {
    next.R() += -next.I();
    next.I() = 0;
  }

  assert(next.Total() == initial_population.Total());
  return next;

  // Funzione simulate-Restituisce un vettore Population con gli stati giorno
  // per giorno
}
std::vector<Population> Simulate(int T_duration, Population const& initial_population)
{
  std::vector<Population> result{initial_population};
  result.reserve(T_duration+1);
  for (int i{0}; i < T_duration; ++i) {
    if (result[i].I() * result[i].Gamma() <= .5 && result[i].Beta() * result[i].S() / result[i].Total() * result[i].I() <= 0.5) {
      std::cerr << "Simulation terminated for statistical limit\nEpidemy can be considered ended at day " << i << "\n";
      break;
    }
    if (result[i].I() == 0) {
      std::cerr << "Simulation terminated at day " << i << " because there are 0 infected\n";
      break;
    }
    result.push_back(Evolve(result[i]));
  }
  return result;
}
void Print(std::vector<Population> const& simulated)
{
  std::cout << "-----------------------------------------------------------\n"
            << "Simulation        Days: " << simulated.size()-1 << std::setw(18) << "        N: " << simulated[0].Total()
            << "\n"
               "-----------------------------------------------------------\n"
               "|  Day  |    S    |    I    |    R    |  Beta  |  Gamma  |\n";
  for (auto it = simulated.begin(); it!=simulated.end(); ++it) {
    std::cout << "| " << std::setw(5) << std::distance(simulated.begin(),it) << " | " << std::setw(7) << it->S() << " | " << std::setw(7) << it->I() << " | "
              << std::setw(7) << it->R() << " | " << std::setw(6) << it->Beta() << " | " << std::setw(7) << it->Gamma()
              << " |\n";
  }
}

}  // namespace Simulation
