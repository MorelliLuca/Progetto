#include <SFML/Graphics.hpp>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>
#include "graph.hpp"
#include "sir.hpp"

constexpr int Graphic_window_side{400}; //Dimensione del lato del grafico
constexpr int Space_caption{100}; //Larghezza della legenda

Simulation::Population get_parameter()
{  // Funzione che prende in input i parametri iniziali della simulazione
  double beta, gamma;
  std::string quarantine = "No";
  Simulation::Data initial_state;
  std::string vax;
  int n_vax{0};
  std::cin >> beta >> gamma >> initial_state.S >> initial_state.I >> initial_state.R >> vax;
 if (vax=="yes") //Controllo che prende il numero di vaccini giornalieri se si vaccina
  {
    std::cout << "Insert number of vaccines per day: ";
    std::cin >> n_vax;
    if (n_vax<=Simulation::Data::min)
    {
      throw std::invalid_argument{"The number of vaccines has to be more than 0"};
    }
  } 
  // Controlli sui dati inseriti
  if (initial_state.S < Simulation::Data::min || initial_state.I < Simulation::Data::min || initial_state.R < Simulation::Data::min || beta < Simulation::Data::min ||
      gamma < Simulation::Data::min) {
    throw std::invalid_argument{"These parameters can't be less than 0"};
  }
  if (beta > Simulation::Data::Max || gamma > Simulation::Data::Max) {
    throw std::invalid_argument{"Beta and gamma can't be more than 1"};
  }
  if (std::cin.fail()) {
    throw std::invalid_argument{"These parameters have to be numbers"};
  }
  if (initial_state.S == Simulation::Data::min && initial_state.I == Simulation::Data::min && initial_state.R == Simulation::Data::min) {
    throw std::invalid_argument{"These parameters can't all be 0"};
  }
  
  Simulation::Population population{beta, gamma, initial_state, quarantine, n_vax};
  return population;
}

int main()
{
  std::cout << "Insert Beta, Gamma, S, I, R, and if people can get vaccinated (yes/no): ";
  while (1) {
    try {
      Simulation::Population initial_population{get_parameter()};  // Inizializiazione della popoalzione iniziale
      int simulation_t, start_vax{0};
      std::cout <<"Insert number of days of simulation: ";         // Tempo di simulazione
      std::cin >> simulation_t;
      // Controllo sul tempo di simulazione inserito
      if (simulation_t <= Simulation::Data::min) {
        throw std::invalid_argument{"Time has to be more than 0"};
      }
      if (std::cin.fail()) {
        throw std::invalid_argument{"These parameters have to be numbers"};
      }
      if (initial_population.N_vax()!=0) //Se si vaccina chiede da che giorno iniziare
      {
        std::cout << "Insert from what day they start vaccinating: ";
        std::cin >> start_vax;
        //Controlli sulla data di inizio vaccinazioni
        if (simulation_t <= start_vax) { 
        throw std::invalid_argument{"You can't start vaccinating after the end of the simulation"};
      }
      if (start_vax < Simulation::Data::min) {
        throw std::invalid_argument{"This has to be more than 0"};
      }
      if (std::cin.fail()) {
        throw std::invalid_argument{"These parameters have to be numbers"};
      }
      }
      // Simulazione
      std::vector<Simulation::Population> population{Simulation::Simulate(simulation_t, initial_population, start_vax)};
      // Stampa a terminale dei risulati della simulazione
      Simulation::Print(population);
      sf::RenderWindow w_graph(sf::VideoMode(Graphic_window_side+Space_caption+5, Graphic_window_side), "SIR Graph");  // Finestra grafica
      w_graph.clear(sf::Color::White);                                                               // Impostazione dello sfondo
      Display::print_R(w_graph, population);                                                         // Stampa grafica del grafico di R
      Display::print_S(w_graph, population);                                                         // Stampa grafica del grafico di S
      Display::print_I(w_graph, population);                                                         // Stampa grafica del grafico di R
      Display::print_axis(w_graph, population);                                                      // Stampa degli assi
      Display::print_caption(w_graph, population);                                                    //Stampa legenda
      while (w_graph.isOpen()) {                                                                     // Ciclo che evita la terminazione automatica del programma a finestra aperta
        sf::Event event;                                                                             // Evento usato per rilevare la chiusura della finestra grafica
        while (w_graph.waitEvent(event)) { 
                                                         // Ciclo utilizzato per chiudere il programma quando viene chiusa la finestra
          if (event.type == sf::Event::Closed) {
            w_graph.close();
          }
        }
      }
      break;
    } catch (std::invalid_argument const& e) {
      std::cerr << "\033[31mInavalid input:\033[0m " << e.what() << '\n';
      // Pulizia di quanto inserito nello stream per poter reinserire i dati
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