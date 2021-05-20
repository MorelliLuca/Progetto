#ifndef PLAGUE_HPP
#define PLAGUE_HPP

#include <cassert>
#include <vector>
#include <algorithm>
#include <iostream>

namespace Simulation {

enum class Person { E, S, I, R };  // Enum class degli stati della salute di una persona

class World  // Classe che contine i dati del mondo
{
   using Grid = std::vector<Person>;  // Alias di un vettore di persone
   int side;                          // Numero di persone per lato del mondo
   Grid grid;                         // Griglia delle persone
   double beta;
   double gamma;
   static constexpr Person outside_person=Person::E;  // Stato delle persone esterne alla griglia
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
   int get_S() const // Conteggio delle persone S
   {
      return std::count_if(grid.begin(),grid.end(),[](Person person){return person==Person::S;});
   }
   int get_I() const // Conteggio delle persone I
   {
      return std::count_if(grid.begin(),grid.end(),[](Person person){return person==Person::I;});
   }
   int get_R() const // Conteggio delle persone R
   {
      return std::count_if(grid.begin(),grid.end(),[](Person person){return person==Person::R;});
   }
   int get_E() const // Conteggio delle persone E
   {
      return std::count_if(grid.begin(),grid.end(),[](Person person){return person==Person::E;});
   }

   std::vector<int> find_E() const  //Funzione che restituisce un vettore con le coordinate delle celle vuote
   {
      std::vector<int> result;
      auto it{grid.begin()};
      while(it!=grid.end())
      {
         it = std::find(it,grid.end(),Person::E);
         if(it!=grid.end()){
         int position=std::distance(grid.begin(),it);
         result.push_back(position/side);
         result.push_back(position%side);
         it++;
         }
      }
      return result;
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

void print_terminal(Simulation::World const& world, int day,double R0);

void print_intestation(World const& world);

double eval_R0(World const& old_world,World const& new_World);

void swap(World& world,int r1,int c1,int r2, int c2);

void walk(World& world);


}  // namespace Simulation

#endif