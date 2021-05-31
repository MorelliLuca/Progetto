#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <thread>
#include "graph.hpp"
#include "plague.hpp"

constexpr int sleep_time{1000};      // Tempo trascorso tra la simulazione di un giorno e l'altro
constexpr int Window_side{400};      // Dimensione finstra grafica
constexpr int Window_opt_side{200};  // Dimensione finstra grafica

Simulation::World get_parameter()
{  // Funzione che prende in input i parametri iniziali della simulazione
  int side;
  double beta, gamma, theta;
  std::cin >> side >> beta >> gamma >> theta;
  // Controlli della coerenza dei dati inseriti con il modello
  if (beta < Simulation::World::Data_min || gamma < Simulation::World::Data_min || theta < Simulation::World::Data_min) {
    throw std::invalid_argument{"Beta gamma, and theta can't be less than 0"};
  }
  if (side <= Simulation::World::Data_min) {
    throw std::invalid_argument{"Dimension of the world has to be more than 0"};
  }
  if (beta > Simulation::World::Beta_Gamma_Max || gamma > Simulation::World::Beta_Gamma_Max || theta > Simulation::World::Beta_Gamma_Max) {
    throw std::invalid_argument{"Beta and gamma can't be more than 1"};
  }
  if (std::cin.fail()) {
    throw std::invalid_argument{"These parameters have to be numbers"};
  }
  Simulation::World parameters{side, beta, gamma, theta};
  return parameters;
}

int main()
{
  std::cout << "Insert the side of the world, beta, gamma and theta: ";
  while (1) {
    try {
      Simulation::World world{get_parameter()};                                            // Inizializazione del mondo nella sua configurazione iniziale
      sf::RenderWindow w_grid(sf::VideoMode(Window_side, Window_side), "SIR Simulation");  // Finstra in cui è rappresentata la griglia
      Display::print(w_grid, world);                                                       // Visualizazione a finestra della configurazione iniziale
      int day{0};                                                                          // Contatore dei giorni già simulati
      while (w_grid.isOpen()) {                                 // Ciclco che impedisce che il programma termini automaticamente prima della chiusura della finestra
        sf::Event event;                                        // Evento utilizzato per rilevare la chiusura della finestra grafica
        Display::set_status(w_grid, world);                     // Funzione che permette di cambiare lo stato di una persona con il mouse
        Display::print(w_grid, world);                          // Visualizazione dello stato modificato della popolazione
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {  // Rilevazione della perssione del tasto enter per iniziare la simulazione
          // Inizio stampa della tabella
          Simulation::print_intestation(world);
          Simulation::print_terminal(world, day);

          sf::RenderWindow opt_screen(sf::VideoMode(Window_opt_side, Window_opt_side), "Option control panel");
          Display::opt_screen_print(opt_screen, world);
          while (opt_screen.isOpen() && w_grid.isOpen()) {  // Ciclo che continua la simulazione fino alla pressione di esc o alla chisura della finestra
            if (world.lockdown_status() == Simulation::Lockdown::OFF) {
              Simulation::walk(world);
            }

            Simulation::World next = Simulation::evolve(world);  // Evoluzione della simulazione di un giorno
            Display::print(w_grid, next);                        // Visualizazione delle variazioni graficamente
            ++day;
            next.eval_R0(world);

            Simulation::print_terminal(next, day);  // Stampa a terminale
            world = next;
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
            while (opt_screen.pollEvent(event)) {  // Ciclo che consente la chiusura del programma dalla finestra
              if (event.type == sf::Event::Closed) {
                opt_screen.close();
              }

              Display::option(world, event, opt_screen);
            }
            while (w_grid.pollEvent(event)) {
              if (event.type == sf::Event::Closed) {
                opt_screen.close();
                w_grid.close();
              }
            }
          }
        }

        while (w_grid.pollEvent(event)) {  // Ciclo che consente la chiusura del programma dalla finestra
          if (event.type == sf::Event::Closed) w_grid.close();
        }
      }

      break;
    } catch (std::invalid_argument const& e) {  // Errore dovuto ad un input errato
      std::cerr << "\033[31mInavalid input:\033[0m " << e.what() << '\n';
      // Pulizia di tutti i dati inseriti nello stream per permettere una nuova immissione
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