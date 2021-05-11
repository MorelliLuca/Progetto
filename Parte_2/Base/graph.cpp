#include "graph.hpp"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include <thread>
#include "plague.hpp"

namespace Display {
double person_size(sf::RenderWindow const& window, Simulation::World const& world) { return window.getSize().x / world.get_side(); }

void print(sf::RenderWindow& window, Simulation::World const& world)
{
  double person_s = person_size(window, world);
  window.clear(sf::Color::White);
  sf::RectangleShape square(sf::Vector2f(person_s - 1, person_s - 1));
  for (int r{0}; r < world.get_side(); ++r) {
    for (int c{0}; c < world.get_side(); ++c) {
      square.setPosition(r * (person_s), c * (person_s));
      if (world.person(r, c) == Simulation::Person::S) {
        square.setFillColor(sf::Color::Green);
      } else if (world.person(r, c) == Simulation::Person::I) {
        square.setFillColor(sf::Color::Red);
      } else {
        square.setFillColor(sf::Color::White);
      }
      window.draw(square);
    }
  }
  window.display();
}

void setStatus(sf::RenderWindow& window, Simulation::World& world)
{
  double person_s = person_size(window, world);
  sf::Vector2i local_position = sf::Mouse::getPosition(window);
  bool good_position = (local_position.x < world.get_side() && local_position.x >= 0 && local_position.y < world.get_side() && local_position.y >= 0);
  if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && good_position) {
    int r = static_cast<int>(local_position.x / person_s);
    int c = static_cast<int>(local_position.y / person_s);

    if (world.person(r, c) == Simulation::Person::S) {
      world.person(r, c) = Simulation::Person::I;
    } else if (world.person(r, c) == Simulation::Person::I) {
      world.person(r, c) = Simulation::Person::R;
    } else if (world.person(r, c) == Simulation::Person::R) {
      world.person(r, c) = Simulation::Person::S;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
}

}  // namespace Display