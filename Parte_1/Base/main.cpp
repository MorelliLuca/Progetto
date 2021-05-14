#include <SFML/Graphics.hpp>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>
#include "graph.hpp"
#include "sir.hpp"

Simulation::Population get_parameter()
{  // Funzione che prende in input i parametri iniziali della simulazione
  double beta, gamma;
  Simulation::Data initial_state;
  std::cin >> beta >> gamma >> initial_state.S >> initial_state.I >> initial_state.R;
  // Controlli sui dati inseriti
  if (initial_state.S < Simulation::Data::min || initial_state.I < Simulation::Data::min || initial_state.R < Simulation::Data::min ||
      beta < Simulation::Data::min || gamma < Simulation::Data::min) {
    throw std::invalid_argument{"These parameters can't be less than 0"};
  }
  if (beta > Simulation::Data::max || gamma > Simulation::Data::max) { throw std::invalid_argument{"Beta and gamma can't be more than 1"}; }
  if (std::cin.fail()) { throw std::invalid_argument{"These parameters have to be numbers"}; }
  if (initial_state.S == Simulation::Data::min && initial_state.I == Simulation::Data::min && initial_state.R == Simulation::Data::min) {
    throw std::invalid_argument{"These parameters can't all be 0"};
  }
  Simulation::Population population{beta, gamma, initial_state};
  return population;
}

int main()
{
  while (1) {
    try {
      Simulation::Population initial_population{get_parameter()};
      int simulation_t;
      std::cin >> simulation_t;
      if (simulation_t <= Simulation::Data::min) { throw std::invalid_argument{"Time has to be more than 0"}; }
      if (std::cin.fail()) { throw std::invalid_argument{"These parameters have to be numbers"}; }

      std::vector<Simulation::Population> population{Simulation::Simulate(simulation_t, initial_population)};
      Simulation::Print(population);
      // Graphing
      sf::RenderWindow w_graph(sf::VideoMode(400, 400), "SIR Graph");
      w_graph.clear(sf::Color::White);
      Display::print_R(w_graph, population);
      Display::print_S(w_graph, population);
      Display::print_I(w_graph, population);
      Display::print_axis(w_graph, population);
      while (w_graph.isOpen()) {
        sf::Event event;
        while (w_graph.waitEvent(event)) {
          if (event.type == sf::Event::Closed) w_graph.close();
          if (event.type == sf::Event::Resized) {
            w_graph.clear(sf::Color::White);
            Display::print_R(w_graph, population);
            Display::print_S(w_graph, population);
            Display::print_I(w_graph, population);
            Display::print_axis(w_graph, population);
            break;
          }
        }
      }
      break;
    } catch (std::invalid_argument const& e) {
      std::cerr << "\033[31mInavalid input:\033[0m " << e.what() << '\n';
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } catch (std::runtime_error const& e) {
      std::cerr << "\033[31mError:\033[0m " << e.what() << '\n';
      return EXIT_FAILURE;
    } catch (...) {
      std::cerr << "Caught unknown exception" << '\n';
      return EXIT_FAILURE;
    }
  }

  return 0;
}