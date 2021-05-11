#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <thread>
#include "graph.hpp"
#include "plague.hpp"

Simulation::World get_parameter()
{  // Funzione che prende in input i parametri iniziali della simulazione
  int side;
  double beta, gamma;
  std::cin >> side >> beta >> gamma;
  if (beta < 0 || gamma < 0) { throw std::invalid_argument{"These parameters can't be less than 0"}; }
  if (side <= 0) { throw std::invalid_argument{"This parameter has to be more than 0"}; }
  if (beta > 1 || gamma > 1) { throw std::invalid_argument{"Beta and gamma can't be more than 1"}; }
  if (std::cin.fail()) { throw std::invalid_argument{"These parameters have to be numbers"}; }
  Simulation::World parameters{side, beta, gamma};
  return parameters;
}

int main()
{
  while (1) {
    try {
      Simulation::World world{get_parameter()};
      // Graphing
      sf::RenderWindow w_grid(sf::VideoMode(400, 400), "SIR Simulation");
      Display::print(w_grid, world);
      while (w_grid.isOpen()) {
        sf::Event event;
        Display::setStatus(w_grid, world);
        Display::print(w_grid, world);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
          while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && w_grid.isOpen()) {
            world = Simulation::evolve(world);
            Display::print(w_grid, world);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            while (w_grid.pollEvent(event)) {
              if (event.type == sf::Event::Closed) { w_grid.close(); }
            }
          }
        }

        while (w_grid.pollEvent(event)) {
          if (event.type == sf::Event::Closed) w_grid.close();
        }
      }

      break;
    } catch (std::invalid_argument const& e) {
      std::cerr << "\033[31mInavalid input:\033[0m " << e.what() << '\n';
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      break;
    } catch (std::runtime_error const& e) {
      std::cerr << "\033[31mError:\033[0m " << e.what() << '\n';
    } catch (...) {
      std::cerr << "Caught unknown exception" << '\n';
      return EXIT_FAILURE;
    }
  }

  return 0;
}