#ifndef PLAGUE_HPP
#define PLAGUE_HPP

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace Simulation {

constexpr int Mask_factor{2};

enum class Person { E, S, I, R, V };  // Enum class degli stati della salute di una persona

enum class Mask { OFF, ON };  // Enum dello stato di uso delle mascherine

enum class Lockdown { OFF, ON };  // Enum dello stato del lockdown

enum class Vax { OFF, ON };  // Enum dello stato della vaccinazione

// Classe che contine i dati del mondo
class World
{
  using Grid = std::vector<Person>;  // Alias di un vettore di persone
  int side;                          // Numero di persone per lato del mondo
  Grid grid;                         // Griglia delle persone
  double beta;
  double gamma;
  double theta;  // Rapporto di vaccinazione giornaliero
  Mask mask = Simulation::Mask::OFF;
  Lockdown lockdown = Simulation::Lockdown::OFF;
  Vax vax = Simulation::Vax::OFF;
  static constexpr Person Outside_person = Person::E;  // Stato delle persone esterne alla griglia
  static constexpr int Outside_coord{-1};              // Coordinate delle celle esterne nel bordo superiore e di sinistra
 public:
  // Valori limite dei paramentri
  static constexpr int Data_min{0};        // Valor minimo assunto dai dati
  static constexpr int Beta_Gamma_Max{1};  // Valor massimo assunto da gamma e beta

  // Costruttore
  World(int N, double b, double g, double t) : side{N}, grid(N * N, Person::S), beta{b}, gamma{g}, theta{t}
  {
    // Condizione necessaria per il senso della simulazione
    assert(beta >= Data_min);
    assert(gamma >= Data_min);
    assert(N > Data_min);
    assert(beta <= Beta_Gamma_Max);
    assert(gamma <= Beta_Gamma_Max);
  }

  // Funzione menbro che consente di accedere allo stato di una determinata persona
  Person const& person(int r, int c) const
  {
    // Condizioni necessarie perchè si stia richidendo una persona della griglia
    assert(r <= side && c <= side);
    assert(r >= Outside_coord && c >= Outside_coord);
    // Caso in cui si richieda lo stato di una persona appena esterna alla griglia di default R (rimossa)
    // necessario per poter controllare le persone adiacenti ad una persona sul bordo della griglia
    if (r == Outside_coord || c == Outside_coord || r == side || c == side) {
      return Outside_person;
    }
    int cursor = r * side + c;  // Conversione delle coordinate bidimensionali alla posizione nel vettore grid
    return grid[cursor];
  }

  // Funzione Membro che consente di modificare lo stato di una persona
  Person& person(int r, int c)
  {
    // Condizioni necessarie perchè si stia richidendo una persona della griglia
    assert(r < side && c < side);
    assert(r >= Data_min && c >= Data_min);
    int cursor = r * side + c;  // Conversione delle coordinate bidimensionali alla posizione nel vettore grid
    return grid[cursor];
  }
  // Funzioni che consentono di accedere ai dati privati di World
  int get_side() const
  {
    return side;
  }
  double const& get_beta() const
  {
    return beta;
  }
  double const& get_gamma() const
  {
    return gamma;
  }
  double const& get_theta() const
  {
    return theta;
  }
  Mask mask_status() const
  {
    return mask;
  }
  Lockdown lockdown_status() const
  {
    return lockdown;
  }
  Vax vax_status() const
  {
    return vax;
  }
  int get_S() const  // Conteggio delle persone S
  {
    return std::count_if(grid.begin(), grid.end(), [](Person person) { return person == Person::S; });
  }
  int get_I() const  // Conteggio delle persone I
  {
    return std::count_if(grid.begin(), grid.end(), [](Person person) { return person == Person::I; });
  }
  int get_R() const  // Conteggio delle persone R
  {
    return std::count_if(grid.begin(), grid.end(), [](Person person) { return person == Person::R; });
  }
  int get_E() const  // Conteggio delle persone E
  {
    return std::count_if(grid.begin(), grid.end(), [](Person person) { return person == Person::E; });
  }
  int get_V() const  // Conteggio delle persone V
  {
    return std::count_if(grid.begin(), grid.end(), [](Person person) { return person == Person::V; });
  }

  // Funzioni che restituiscono una stringa con lo stato di Mask Lockdown e Vax
  std::string const string_state_mask() const
  {
    return (mask == Mask::ON) ? "ON" : "OFF";
  }

  std::string const string_state_lockdown() const
  {
    return (lockdown == Lockdown::ON) ? "ON" : "OFF";
  }

  // Funzione che restituisce un vettore con le coordinate delle celle vuote
  std::vector<int> find_E() const
  {
    std::vector<int> result;
    auto it{grid.begin()};
    while (it != grid.end()) {
      it = std::find(it, grid.end(), Person::E);
      if (it != grid.end()) {
        int position = std::distance(grid.begin(), it);
        result.push_back(position / side);
        result.push_back(position % side);
        it++;
      }
    }
    return result;
  }

  // Funzioni che modificano gli stati di Mask Lockdown e Vax
  void start_vax()
  {
    vax = Vax::ON;
  }

  void change_mask()
  {
    if (mask == Mask::ON) {
      mask = Mask::OFF;
    } else {
      mask = Mask::ON;
    }
  }

  void change_lockdown()
  {
    if (lockdown == Lockdown::ON) {
      lockdown = Lockdown::OFF;
    } else {
      lockdown = Lockdown::ON;
    }
  }
};

// Operatore == per la classe World necessario per i test
inline bool operator==(World const& left, World const& right)
{
  bool result;
  for (int i{0}; i < left.get_side(); ++i) {
    for (int j{0}; j < left.get_side(); ++j) {
      result = left.person(i, j) == right.person(i, j);
      if (result == false) {
        return result;
      }
    }
  }
  return result;
}

// Operatore != per la classe World necessario per i test
inline bool operator!=(World const& left, World const& right)
{
  return !(left == right);
}

// Dichiarazione delle free function di plague.cpp

int I_near(World const& world, int r, int c);

Person person_next_status(World const& world, int r, int c);

World evolve(World const& current_world);

void print_terminal(Simulation::World const& world, int day);

void print_intestation(World const& world);

void swap(World& world, int r1, int c1, int r2, int c2);

void walk(World& world);

}  // namespace Simulation

#endif