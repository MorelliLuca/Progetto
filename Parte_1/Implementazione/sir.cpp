#include "sir.hpp"
#include <cassert>
#include <iomanip>
#include <iostream>
#include <vector>
#include <string>

namespace Simulation {

constexpr int Term_width_q{5};            //Dimensione colonna quarantena
constexpr int Term_width_day{5};          // Dimensione colonna day
constexpr int Term_width_SIR{7};           // Dimensione  colonna S, I e R
constexpr int Term_width_beta{6};          // Dimensione colonna beta
constexpr int Term_width_gamma{7};         // Dimensione colonna gamma
constexpr int Term_width_intestation{23};  // Dimensione spaziatura intestazione

constexpr double Approx_term{0.5};  // Termine utilizzato per arrotondare i double a int con lo static_cast

// Funzione Evolve: calcola lo stato successivo della popolazione

Population const Evolve(Population const& initial_population)  // Funzione che determina l'evoluzione della popolazione con il modello SIR
{
   double gamma{initial_population.Gamma()};
   double beta{initial_population.Beta()};
   double N{static_cast<double>(initial_population.Total())};
   std::string quar{initial_population.Quarantine()};
   if (initial_population.I()>=(N/3))
   {
      beta=beta-0.05*beta;
      quar="Yes";
   }
   else{
      quar="No";
   }
   Population next{beta, gamma};
   next.Quarantine()=quar;
   // Calcolo dei valori successivi di S,I e R
   next.S() = static_cast<int>(initial_population.S() - beta * (initial_population.S() / N) * initial_population.I() + Approx_term);
   assert(next.S() >= Data::min);
   next.I() = static_cast<int>(initial_population.I() + beta * (initial_population.S() / N) * initial_population.I() -
                               gamma * initial_population.I() + Approx_term);
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
      for (int i{Data::min}; i < T_duration; ++i) {
            // Terminazione della simulazione se le variazioni di popolazione sono troppo piccole per poter esser valutate in seguito alle
            // approssimazioni
            if (result[i].I() * result[i].Gamma() <= Simulation::Data::Variation_min &&
                result[i].Beta() * result[i].S() / result[i].Total() * result[i].I() <= Simulation::Data::Variation_min) {
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
             << "Simulation        Days: " << simulated.size() - 1 << std::setw(Term_width_intestation) << "        N: " << simulated[0].Total()
             << "\n"
                "----------------------------------------------------------------\n"
                "|  Day  |    S    |    I    |    R    |  Beta  |  Gamma  |  Quar  |\n";
      for (auto it = simulated.begin(); it != simulated.end(); ++it) {
         std::cout << "| " << std::setw(Term_width_day) << std::distance(simulated.begin(), it) << " | " << std::setw(Term_width_SIR) << it->S()
                   << " | " << std::setw(Term_width_SIR) << it->I() << " | " << std::setw(Term_width_SIR) << it->R() << " | "
                   << std::setw(Term_width_beta) << it->Beta() << " | " << std::setw(Term_width_gamma) << it->Gamma() << "| " << std::setw(Term_width_q) << it->Quarantine() << " |\n";
      }
}

}  // namespace Simulation
