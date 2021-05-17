#ifndef PLAGUE_HPP
#define PLAGUE_HPP

#include <cassert>
#include <vector>

namespace Simulation {

enum class Person { S, I, R };  //Enum class degli stati della salute di una persona 

class World  //Classe che contine i dati del mondo 
{
  using Grid = std::vector<Person>; //Alias di un vettore di persone
  int side;  //Numero di persone per lato del mondo
  Grid grid; //Griglia delle persone
  double beta;
  double gamma;
  static constexpr Person removed_person = Person::R; //Stato delle persone esterne alla griglia 

 public:
  World(int N, double b, double g) : side{N}, grid(N * N, Person::S), beta{b}, gamma{g} //Costruttore
  {
    //Condizione necessaria per il senso della simulazione
    assert(beta >= 0);
    assert(gamma >= 0);
    assert(N > 0);
  }

  Person const& person(int r, int c) const  //Funzione menbro che consente di accedere allo stato di una determinata persona
  {
    //Condizioni necessarie perchè si stia richidendo una persona della griglia 
    assert(r <= side && c <= side);
    assert(r >= -1 && c >= -1);
    //Caso in cui si richieda lo stato di una persona appena esterna alla griglia di default R (rimossa) 
    //necessario per poter controllare le persone adiacenti ad una persona sul bordo della griglia
    if (r == -1 || c == -1 || r == side || c == side) { return removed_person; }
    int cursor = r * side + c; //Conversione delle coordinate bidimensionali alla posizione nel vettore grid
    return grid[cursor];
  }

  Person& person(int r, int c)  //Funzione Membro che consente di modificare lo stato di una persona
  {
    //Condizioni necessarie perchè si stia richidendo una persona della griglia 
    assert(r < side && c < side);
    assert(r >= 0 && c >= 0);
    int cursor = r * side + c;//Conversione delle coordinate bidimensionali alla posizione nel vettore grid
    return grid[cursor];
  }
  //Funzioni che consentono di accedere ai dati privati di World
  int get_side() const { return side; }
  double const& get_beta() const { return beta; }
  double const& get_gamma() const { return gamma; }
  int get_S() const
  {
    int count{0};  //Conteggio delle persone S
    for (auto it{grid.begin()}; it != grid.end(); it++) {
      if (*it == Person::S) { ++count; }
    }
    return count;
  }
  int get_I() const
  {
    int count{0};  //Conteggio delle persone I
    for (auto it{grid.begin()}; it != grid.end(); it++) {
      if (*it == Person::I) { ++count; }
    }
    return count;
  }
  int get_R() const
  {
    int count{0};  //Conteggio delle persone I
    for (auto it{grid.begin()}; it != grid.end(); it++) {
      if (*it == Person::R) { ++count; }
    }
    return count;
  }
};

inline bool operator==(World const& left, World const& right) //Operatore == per la classe World necessario per i test
{
  bool result;
  for (int i{0}; i < left.get_side(); ++i) {
    for (int j{0}; j < left.get_side(); ++j) {
      result = left.person(i, j) == right.person(i, j);
      if (result == false) { return result; }
    }
  }
  return result;
}

inline bool operator!=(World const& left, World const& right) { return !(left == right); } //Operatore != per la classe World necessario per i test

//Dichiarazione delle free function di plague.cpp

int I_near(World const& world, int r, int c);

Person person_next_status(World const& world, int r, int c);

World evolve(World const& current_world);

void print_terminal(Simulation::World& world, int day);

void print_intestation(World const& world);

}  // namespace Simulation

#endif