#include "sir.hpp"
#include <cassert>

namespace Simulation {

    //Funzione Evolve-Calcola lo stato successivo della popolazione 

Population const Evolve(Population const& initial_population)
{
  double gamma{initial_population.Gamma()};
  double beta{initial_population.Beta()};
  double N{static_cast<double>(initial_population.Total())};
  Population next{beta, gamma};
  next.S()=static_cast<int>(initial_population.S()-beta*(initial_population.S()/N)*initial_population.I());
  next.I()=static_cast<int>(initial_population.I()+beta*(initial_population.S()/N)*initial_population.I()-gamma*initial_population.I());
  next.R()=static_cast<int>(initial_population.R()+gamma*initial_population.I());
  assert(next.S()+next.R()+next.I()==N);
  return next;
}

}  // namespace Simulation
