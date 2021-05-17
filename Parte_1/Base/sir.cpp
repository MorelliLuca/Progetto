#include "sir.hpp"
#include <cassert>
#include <iomanip>
#include <iostream>
#include <vector>

namespace Simulation {

constexpr double Approx_term{0.5};  // Termine utilizzato per arrotondare i double a int con lo static_cast

// Funzione Evolve: calcola lo stato successivo della popolazione

Population const Evolve(Population const& initial_population) //Funzione che determina l'evoluzione della popolazione con il modello SIR
{
  double gamma{initial_population.Gamma()};
  double beta{initial_population.Beta()};
  double N{static_cast<double>(initial_population.Total())};
  Population next{beta, gamma};
  //Calcolo dei valori successivi di S,I e R
  next.S() = static_cast<int>(initial_population.S() - beta * (initial_population.S() / N) * initial_population.I() + Approx_term);
  assert(next.S() >= Data::min);
  next.I() = static_cast<int>(initial_population.I() + beta * (initial_population.S() / N) * initial_population.I() - gamma * initial_population.I() +
                              Approx_term);
  assert(next.I() >= Data::min);
  next.R() = static_cast<int>(initial_population.R() + gamma * initial_population.I() + Approx_term);
  assert(next.R() >= Data::min);

  if (next.Total() != initial_population.Total() && next.S() != Data::min)  // Correzione fluttuazione del numero totale di persone con S>0
  {
    next.S() += initial_population.Total() - next.Total();
  }
  if (next.Total() != initial_population.Total() && next.S() == Data::min)  // Correzione fluttuazione del numero totale di persone con S=0
  {
    next.R() += initial_population.Total() - next.Total();
  }

  if (next.S() <= Data::min)  // Condizione S>0
  {
    next.I() += -next.S();
    next.S() = Data::min;
  }

  if (next.I() <= Data::min)  // Condizione I>0
  {
    next.R() += -next.I();
    next.I() = Data::min;
  }

  assert(next.Total() == initial_population.Total());
  return next;

  // Funzione simulate-Restituisce un vettore Population con gli stati giorno
  // per giorno
}
std::vector<Population> Simulate(int T_duration, Population const& initial_population)
{
  std::vector<Population> result{initial_population};
  result.reserve(T_duration + 1);
  for (int i{0}; i < T_duration; ++i) {
    // Terminazione della simulazione se le variazioni di popolazione sono troppo piccole per poter esser valutate in seguto alle approssimazioni
    if (result[i].I() * result[i].Gamma() <= Simulation::Data::variation_min &&
        result[i].Beta() * result[i].S() / result[i].Total() * result[i].I() <= Simulation::Data::variation_min) {
      std::cerr << "Simulation terminated for statistical limit\nEpidemy can be considered ended at day " << i << "\n";
      break;
    }
    // Terminazione della simulazione se il numero di I=0
    if (result[i].I() == Data::min) {
      std::cerr << "Simulation terminated at day " << i << " because there are 0 infected\n";
      break;
    }
    result.push_back(Evolve(result[i]));
  }
  return result;
}
void Print(std::vector<Population> const& simulated)  // Funzione che stampa la tabella con i risultati
{
  std::cout << "-----------------------------------------------------------\n"
            << "Simulation        Days: " << simulated.size() - 1 << std::setw(18) << "        N: " << simulated[0].Total()
            << "\n"
               "-----------------------------------------------------------\n"
               "|  Day  |    S    |    I    |    R    |  Beta  |  Gamma  |\n";
  for (auto it = simulated.begin(); it != simulated.end(); ++it) {
    std::cout << "| " << std::setw(5) << std::distance(simulated.begin(), it) << " | " << std::setw(7) << it->S() << " | " << std::setw(7) << it->I()
              << " | " << std::setw(7) << it->R() << " | " << std::setw(6) << it->Beta() << " | " << std::setw(7) << it->Gamma() << " |\n";
  }
}

}  // namespace Simulation
