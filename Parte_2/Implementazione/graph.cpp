#include "graph.hpp"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include <thread>
#include "plague.hpp"
#include <SFML/Graphics/Rect.hpp>

namespace Display {
constexpr int Origin{0};                                                            // Coordinate sia x che y dell'origine
constexpr int Grid_line_width{1};                                                   // Spessore della linea che separa le celle
constexpr int Sleep_time{200};                                                      // Tempo di attesa tra la rilevazione di un click e il seguente
constexpr int Margin{10};
constexpr int Font_dimension{15};
constexpr int Box_size{35};

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
      } else if (world.person(r, c) == Simulation::Person::V) {
        square.setFillColor(sf::Color::Blue);
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

  bool good_position = (r >= Origin && c >= Origin && r < world.get_side() && c < world.get_side());  // Controllo che il mouse si trovi dentro alla finestra

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

void opt_screen_print(sf::RenderWindow &window,Simulation::World &world)
{
  window.clear(sf::Color::White);
  sf::Font font;
  if (!font.loadFromFile("font.ttf"))
  {
    throw std::runtime_error{"Font hasn't been loaded correctly"};
  }
  sf::Text text;
  text.setFont(font);
  double spacing{static_cast<double>(window.getSize().y)/4};
  text.setFillColor(sf::Color::Black);
  text.setCharacterSize(Font_dimension);
 
    text.setPosition(sf::Vector2f(Margin,Margin+spacing/2-Font_dimension));
    text.setString("Masks");
    window.draw(text);
    text.move(sf::Vector2f(0,spacing));
    text.setString("Lockdown");
    window.draw(text);
    text.move(sf::Vector2f(0,spacing));
    text.setString("Vaccination");
    window.draw(text);


    sf::RectangleShape box;
    box.setSize(sf::Vector2f(Box_size,Box_size));
    box.setPosition(sf::Vector2f(window.getSize().x-Box_size-Margin,Margin));
    text.setPosition(sf::Vector2f(window.getSize().x-Box_size/2-Margin-Font_dimension,Margin+(Box_size-Font_dimension)/2));
    if(world.mask_status()==Simulation::Mask::ON)
    {
      box.setFillColor(sf::Color::Green);
      text.setString("ON");
    }else {
      box.setFillColor(sf::Color::Red);
      text.setString("OFF");
    }
    window.draw(box);
    window.draw(text);

    box.move(sf::Vector2f(0,spacing));
    text.move(sf::Vector2f(0,spacing));

    if(world.lockdown_status()==Simulation::Lockdown::ON)
    {
      box.setFillColor(sf::Color::Green);
      text.setString("ON");
    }else {
      box.setFillColor(sf::Color::Red);
      text.setString("OFF");
    }
    window.draw(box);
    window.draw(text);

    box.move(sf::Vector2f(0,spacing));
    text.move(sf::Vector2f(0,spacing));

    if(world.vax_status()==Simulation::Vax::ON)
    {
      box.setFillColor(sf::Color::Blue);
      text.setString("ON");
    }else {
      box.setFillColor(sf::Color::Red);
      text.setString("OFF");
    }
    window.draw(box);
    window.draw(text);

box.setPosition(sf::Vector2f(Margin,window.getSize().y-Margin-Box_size));
text.setPosition(sf::Vector2f((window.getSize().x-Font_dimension)/2,window.getSize().y-Margin-Box_size/2-Font_dimension/2));
text.setString("STOP");
box.setFillColor(sf::Color::Red);
box.setSize(sf::Vector2f(window.getSize().x-2*Margin,Box_size));
window.draw(box);
    window.draw(text);
    


  
  

window.display();
  
}

void option(Simulation::World &world,sf::Event &event,sf::RenderWindow &window)
{
  
    sf::Vector2f mouse_position{sf::Mouse::getPosition(window)};
    if(event.type==sf::Event::MouseButtonPressed)
    {
      if(event.mouseButton.button == sf::Mouse::Left)
      {
      
      double spacing{static_cast<double>(window.getSize().y)/4};
      sf::Rect<float> mask_box(window.getSize().x-Box_size-Margin,Margin,Box_size,Box_size);
      sf::Rect<float> lockdown_box(window.getSize().x-Box_size-Margin,Margin+spacing,Box_size,Box_size);
      sf::Rect<float> vax_box(window.getSize().x-Box_size-Margin,Margin+2*spacing,Box_size,Box_size);
      sf::Rect<float> stop_box(Margin,window.getSize().y-Margin-Box_size,window.getSize().x-2*Margin,Box_size);

      if (mask_box.contains(mouse_position))
      {
        world.change_mask();
      }else if (lockdown_box.contains(mouse_position))
      {
        world.change_lockdown();
      }else if (vax_box.contains(mouse_position))
      {
        world.start_vax();
      }else if(stop_box.contains(mouse_position))
      {
        window.close();
      }
      
      opt_screen_print(window,world);
     }
    }


    
  }
}
  // namespace Display