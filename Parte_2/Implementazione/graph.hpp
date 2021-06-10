#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <SFML/Graphics.hpp>
#include "plague.hpp"

namespace Display {
// Dichiarazione delle free function di graph.cpp
double person_size(sf::RenderWindow const& window, Simulation::World const& world);

void print(sf::RenderWindow& window, Simulation::World const& world);

void set_status(sf::RenderWindow& window, Simulation::World& world);

void option(Simulation::World& world, sf::Event& event, sf::RenderWindow& window);

void opt_screen_print(sf::RenderWindow& window, Simulation::World& world);

void print_R(sf::RenderWindow& window, std::vector<Simulation::World> const& data_vector);
void print_I(sf::RenderWindow& window, std::vector<Simulation::World> const& data_vector);
void print_S(sf::RenderWindow& window, std::vector<Simulation::World> const& data_vector);
void print_V(sf::RenderWindow& window, std::vector<Simulation::World> const& data_vector);

void print_graph(sf::RenderWindow& window, std::vector<Simulation::World> const& data_vector);

void print_axis(sf::RenderWindow& window, std::vector<Simulation::World> const& data_vector);

}  // namespace Display

#endif