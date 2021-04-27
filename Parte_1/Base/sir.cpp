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
<<<<<<< HEAD
  next.S()=static_cast<int>(initial_population.S()-beta*(initial_population.S()/N)*initial_population.I());
  next.I()=static_cast<int>(initial_population.I()+beta*(initial_population.S()/N)*initial_population.I()-gamma*initial_population.I());
  next.R()=static_cast<int>(initial_population.R()+gamma*initial_population.I());
  assert(next.S()+next.R()+next.I()==N);
=======
  next.S()=static_cast<int>(initial_population.S()-beta*(initial_population.S()/N)*initial_population.I()+0.5);
  assert(next.S()>=0);
  next.I()=static_cast<int>(initial_population.I()+beta*(initial_population.S()/N)*initial_population.I()-gamma*initial_population.I()+0.5);
  assert(next.I()>=0);
  next.R()=static_cast<int>(initial_population.R()+gamma*initial_population.I()+0.5);
  assert(next.R()>=0);
  assert(next.S()+next.I()+next.R()==N);
>>>>>>> c33342c790912d0f81c918c3f9e6831b2db71187
  return next;

}

}  // namespace Simulation
