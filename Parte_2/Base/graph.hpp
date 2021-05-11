#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <SFML/Graphics.hpp>
#include "plague.hpp"

namespace Display {
double person_size(sf::RenderWindow const& window, Simulation::World const& world);

void print(sf::RenderWindow& window, Simulation::World const& world);

void setStatus(sf::RenderWindow& window, Simulation::World& world);

}  // namespace Display

#endif