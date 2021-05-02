#include "plague.hpp"
#include <cassert>
#include <vector>
#include <random>

namespace Simulation {

std::mt19937 gen;
std::uniform_real_distribution<> dis (0.0,1.0);

inline int I_near(World const& world, int r, int c)
{
  int result = 0;
  for (int i : {-1, 0, 1}) {
    for (int j : {-1, 0, 1}) {
      if (world.person(r+i, c+j)==Person::I)
      {
        ++result;
      }
    }
  }
  return result;
}

inline Person check_next_status (World const& world, int r, int c){
    Person const& person {world.person(r,c)};
    double beta{world.get_beta()};
     double gamma{world.get_gamma()};
    switch (person){
     case Person::S: if (dis(gen)<=beta+beta/8*I_near(world,r,c)){
         return Person::I;
     }
     case Person::I: if (dis(gen)<=gamma){
         return Person::R;
     }
     else {return Person::I;}
     case Person::R: return Person::R;  
    }
    
}
inline World evolve (World const& current_world) {
    int const N = current_world.get_side();
    World next(N, current_world.get_beta(), current_world.get_gamma());
    for (int i=0; i!=N; ++i){
        for (int j=0; j!=0; ++j){
            next.person(i,j)=check_next_status(current_world,i,j);
        }
    }

}





}