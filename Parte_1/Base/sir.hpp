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
  static constexpr int min{0};
  static constexpr double variation_min{0.5};
};

class Population
{
  Data state;
  double beta;
  double gamma;

 public:
  Population(double b, double g, Data initial_state) : state{initial_state}, beta{b}, gamma{g}
  {
    assert(state.I >= Data::min);
    assert(state.R >= Data::min);
    assert(state.S >= Data::min);
    assert(beta >= Data::min);
    assert(gamma >= Data::min);
  }

  Population(double b, double g) : beta{b}, gamma{g}
  {
    assert(beta >= Data::min);
    assert(gamma >= Data::min);
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