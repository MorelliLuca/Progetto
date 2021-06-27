#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <thread>
#include <vector>
#include "graph.hpp"
#include "plague.hpp"

constexpr int sleep_time{750};       // Tempo trascorso tra la simulazione di un giorno e l'altro
constexpr int Window_side{400};      // Dimensione finestra grafica
constexpr int Window_opt_side{200};  // Dimensione finestra grafica

// Funzione che prende in input i parametri iniziali della simulazione
Simulation::World get_parameter()
{
  int side;
  double beta, gamma, theta;
  std::cin >> side >> beta >> gamma >> theta;
  // Controlli della coerenza dei dati inseriti con il modello
  if (std::cin.fail()) {
    throw std::invalid_argument{"These parameters have to be numbers"};
  }
  if (beta < Simulation::World::Data_min || gamma < Simulation::World::Data_min || theta < Simulation::World::Data_min) {
    throw std::invalid_argument{"Beta gamma, and theta can't be less than 0"};
  }
  if (side <= Simulation::World::Data_min) {
    throw std::invalid_argument{"Dimension of the world has to be more than 0"};
  }
  if (beta > Simulation::World::Beta_Gamma_Max || gamma > Simulation::World::Beta_Gamma_Max || theta > Simulation::World::Beta_Gamma_Max) {
    throw std::invalid_argument{"Beta, gamma and theta can't be more than 1"};
  }
  Simulation::World parameters{side, beta, gamma, theta};
  return parameters;
}

int main()
{
  
  while (1) {
    try {
      std::cout << "Insert the side of the world, beta, gamma and theta: ";
      Simulation::World world{get_parameter()};                                                 // Inizializazione del mondo nella sua configurazione iniziale
      sf::RenderWindow grid_screen(sf::VideoMode(Window_side, Window_side), "SIR Simulation");  // Finstra in cui è rappresentata la griglia
      Display::print(grid_screen, world);                                                       // Visualizazione a finestra della configurazione iniziale
      int day{0};                                                                               // Contatore dei giorni già simulati
      std::vector<Simulation::World> world_history;                                             // Vettore di tutti gli stati del mondo generati per poter stampare il grafico
      sf::RenderWindow graph_screen(sf::VideoMode(Window_side, Window_side), "History Graph");  // Finestra del grafico
      sf::Event event;                                                                          // Evento utilizzato per rilevare la chiusura della finestra grafica e i click
      // Ciclo che impedisce che il programma termini automaticamente prima della chiusura della finestra
      while (grid_screen.isOpen()) {
        Display::print(grid_screen, world);  // Visualizazione dello stato modificato della popolazione
        // Inizio Simulazione
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {  // Rilevazione della perssione del tasto enter per iniziare la simulazione
          // Inizio stampa della tabella
          Simulation::print_intestation(world);                                                                  // Stampa intestazione
          Simulation::print_terminal(world, day);                                                                // Stampa dati iniziali
          world_history.push_back(world);                                                                        // Aggiunta al vettore lo stato iniziale
          sf::RenderWindow opt_screen(sf::VideoMode(Window_opt_side, Window_opt_side), "Option control panel");  // Finestra delle opzioni
          Display::opt_screen_print(opt_screen, world);
          // Ciclo che continua la simulazione fino alla pressione di esc o alla chisura della finestra
          while (opt_screen.isOpen() && grid_screen.isOpen()) {
            // Funzione che gestisce il random walk se non c'è il lockdown
            if (world.lockdown_status() == Simulation::Lockdown::OFF) {
              Simulation::walk(world);
            }
            // Evoluzione della simulazione di un giorno
            ++day;  // Variazione del giorno
            Simulation::World next = Simulation::evolve(world);
            Display::print(grid_screen, next);  // Visualizazione delle variazioni graficamente
            Simulation::print_terminal(next, day);              // Stampa a terminale
            world_history.push_back(next);                      // Aggiunta al vettore del nuovo stato
            Display::print_graph(graph_screen, world_history);  // Stampa del grafico
            world = next;
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
            // Ciclo degli eventi della finestra delle opzioni
            while (opt_screen.pollEvent(event)) {
              if (event.type == sf::Event::Closed) {  // Chiususra della finesta delle opzioni
                opt_screen.close();
              }
              Display::option(world, event, opt_screen);  // Controllo sulla variazione delle opzioni
            }
            // Ciclo di controllo degli eventi della finestra con la griglia
            while (grid_screen.pollEvent(event)) {
              if (event.type == sf::Event::Closed) {  // Chiusura del programma
                opt_screen.close();
                grid_screen.close();
                graph_screen.close();
              }
            }
          }
        }
        // Ciclo di controllo degli eventi della finestra con la griglia
        while (grid_screen.pollEvent(event)) {
          if (event.type == sf::Event::Closed) {  // Chiusura del programma
            grid_screen.close();
            graph_screen.close();
          }
          Display::set_status(grid_screen, world, event);  // Funzione che permette di cambiare lo stato di una persona con il mouse
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
