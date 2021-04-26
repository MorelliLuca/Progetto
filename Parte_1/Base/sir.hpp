#ifndef SIR_HPP
#define SIR_HPP

struct Data {
  int S{0};
  int I{0};
  int R{0};
};

class Population {
  Data state;
  double beta;
  double gamma;

public:
  Population(double b, double g, Data initial_state) : state{initial_state}, beta{b}, gamma{g} {}

  // Funzioni membro per accedere ai dati privati

  int &S() { return state.S; }
  int &I() { return state.I; }
  int &R() { return state.R; }
  int const &S() const { return state.S; }
  int const &I() const { return state.I; }
  int const &R() const { return state.R; }
  int Total() const { return state.S + state.I + state.R; }
  double const &Beta() const { return beta; }
  double const &Gamma() const { return gamma; }
};

#endif