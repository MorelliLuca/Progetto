
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
  double epsilon_x{(window.getSize().x-100) / static_cast<double>(data_vector.size())};      // Dimensione in pixel di un giorno
  double epsilon_y{(window.getSize().y) / static_cast<double>(data_vector[0].Total())};  // Dimensione in pixel di una persona
  sf::RectangleShape line(sf::Vector2f(epsilon_x, Origin));                      // Colonna con spessore ridotto per un piccolo deltax
  line.setPosition(sf::Vector2f(Origin+100, window.getSize().y));                    // Pozione iniziale del primo dato del grafico
  line.setFillColor(sf::Color::Red);  // Colore del grafico                                        
  // Stampa di una colonna per ogni elemento del vettore nella sua rispettiva posizione
  for (auto it = data_vector.begin(); it != data_vector.end(); ++it) {
    if(it!=data_vector.begin()&&it->I()<=(it-1)->I()) //spessore della colonna se il dato precedente era maggiore o uguale
    {
      line.setSize(sf::Vector2f(epsilon_x, 3+epsilon_y*((it-1)->I()-it->I()))); //lo spessore è dato da 3 pixel più la differenza tra gli infetti, in modo da evitare spazi bianchi
    }
    else{
      if(it!=data_vector.begin()&&it->I()>(it-1)->I()) //spessore della colonna se il dato precedente era minore
    {
      line.setSize(sf::Vector2f(epsilon_x, 3+epsilon_y*(it->I()-(it-1)->I())));
    }
    else{
      line.setSize(sf::Vector2f(epsilon_x, 7));//spessore della prima colonna
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
  line.setPosition(sf::Vector2f(Origin+100, window.getSize().y));
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
  line.setPosition(sf::Vector2f(Origin+100, window.getSize().y));
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
  label.setPosition(sf::Vector2f(100, Origin));
  window.draw(label);
  // Stampa della scala asse Y
  for (int i{1}; i < Axis_division; ++i) {
    line.setPosition(sf::Vector2f(Origin+100, i * window.getSize().y / Axis_division));
    window.draw(line);
    label.setString(std::to_string(data_vector[0].Total() / Axis_division * (Axis_division - i)));
    label.setPosition(sf::Vector2f(Line_length+100, i * window.getSize().y / Axis_division - Font_dimension / 2));
    window.draw(label);
  }
  label.setString("0");  // Etichetta origine
  label.setPosition(sf::Vector2f(Origin+100, window.getSize().y - Font_dimension));
  window.draw(label);
  line.rotate(Rot_angle);  // Rotazione di 90° necessaria per stmpare la scala sull'asse x
                           // Stampa della scalla asse x
  for (int i{1}; i < Axis_division; ++i) {
    line.setPosition(sf::Vector2f(100+(i * (window.getSize().x-100) / Axis_division), window.getSize().y - Line_length));
    window.draw(line);
    label.setString(std::to_string(data_vector.size() / Axis_division * i));
    label.setPosition(sf::Vector2f(100+(i * (window.getSize().x-100) / Axis_division - Font_dimension / 2), window.getSize().y - Line_length - Font_dimension));
    window.draw(label);
  }
  label.setString("Day");  // Etichetta asse x
  label.setPosition(sf::Vector2f(100+window.getSize().x - Line_length - Font_dimension, window.getSize().y - Font_dimension));
  window.draw(label);
  window.display();
}
void print_caption (sf::RenderWindow& window, std::vector<Simulation::Population> const& data_vector)
{
  sf::Font font;
  if (!font.loadFromFile("font.ttf")) {
    throw std::runtime_error{"Font was not loaded correctly"};
  }
  sf::RectangleShape S_box(sf::Vector2f(70, 10)); //box color verde con outline nero per i suscettibili
  S_box.setPosition(sf::Vector2f(14, 40));
  S_box.setFillColor(sf::Color::Green);
  S_box.setOutlineThickness(1);
S_box.setOutlineColor(sf::Color::Black);
   window.draw(S_box);
  sf::Text label_S; //etichetta suscettibili
  label_S.setFont(font);
  label_S.setCharacterSize(Font_dimension);
  label_S.setFillColor(sf::Color::Black);
  label_S.setString("Susceptible");  
  float label_S_width = label_S.getLocalBounds().width;
  label_S.setPosition(sf::Vector2f(14+(70-label_S_width)/2, 23)); //serve a centrare la scritta sopra il box
  window.draw(label_S);
  sf::RectangleShape I_box(sf::Vector2f(70, 10)); //box color rosso con outline nero per gli infetti
  I_box.setPosition(sf::Vector2f(14, 120));
  I_box.setFillColor(sf::Color::Red);
  I_box.setOutlineThickness(1);
I_box.setOutlineColor(sf::Color::Black);
   window.draw(I_box);
  sf::Text label_I; //etichetta infetti
  label_I.setFont(font);
  label_I.setCharacterSize(Font_dimension);
  label_I.setFillColor(sf::Color::Black);
  label_I.setString("Infected");
  float label_I_width = label_I.getLocalBounds().width;
  label_I.setPosition(sf::Vector2f(14+(70-label_I_width)/2, 103));
  window.draw(label_I);
  sf::RectangleShape R_box(sf::Vector2f(70, 10)); //box ciano con outline nero per i rimossi
  R_box.setPosition(sf::Vector2f(14, 200));
  R_box.setFillColor(sf::Color::Cyan);
  R_box.setOutlineThickness(1);
R_box.setOutlineColor(sf::Color::Black);
   window.draw(R_box);
  sf::Text label_R; //etichetta rimossi
  label_R.setFont(font);
  label_R.setCharacterSize(Font_dimension);
  label_R.setFillColor(sf::Color::Black);
  label_R.setString("Removed");
  float label_R_width = label_R.getLocalBounds().width;
  label_R.setPosition(sf::Vector2f(14+(70-label_R_width)/2, 183));
  window.draw(label_R);
  sf::RectangleShape N_box(sf::Vector2f(70, 10)); //box bianco con outline nero per il numero totale di persone
  N_box.setPosition(sf::Vector2f(14, 280));
  N_box.setFillColor(sf::Color::White);
  N_box.setOutlineThickness(1);
N_box.setOutlineColor(sf::Color::Black);
   window.draw(N_box);
  sf::Text label_N; //etichetta persone totali 
  label_N.setFont(font);
  label_N.setCharacterSize(Font_dimension);
  label_N.setFillColor(sf::Color::Black);
  label_N.setString("Total people");
  label_N.setPosition(sf::Vector2f(13, 263));
  window.draw(label_N);
  sf::Text label_N_box; //numero di persone totali che compare dentro al box
  label_N_box.setFont(font);
  label_N_box.setCharacterSize(Font_dimension);
  label_N_box.setFillColor(sf::Color::Black);
  label_N_box.setString(std::to_string(data_vector[0].Total())); //serve a centrare la scritta dentro al box
  float label_N_box_width = label_N_box.getLocalBounds().width;
  label_N_box.setPosition(sf::Vector2f(14+(70-label_N_box_width)/2, 280));
  window.draw(label_N_box);
  sf::RectangleShape D_box(sf::Vector2f(70, 10));  //box bianco con outline nero per il numero di giorni
  D_box.setPosition(sf::Vector2f(14, 360));
  D_box.setFillColor(sf::Color::White);
  D_box.setOutlineThickness(1);
D_box.setOutlineColor(sf::Color::Black);
   window.draw(D_box);
  sf::Text label_D; //etichetta giorni
  label_D.setFont(font);
  label_D.setCharacterSize(Font_dimension);
  label_D.setFillColor(sf::Color::Black);
  label_D.setString("Days");
  float label_D_width = label_D.getLocalBounds().width;
  label_D.setPosition(sf::Vector2f(14+(70-label_D_width)/2, 343));
  window.draw(label_D);
  sf::Text label_D_box; //numero di giorni che compare dentro al box
  label_D_box.setFont(font);
  label_D_box.setCharacterSize(Font_dimension);
  label_D_box.setFillColor(sf::Color::Black);
  label_D_box.setString(std::to_string(data_vector.size()-1));
  float label_D_box_width = label_D_box.getLocalBounds().width;
  label_D_box.setPosition(sf::Vector2f(14+(70-label_D_box_width)/2, 360));
  window.draw(label_D_box);
  sf::RectangleShape line (sf::Vector2f(Line_width, window.getSize().y));
  line.setPosition(sf::Vector2f(99, Origin));
  line.setFillColor(sf::Color::Black);
  window.draw(line);
  window.display();
}
}  // namespace Display