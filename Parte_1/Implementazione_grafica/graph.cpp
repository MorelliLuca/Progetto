
#include "graph.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include "sir.hpp"

namespace Display {
constexpr double Origin{0};           // Posizione sia x che y dell'origine
constexpr double Line_width{1};       // Spessore della linea utilizzata per gli assi
constexpr double Line_length{10};     // Lunghezza della linea utilizzata per gli assi
constexpr double Font_dimension{10};  // Dimensione del font
constexpr int Axis_division{10};      // Numero di divisioni delle scale degli assi
constexpr int Rot_angle{90};          // Angolo utilizzato per le rotazioni

void print_I(sf::RenderWindow& window, std::vector<Simulation::Population> const& data_vector)  // Stampa del grafico di I
{
  double epsilon_x{window.getSize().x / static_cast<double>(data_vector.size())};      // Dimensione in pixel di un giorno
  double epsilon_y{window.getSize().y / static_cast<double>(data_vector[0].Total())};  // Dimensione in pixel di una persona
  sf::RectangleShape line(sf::Vector2f(epsilon_x, Origin));                      // Colonna che costituisce l'area sottesa al grafico per un piccolo deltax
  line.setPosition(sf::Vector2f(Origin, window.getSize().y));                    // Pozione iniziale del primo dato del grafico
  line.setFillColor(sf::Color::Red);  // Colore del grafico                                        
  // Stampa di una colonna per ogni elemento del vettore nella sua rispettiva posizione
  //0.1 0.04 500 600 70 no
  for (auto it = data_vector.begin(); it != data_vector.end(); ++it) {
    if(it!=data_vector.begin()&&it->I()<=(it-1)->I())
    {
      line.setSize(sf::Vector2f(epsilon_x, 3+epsilon_y*((it-1)->I()-it->I())));
    }
    else{
      if(it!=data_vector.begin()&&it->I()>(it-1)->I())
    {
      line.setSize(sf::Vector2f(epsilon_x, 3+epsilon_y*(it->I()-(it-1)->I())));
    }
    else{
      line.setSize(sf::Vector2f(epsilon_x, 7));
    }  
    }
      line.move(sf::Vector2f(Origin, -it->I() * epsilon_y-line.getSize().y));
    window.draw(line);
    // Correzione dovuta al fatto che l'origine del piano per sfml è in alto a sinitra e non in basso a sinistra
    // Così facendo resetto la posizione sull'asse dell y della colonna per agevolare il movimento verso il giorno successivo
     line.move(sf::Vector2f(epsilon_x, it->I()*epsilon_y+line.getSize().y));
  }
  window.display();
}
// Le due funzioni a seguire stampano il grafico di S e R e funzionano in maniera del tutto analoga a quella precedente
void print_S(sf::RenderWindow& window, std::vector<Simulation::Population> const& data_vector)
{
  double epsilon_x{window.getSize().x / static_cast<double>(data_vector.size())};
  double epsilon_y{window.getSize().y / static_cast<double>(data_vector[0].Total())};
  sf::RectangleShape line(sf::Vector2f(epsilon_x, Origin));
  line.setPosition(sf::Vector2f(Origin, window.getSize().y));
  line.setFillColor(sf::Color::Green);
  for (auto it = data_vector.begin(); it != data_vector.end(); ++it) {
    if(it!=data_vector.begin()&&it->S()<=(it-1)->S())
    {
      line.setSize(sf::Vector2f(epsilon_x, 3+epsilon_y*((it-1)->S()-it->S())));
    }
    else{
      if(it!=data_vector.begin()&&it->S()>(it-1)->S())
    {
      line.setSize(sf::Vector2f(epsilon_x, 3+epsilon_y*(it->S()-(it-1)->S())));
    }
    else{
      line.setSize(sf::Vector2f(epsilon_x, 7));
    }  
    }
    line.move(sf::Vector2f(Origin, -it->S() * epsilon_y-line.getSize().y));
    window.draw(line);
    line.move(sf::Vector2f(epsilon_x, it->S() * epsilon_y+line.getSize().y));
  }
  window.display();
}
void print_R(sf::RenderWindow& window, std::vector<Simulation::Population> const& data_vector)
{
  double epsilon_x{window.getSize().x / static_cast<double>(data_vector.size())};
  double epsilon_y{window.getSize().y / static_cast<double>(data_vector[0].Total())};
  sf::RectangleShape line(sf::Vector2f(epsilon_x, Origin));
  line.setPosition(sf::Vector2f(Origin, window.getSize().y));
  line.setFillColor(sf::Color::Cyan);
  for (auto it = data_vector.begin(); it != data_vector.end(); ++it) {
    if(it!=data_vector.begin()&&it->R()<=(it-1)->R())
    {
      line.setSize(sf::Vector2f(epsilon_x, 3+epsilon_y*((it-1)->R()-it->R())));
    }
    else{
      if(it!=data_vector.begin()&&it->R()>(it-1)->R())
    {
      line.setSize(sf::Vector2f(epsilon_x, 3+epsilon_y*(it->R()-(it-1)->R())));
    }
    else{
      line.setSize(sf::Vector2f(epsilon_x, 7));
    }  
    }
    line.move(sf::Vector2f(Origin, -it->R() * epsilon_y-line.getSize().y));
    window.draw(line);
    line.move(sf::Vector2f(epsilon_x, it->R() * epsilon_y+line.getSize().y));
  }
  window.display();
}
void print_axis(sf::RenderWindow& window, std::vector<Simulation::Population> const& data_vector)  // Stampa degli assi del grafico
{
  sf::RectangleShape line(sf::Vector2f(Line_length, Line_width));  // Linea utilizzata per la scala
  // Caricamento del font utilizzato per stampare le etichette degli assi
  sf::Font font;
  if (!font.loadFromFile("font.ttf")) {
    throw std::runtime_error{"Font was not loaded correctly"};
  }  // Controllo sul corretto caricamento del font
  // Creazione dell'etichetta e impostazione di questa
  sf::Text label;
  label.setFont(font);
  label.setCharacterSize(Font_dimension);
  label.setFillColor(sf::Color::Black);
  line.setFillColor(sf::Color::Black);
  label.setString("N");  // Etichetta asse y
  window.draw(label);
  // Stampa della scala asse Y
  for (int i{1}; i < Axis_division; ++i) {
    line.setPosition(sf::Vector2f(Origin, i * window.getSize().y / Axis_division));
    window.draw(line);
    label.setString(std::to_string(data_vector[0].Total() / Axis_division * (Axis_division - i)));
    label.setPosition(sf::Vector2f(Line_length, i * window.getSize().y / Axis_division - Font_dimension / 2));
    window.draw(label);
  }
  label.setString("0");  // Etichetta origine
  label.setPosition(sf::Vector2f(Origin, window.getSize().y - Font_dimension));
  window.draw(label);
  line.rotate(Rot_angle);  // Rotazione di 90° necessaria per stmpare la scala sull'asse x
                           // Stampa della scalla asse x
  for (int i{1}; i < Axis_division; ++i) {
    line.setPosition(sf::Vector2f(i * window.getSize().x / Axis_division, window.getSize().y - Line_length));
    window.draw(line);
    label.setString(std::to_string(data_vector.size() / Axis_division * i));
    label.setPosition(sf::Vector2f(i * window.getSize().x / Axis_division - Font_dimension / 2, window.getSize().y - Line_length - Font_dimension));
    window.draw(label);
  }
  label.setString("Day");  // Etichetta asse x
  label.setPosition(sf::Vector2f(window.getSize().x - Line_length - Font_dimension, window.getSize().y - Font_dimension));
  window.draw(label);
  window.display();
}
}  // namespace Display