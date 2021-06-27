#ifndef SIR_HPP
#define SIR_HPP

#include <cassert>
#include <string>
#include <vector>

namespace Simulation {

struct Data  // Struct contenente i valori S, I e R e costanti
{
  int S{0};
  int I{0};
  int R{0};
  static constexpr int min{0};                 // Valore minimo dei dati
  static constexpr double Variation_min{0.5};  // Variazione minima di popolazione per cui l'algoritmo è sensibile
  static constexpr int Max{1};                 // Valore massimo di beta e gamma
};

class Population  // Classe che rappresenta una popolazione
{
  Data state;  // Dati S I R
  double beta;
  double gamma;
  std::string quarantine;
  int n_vax;

 public:
  Population(double b, double g, Data initial_state, std::string q, int n) : state{initial_state}, beta{b}, gamma{g}, quarantine{q}, n_vax{n}
  {
    // Condizioni necessarie per il senso della simulazione
    assert(state.I >= Data::min);
    assert(state.R >= Data::min);
    assert(state.S >= Data::min);
    assert(beta >= Data::min);
    assert(gamma >= Data::min);
    assert(beta <= Data::Max);
    assert(gamma <= Data::Max);
  }

  Population(double b, double g) : beta{b}, gamma{g}
  {
    // Condizioni necessarie per il senso della simulazione
    assert(beta >= Data::min);
    assert(gamma >= Data::min);
    assert(beta <= Data::Max);
    assert(gamma <= Data::Max);
  }

  // Funzioni membro per accedere ai dati privati

  int& S()
  {
    return state.S;
  }
  int& I()
  {
    return state.I;
  }
  int& R()
  {
    return state.R;
  }
  int const& S() const
  {
    return state.S;
  }
  int const& I() const
  {
    return state.I;
  }
  int const& R() const
  {
    return state.R;
  }
  int& N_vax()
  {
    return n_vax;
  }
  int const& N_vax() const
  {
    return n_vax;
  }
  int Total() const
  {
    return state.S + state.I + state.R;
  }
  double& Beta()
  {
    return beta;
  }
  double const& Beta() const
  {
    return beta;
  }
  double const& Gamma() const
  {
    return gamma;
  }
  std::string const& Quarantine() const
  {
    return quarantine;
  }
  std::string& Quarantine()
  {
    return quarantine;
  }
};
// Dichiarazione free functions di sir.cpp
Population const Evolve(Population const& initial_population);
std::vector<Population> Simulate(int T_duration, Population const& initial_population, int Start_vax);
void Print(std::vector<Population> const& simulated);
}  // namespace Simulation

#endif