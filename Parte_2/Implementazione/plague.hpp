#ifndef PLAGUE_HPP
#define PLAGUE_HPP

#include <cassert>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>


namespace Simulation {


constexpr int Mask_factor{2};

enum class Person { E, S, I, R , V};  // Enum class degli stati della salute di una persona

enum class Mask {OFF, ON};

enum class Lockdown {OFF, ON};

enum class Vax {OFF, ON};

class World  // Classe che contine i dati del mondo
{
   using Grid = std::vector<Person>;  // Alias di un vettore di persone
   int side;                          // Numero di persone per lato del mondo
   Grid grid;                         // Griglia delle persone
   double beta;
   double gamma;
   double theta;
   double R0;
   Mask mask;
   Lockdown lockdown;
   Vax vax;
   static constexpr Person Outside_person=Person::E;  // Stato delle persone esterne alla griglia
   static constexpr int Outside_coord{-1};           // Coordinate delle celle esterne nel bordo superiore e di sinistra
   

  public:
  static constexpr double Low_R0{1};
   static constexpr double Medium_R0{3};
   static constexpr double High_R0{4};
   static constexpr int Data_min{0};                    // Valor minimo assunto dai dati
   static constexpr int Beta_Gamma_Max{1};              // Valor massimo assunto da gamma e beta
   World(int N, double b, double g,double t) : side{N}, grid(N * N, Person::S), beta{b}, gamma{g},  theta{t}, R0{b/g}   // Costruttore
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
            return Outside_person;
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
   double const& get_theta() const { return theta; }
   double const& get_R0() const { return R0; }
   Mask mask_status() const {return mask;}
   Lockdown lockdown_status() const {return lockdown;}
   Vax vax_status() const {return vax;}
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
   int get_V() const // Conteggio delle persone V
   {
      return std::count_if(grid.begin(),grid.end(),[](Person person){return person==Person::V;});
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

   double eval_R0(World const& old_world)
{
   double N{static_cast<double>(get_side()*get_side()-get_E())};
   double new_R{static_cast<double>(get_R())};
   double old_R{static_cast<double>(old_world.get_R())};
   double new_S{static_cast<double>(get_S())};
   double old_S{static_cast<double>(old_world.get_S())};
   R0=N*(1-new_S/old_S)/(new_R-old_R);
   if (R0>High_R0 && lockdown==Lockdown::OFF){
      lockdown=Lockdown::ON;
      }
   if (R0<Medium_R0 && lockdown==Lockdown::ON){
      lockdown=Lockdown::OFF;
      }
   if (R0>Medium_R0 && mask==Mask::OFF){
      mask=Mask::ON;
       beta=beta/Mask_factor;
      }
   if (R0<Low_R0 && mask==Mask::ON){
      mask=Mask::OFF;
      beta=beta*Mask_factor;
      }
   return R0;
}

void start_vax()
{
   vax=Vax::ON;
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

void print_terminal(Simulation::World const& world, int day);

void print_intestation(World const& world);

void swap(World& world,int r1,int c1,int r2, int c2);

void walk(World& world);


}  // namespace Simulation

#endif