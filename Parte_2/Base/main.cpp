#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <thread>
#include "graph.hpp"
#include "plague.hpp"

constexpr int sleep_time{1000};  // Tempo trascorso tra la simulazione di un giorno e l'altro
constexpr int Max_input{1};      // Valore massimo per l'input di gamma e beta
constexpr int Min_input{0};      // Valore minimo degli input
constexpr int Window_side{400};  // Dimensione finestra grafica

Simulation::World get_parameter()
{  // Funzione che prende in input i parametri iniziali della simulazione
   int side;
   double beta, gamma;
   std::cin >> side >> beta >> gamma;
      // Controlli della coerenza dei dati inseriti con il modello
      if (beta < Min_input || gamma < Min_input) {
         throw std::invalid_argument{"Beta and gamma can't be less than 0"};
   }
      if (side <= Min_input) {
         throw std::invalid_argument{"This parameter has to be more than 0"};
   }
      if (beta > Max_input || gamma > Max_input) {
         throw std::invalid_argument{"Beta and gamma can't be more than 1"};
   }
      if (std::cin.fail()) {
         throw std::invalid_argument{"These parameters have to be numbers"};
   }
   Simulation::World parameters{side, beta, gamma};
   return parameters;
}

int main()
{
   std::cout << "Insert the side of the world, beta and gamma: ";
      while (1) {
            try {
               Simulation::World world{get_parameter()};  // Inizializzazione del mondo nella sua configurazione iniziale
               sf::RenderWindow w_grid(sf::VideoMode(Window_side, Window_side), "SIR Simulation");  // Finestra in cui è rappresentata la griglia
               Display::print(w_grid, world);  // Visualizzazione a finestra della configurazione iniziale
               int day{0};                     // Contatore dei giorni già simulati
                  while (w_grid.isOpen()) {  // Ciclo che impedisce che il programma termini automaticamente prima della chiusura della finestra
                     sf::Event event;        // Evento utilizzato per rilevare la chiusura della finestra grafica
                     Display::set_status(w_grid, world);  // Funzione che permette di cambiare lo stato di una persona con il click del mouse
                     Display::print(w_grid, world);       // Visualizzazione dello stato modificato della popolazione
                        if (sf::Keyboard::isKeyPressed(
                                sf::Keyboard::Enter)) {  // Rilevazione della pressione del tasto enter per iniziare la simulazione
                              // Inizio stampa della tabella
                              Simulation::print_intestation(world);
                              Simulation::print_terminal(world, day);
                              while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) &&
                                     w_grid.isOpen()) {  // Ciclo che continua la simulazione fino alla pressione di esc o alla chiusura della finestra
                                 world = Simulation::evolve(world);  // Evoluzione della simulazione di un giorno
                                 Display::print(w_grid, world);      // Visualizazione grafica delle variazioni 
                                 ++day;
                                 Simulation::print_terminal(world, day);  // Stampa a terminale
                                 std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
                                    while (w_grid.pollEvent(event)) {  // Ciclo che consente la chiusura del programma dalla finestra
                                          if (event.type == sf::Event::Closed) {
                                             w_grid.close();
                                       }
                                    }
                              }
                     }

                        while (w_grid.pollEvent(event)) {  // Ciclo che consente la chiusura del programma dalla finestra
                           if (event.type == sf::Event::Closed)
                              w_grid.close();
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