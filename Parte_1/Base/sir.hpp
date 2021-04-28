#ifndef SIR_HPP
#define SIR_HPP

#include <cassert>
#include <vector>

namespace Simulation {

struct Data
{
  int S{0};
  int I{0};
  int R{0};
};

class Population
{
  Data state;
  double beta;
  double gamma;

 public:
  Population(double b, double g, Data initial_state) : state{initial_state}, beta{b}, gamma{g}
  {
    assert(state.I >= 0);
    assert(state.R >= 0);
    assert(state.S >= 0);
    assert(beta >= 0);
    assert(gamma >= 0);
  }

  Population(double b, double g) : beta{b}, gamma{g}
  {
    assert(beta >= 0);
    assert(gamma >= 0);
  }

  // Funzioni membro per accedere ai dati privati

  int& S() { return state.S; }
  int& I() { return state.I; }
  int& R() { return state.R; }
  int const& S() const { return state.S; }
  int const& I() const { return state.I; }
  int const& R() const { return state.R; }
  int Total() const { return state.S + state.I + state.R; }
  double const& Beta() const { return beta; }
  double const& Gamma() const { return gamma; }
};

Population const Evolve(Population const& initial_population);
std::vector<Population> Simulate(int T_duration, Population const& initial_population);
void Print(std::vector<Population> const& simulated);
}  // namespace Simulation

#endif