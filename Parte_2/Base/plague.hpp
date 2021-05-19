#ifndef PLAGUE_HPP
#define PLAGUE_HPP

#include <cassert>
#include <vector>

namespace Simulation {

enum class Person { E, S, I, R };  // Enum class degli stati della salute di una persona

class World  // Classe che contine i dati del mondo
{
   using Grid = std::vector<Person>;  // Alias di un vettore di persone
   int side;                          // Numero di persone per lato del mondo
   Grid grid;                         // Griglia delle persone
   double beta;
   double gamma;
   static constexpr Person outside_person = Person::E;  // Stato delle persone esterne alla griglia
   static constexpr int Data_min{0};                    // Valor minimo assunto dai dati
   static constexpr int Beta_Gamma_Max{1};              // Valor massimo assunto da gamma e beta
   static constexpr int Outside_coord{-1};           // Coordinate delle celle esterne nel bordo superiore e di sinistra

  public:
   World(int N, double b, double g) : side{N}, grid(N * N, Person::S), beta{b}, gamma{g}  // Costruttore
   {
      // Condizione necessaria per il senso della simulazione
      assert(beta >= Data_min);
      assert(gamma >= Data_min);
      assert(N > Data_min);
      assert(beta <= Beta_Gamma_Max);
      assert(gamma <= Beta_Gamma_Max);
   }

   Person const& person(int r, int c) const  // Funzione menbro che consente di accedere allo stato di una determinata persona
   {
      // Condizioni necessarie perchè si stia richidendo una persona della griglia
      assert(r <= side && c <= side);
      assert(r >= Outside_coord && c >= Outside_coord);
         // Caso in cui si richieda lo stato di una persona appena esterna alla griglia di default R (rimossa)
         // necessario per poter controllare le persone adiacenti ad una persona sul bordo della griglia
         if (r == Outside_coord || c == Outside_coord || r == side || c == side) {
            return outside_person;
      }
      int cursor = r * side + c;  // Conversione delle coordinate bidimensionali alla posizione nel vettore grid
      return grid[cursor];
   }

   Person& person(int r, int c)  // Funzione Membro che consente di modificare lo stato di una persona
   {
      // Condizioni necessarie perchè si stia richidendo una persona della griglia
      assert(r < side && c < side);
      assert(r >= Data_min && c >= Data_min);
      int cursor = r * side + c;  // Conversione delle coordinate bidimensionali alla posizione nel vettore grid
      return grid[cursor];
   }
   // Funzioni che consentono di accedere ai dati privati di World
   int get_side() const { return side; }
   double const& get_beta() const { return beta; }
   double const& get_gamma() const { return gamma; }
   int get_S() const
   {
      int count{0};  // Conteggio delle persone S
         for (auto it{grid.begin()}; it != grid.end(); it++) {
               if (*it == Person::S) {
                  ++count;
            }
         }
      return count;
   }
   int get_I() const
   {
      int count{0};  // Conteggio delle persone I
         for (auto it{grid.begin()}; it != grid.end(); it++) {
               if (*it == Person::I) {
                  ++count;
            }
         }
      return count;
   }
   int get_R() const
   {
      int count{0};  // Conteggio delle persone I
         for (auto it{grid.begin()}; it != grid.end(); it++) {
               if (*it == Person::R) {
                  ++count;
            }
         }
      return count;
   }
   int get_E() const
   {
      int count{0};  // Conteggio delle persone E
         for (auto it{grid.begin()}; it != grid.end(); it++) {
               if (*it == Person::E) {
                  ++count;
            }
         }
      return count;
   }
};

inline bool operator==(World const& left, World const& right)  // Operatore == per la classe World necessario per i test
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

inline bool operator!=(World const& left, World const& right) { return !(left == right); }  // Operatore != per la classe World necessario per i test

// Dichiarazione delle free function di plague.cpp

int I_near(World const& world, int r, int c);

Person person_next_status(World const& world, int r, int c);

World evolve(World const& current_world);

void print_terminal(Simulation::World& world, int day);

void print_intestation(World const& world);

}  // namespace Simulation

#endif