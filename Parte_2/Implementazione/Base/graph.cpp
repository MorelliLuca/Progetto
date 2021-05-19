#include "graph.hpp"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include <thread>
#include "plague.hpp"

namespace Display {
constexpr int Origin{0};                                                         // Coordinate sia x che y dell'origine
constexpr int Grid_line_width{1};                                                   // Spessore della linea che separa le celle
constexpr int Sleep_time{200};                                                      // Tempo di attesa tra la rilevazione di un click e il seguente
double person_size(sf::RenderWindow const& window, Simulation::World const& world)  // Funzione che calcola la dimesione di una cella
{
   return window.getSize().x / world.get_side();
}

void print(sf::RenderWindow& window, Simulation::World const& world)  // Funzione che visualizza graficamente la popolazione
{
   double person_s = person_size(window, world);
   window.clear(sf::Color::White);                                                                   // Pulizia della finstra grafica
   sf::RectangleShape square(sf::Vector2f(person_s - Grid_line_width, person_s - Grid_line_width));  // Creazione della forma di una cella
      for (int r{Origin}; r < world.get_side(); ++r) {
            for (int c{Origin}; c < world.get_side(); ++c) {
               square.setPosition(r * (person_s), c * (person_s));  // Posizionamento di un quadrato nella griglia
                  // Impostazione del colore in funzione dello stato della persona
                  if (world.person(r, c) == Simulation::Person::S) {
                     square.setFillColor(sf::Color::Green);
                  } else if (world.person(r, c) == Simulation::Person::I) {
                     square.setFillColor(sf::Color::Red);
                  } else if (world.person(r, c) == Simulation::Person::E) {
                     square.setFillColor(sf::Color::Black);
                  } else {
                     square.setFillColor(sf::Color::White);
                  }
               window.draw(square);
            }
      }
   window.display();
}

void set_status(sf::RenderWindow& window, Simulation::World& world)  // Funzione che consente con il mouse di cambiare lo stato di una persona
{
   double person_s = person_size(window, world);
   sf::Vector2i local_position = sf::Mouse::getPosition(window);
   // Conversione dalla poszione in pixel a quella sulla griglia
   int r = static_cast<int>(local_position.x / person_s);
   int c = static_cast<int>(local_position.y / person_s);

   bool good_position =
       (r >= Origin && c >= Origin && r < world.get_side() && c < world.get_side());  // Controllo che il mouse si trovi dentro alla finestra

      if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && good_position) {  // Variazione da S->I, I->R e R->S
            if (world.person(r, c) == Simulation::Person::S) {
               world.person(r, c) = Simulation::Person::I;
            } else if (world.person(r, c) == Simulation::Person::I) {
               world.person(r, c) = Simulation::Person::R;
            } else if (world.person(r, c) == Simulation::Person::R) {
               world.person(r, c) = Simulation::Person::E;
            } else if (world.person(r, c) == Simulation::Person::E) {
               world.person(r, c) = Simulation::Person::S;
         }
         std::this_thread::sleep_for(std::chrono::milliseconds(Sleep_time));  // Sleep necessario per garantire che sia stato effettuato un click
   }
}

}  // namespace Display