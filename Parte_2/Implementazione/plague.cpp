#include "plague.hpp"
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

namespace Simulation {

constexpr int Min_virus_distribution{0};  // Valore minimo generato randomicamente per determinare infezione e guarigioni
constexpr int Max_virus_distribution{1};  // Valore massimo generato randomicamente per determinare infezione e guarigioni
constexpr int Min_walk_distribution{-1};  // Valore minimo generato randomicamente per consentire alle celle di muoversi
constexpr int Max_walk_distribution{1};   // Valore massimo generato randomicamente per consentire alle celle di muoversi
constexpr double Total_near_person{8};    // Persone limitrofe totali
constexpr int Term_width_day{5};          // Dimensione colonna day
constexpr int Term_width_SIR{7};          // Dimensione  colonna S, I e R
constexpr int Term_width_opt{6};           // Dimensione colonna delle opzioni

std::default_random_engine gen{std::random_device{}()};
std::uniform_real_distribution<double> dis_virus(Min_virus_distribution, Max_virus_distribution);
std::uniform_int_distribution<int> dis_walk(Min_walk_distribution, Max_walk_distribution);

int I_near(World const& world, int r, int c)  // Funzione che controlla quante persone sono infette delle 8 adiacenti ad una data
{
  int result = 0;
  for (int i : {-1, 0, 1}) {
    for (int j : {-1, 0, 1}) {
      if (world.person(r + i, c + j) == Person::I) {
        ++result;
      }
    }
  }
  return result;
}

Person person_next_status(World const& world, int r, int c)  // Funzione che determina quale sarà il prossimo stato di una persona
{
  int const near_I{I_near(world, r, c)};  // Numero di infetti vicino alla persona in poszione (r,c)
  double beta{world.get_beta()};
  if(world.mask_status()==Simulation::Mask::ON)
  {
    beta=beta/2;
  }
  switch (world.person(r, c)) {
    case Person::S:  // Caso persona sana
      if (near_I != 0 && dis_virus(gen) <= (beta + (beta / Total_near_person) * near_I)) {
        return Person::I;
      } else if (world.vax_status() == Vax::ON && dis_virus(gen) <= world.get_theta()) {
        return Person::V;
      }
      break;
    case Person::I:  // Caso persona infetta
      if (dis_virus(gen) <= world.get_gamma()) {
        return Person::R;
      }
      break;
    default:  // Caso persona rimossa o vuota o vaccinata
      break;
  }
  return world.person(r, c);
}
World evolve(World const& current_world)  // Funzione che determina la configurazione successiva della popolazione
{
  World next = current_world;
  for (int i = 0; i != current_world.get_side(); ++i) {
    for (int j = 0; j != current_world.get_side(); ++j) {
      next.person(i, j) = person_next_status(current_world, i, j);
    }
  }
  return next;
}

void print_intestation(World const& world)  // Funzione che stampa a terminale l'intestazione della tabella dei dati simulati
{
  std::cout << "--------------------------------------------------------------------\n"
            << "Simulation                N: " << world.get_side() * world.get_side() - world.get_E()
            << "\n"
               "-------------------------------------------------------------------\n"
               "|  Day  |    \033[;32mS\033[0m    |    \033[;31mI\033[0m    |    \033[;37mR\033[0m    |    \033[;34mV\033[0m    |  MASK  |  LCKD  |\n";
}

void print_terminal(World const& world, int day)  // Funzione che stampa a terminale i risultati della simulazione
{
  std::cout << "| " << std::setw(Term_width_day) << day << " | " << std::setw(Term_width_SIR) << world.get_S() << " | " 
            << std::setw(Term_width_SIR) << world.get_I() << " | " << std::setw(Term_width_SIR) << world.get_R() << " | " 
            << std::setw(Term_width_SIR) << world.get_V() << " | " << std::setw(Term_width_opt) << world.string_state_mask() << " | " 
            << std::setw(Term_width_opt) << world.string_state_lockdown() << " |\n";
}

void swap(World& world, int r1, int c1, int r2, int c2)
{
  if ((r1 != -1 && c1 != -1 && r2 != -1 && c2 != -1) && (r1 != world.get_side() && c1 != world.get_side() && r2 != world.get_side() && c2 != world.get_side())) {
    Person temp_person{world.person(r2, c2)};
    world.person(r2, c2) = world.person(r1, c1);
    world.person(r1, c1) = temp_person;
  }
}

void walk(World& world)
{
  std::vector<int> E_coord{world.find_E()};
  for (auto it{E_coord.begin()}; it != E_coord.end(); ++it) {
    int r1{*it};
    ++it;
    int c1{*it};

    int r2 = r1 + dis_walk(gen);
    int c2 = c1 + dis_walk(gen);

    swap(world, r1, c1, r2, c2);
  }
}

}  // namespace Simulation