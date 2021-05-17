#include "plague.hpp"
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

namespace Simulation {

std::default_random_engine gen{std::random_device{}()};
std::uniform_real_distribution<> dis(0, 1);

inline int I_near(World const& world, int r, int c) //Funzione che controlla quante persone sono infette delle 8 adiacenti ad una data
{
  int result = 0;
  for (int i : {-1, 0, 1}) {
    for (int j : {-1, 0, 1}) {
      if (world.person(r + i, c + j) == Person::I) { ++result; }
    }
  }
  return result;
}

inline Person person_next_status(World const& world, int r, int c)  //Funzione che determina quale sarà il prossimo stato di una persona
{
  int const near_I{I_near(world, r, c)}; //Numero di infetti vicino alla persona in poszione (r,c)
  switch (world.person(r, c)) {
    case Person::S:  //Caso persona sana
      if (near_I != 0 && dis(gen) <= (world.get_beta() + (world.get_beta() / 8.) * near_I)) { return Person::I; }
      break;
    case Person::I:  //Caso persona infetta
      if (dis(gen) <= world.get_gamma()) { return Person::R; }
      break;
    case Person::R:  //Caso persona rimossa
      break;
  }
  return world.person(r, c);
}
World evolve(World const& current_world)  //Funzione che determina la configurazione successiva della popolazione
{
  World next{current_world.get_side(), current_world.get_beta(), current_world.get_gamma()};
  for (int i = 0; i != current_world.get_side(); ++i) {
    for (int j = 0; j != current_world.get_side(); ++j) { next.person(i, j) = person_next_status(current_world, i, j); }
  }
  return next;
}

void print_intestation(World const& world)  //Funzione che stampa a terminale l'intestazione della tabella dei dati simulati
{
  std::cout << "-----------------------------------------------------------\n"
            << "Simulation                N: " << world.get_side() * world.get_side()
            << "\n"
               "-----------------------------------------------------------\n"
               "|  Day  |    S    |    I    |    R    |  Beta  |  Gamma  |\n";
}

void print_terminal(World& world, int day)  //Funzione che stampa a terminale i risultati della simulazione
{
  std::cout << "| " << std::setw(5) << day << " | " << std::setw(7) << world.get_S() << " | " << std::setw(7) << world.get_I() << " | "
            << std::setw(7) << world.get_R() << " | " << std::setw(6) << world.get_beta() << " | " << std::setw(7) << world.get_gamma() << " |\n";
}

}  // namespace Simulation