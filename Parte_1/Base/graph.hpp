#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <vector>
#include <SFML/Graphics.hpp>
#include "sir.hpp"

namespace Display {
void print_I(sf::RenderWindow& window, std::vector<Simulation::Population> const& data_vector);
void print_R(sf::RenderWindow& window, std::vector<Simulation::Population> const& data_vector);
void print_S(sf::RenderWindow& window, std::vector<Simulation::Population> const& data_vector);

void print_axis(sf::RenderWindow& window, std::vector<Simulation::Population> const& data_vector);
}  // namespace Display

#endif