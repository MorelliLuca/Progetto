#include "sir.hpp"
#include <cassert>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace Simulation {

constexpr int Term_width_q{6};            //Dimensione colonna quarantena
constexpr int Term_width_day{5};          // Dimensione colonna day
constexpr int Term_width_SIR{7};           // Dimensione  colonna S, I e R
constexpr int Term_width_beta{6};          // Dimensione colonna beta
constexpr int Term_width_gamma{7};         // Dimensione colonna gamma
constexpr int Term_width_n_vax{7};        //Dimensione colonna numero vaccini
constexpr int Term_width_intestation{23};  // Dimensione spaziatura intestazione

constexpr double Approx_term{0.5};  // Termine utilizzato per arrotondare i double a int con lo static_cast

// Funzione Evolve: calcola lo stato successivo della popolazione

Population const Evolve(Population const& initial_population)  // Funzione che determina l'evoluzione della popolazione con il modello SIR
{
   double gamma{initial_population.Gamma()};
   double beta{initial_population.Beta()};
   double N{static_cast<double>(initial_population.Total())};
   int n_vax{initial_population.N_vax()};
   std::string quar{initial_population.Quarantine()};
   if (initial_population.I()>=(N/3)&&quar=="No") // Controllo per l'attivazione della quarantena
   {
      beta=beta/2; // Beta si dimezza con la quarantena attiva
      quar="Yes";
   }
   else{
      if(quar=="Yes"&&initial_population.I()<(N/3)) // Con I minore di 1/3 del totale si toglie la quarantena e Beta torna normale
      {
          quar="No";
          beta=beta*2;
      } 
   }
   Population next{beta, gamma};
   next.Quarantine()=quar;
   // Calcolo dei valori successivi di S,I e R
   next.S() = static_cast<int>(initial_population.S() - beta * (initial_population.S() / N) * initial_population.I() + Approx_term);
   next.I() = static_cast<int>(initial_population.I() + beta * (initial_population.S() / N) * initial_population.I() - gamma * initial_population.I() + Approx_term);
  
   next.R() = static_cast<int>(initial_population.R() + gamma * initial_population.I() + Approx_term);
   
next.N_vax()=n_vax;
if (next.S()>=n_vax) //Modifiche a S e R se vi sono i vaccini e S è maggiore del numero di vaccini giornalieri
   {
     next.S()=next.S()-n_vax;
     next.R()=next.R()+n_vax;
   }
   else
   {
     next.R()=next.R()+next.S();
     next.S()=Data::min;
   }
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
  assert(next.S() >= Data::min);
   assert(next.R() >= Data::min); 
   assert(next.I() >= Data::min);
  assert(next.Total() == initial_population.Total());
  return next;

  // Funzione simulate-Restituisce un vettore Population con gli stati giorno
  // per giorno
}
std::vector<Population> Simulate(int T_duration, Population const& initial_population, int Start_vax)
{
  std::vector<Population> result{initial_population};
  int n_vax{initial_population.N_vax()};
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
    if (i<Start_vax) // Ciclo che controlla se iniziare a vaccinare
    {
      result[i].N_vax()=Data::min;
    }
    else
    {
      result[i].N_vax()=n_vax;
    }
    result.push_back(Evolve(result[i]));
  }
  return result;
}
void Print(std::vector<Population> const& simulated)  // Funzione che stampa la tabella con i risultati
{
  int i{0}, j{0};
   std::cout << "-----------------------------------------------------------------------------\n"
             << "Simulation        Days: " << simulated.size() - 1 << std::setw(Term_width_intestation) << "        N: " << simulated[0].Total()
             << "\n"
                "-----------------------------------------------------------------------------\n"
                "|  Day  |    S    |    I    |    R    |  Beta  |  Gamma  |  Quar  |  N vax  |\n";
      for (auto it = simulated.begin(); it != simulated.end(); ++it) {
        if (it->N_vax()==Data::min) //Stampa della tabella se non ci sono vaccini
        {
          std::cout << "| " << std::setw(Term_width_day) << std::distance(simulated.begin(), it) << " | " << std::setw(Term_width_SIR) << it->S()
                   << " | " << std::setw(Term_width_SIR) << it->I() << " | " << std::setw(Term_width_SIR) << it->R() << " | "
                   << std::setw(Term_width_beta) << it->Beta() << " | " << std::setw(Term_width_gamma) << it->Gamma() << " | " << std::setw(Term_width_q) << it->Quarantine() << " | " << std::setw(Term_width_n_vax) << it->N_vax() << " |\n";
        }
        else
        {
          if (it->S()>=it->N_vax()) //Stampa della tabella con il numero di vaccinati totali se S è maggiore del numero di vaccini giornalieri
          {
            i++;
              std::cout << "| " << std::setw(Term_width_day) << std::distance(simulated.begin(), it) << " | " << std::setw(Term_width_SIR) << it->S()
                   << " | " << std::setw(Term_width_SIR) << it->I() << " | " << std::setw(Term_width_SIR) << it->R() << " | "
                   << std::setw(Term_width_beta) << it->Beta() << " | " << std::setw(Term_width_gamma) << it->Gamma() << " | " << std::setw(Term_width_q) << it->Quarantine() << " | " << std::setw(Term_width_n_vax) << (it->N_vax()*i) << " |\n";
          }
          else
          {
            if (it->S()!=Data::min) //Stampa della tabella con i vaccinati totali aumentati di S e non del numero di vaccini giornalieri
            {
              std::cout << "| " << std::setw(Term_width_day) << std::distance(simulated.begin(), it) << " | " << std::setw(Term_width_SIR) << it->S()
                   << " | " << std::setw(Term_width_SIR) << it->I() << " | " << std::setw(Term_width_SIR) << it->R() << " | "
                   << std::setw(Term_width_beta) << it->Beta() << " | " << std::setw(Term_width_gamma) << it->Gamma() << " | " << std::setw(Term_width_q) << it->Quarantine() << " | " << std::setw(Term_width_n_vax) << ((it->N_vax()*i)+(it)->S()) << " |\n";
            }
            else{ //Tabella dove il numero di vaccinati totali non aumenta più
              j++;
              std::cout << "| " << std::setw(Term_width_day) << std::distance(simulated.begin(), it) << " | " << std::setw(Term_width_SIR) << it->S()
                   << " | " << std::setw(Term_width_SIR) << it->I() << " | " << std::setw(Term_width_SIR) << it->R() << " | "
                   << std::setw(Term_width_beta) << it->Beta() << " | " << std::setw(Term_width_gamma) << it->Gamma() << " | " << std::setw(Term_width_q) << it->Quarantine() << " | " << std::setw(Term_width_n_vax) << ((it->N_vax()*i)+(it-j)->S()) << " |\n";
            } 
          } 
        } 
      }
}

}  // namespace Simulation
