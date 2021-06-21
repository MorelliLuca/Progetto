#include "graph.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "plague.hpp"

namespace Display {
constexpr int Origin{0};                    // Coordinate sia x che y dell'origine
constexpr int Grid_line_width{1};           // Spessore della linea che separa le celle
constexpr int Margin{10};                   // Margini della finetra delle opzioni
constexpr int Font_dimension{15};           // Dimensione del font nella finestra delle opzioni
constexpr int Box_size{35};                 // Dimensione dei box usati come pulsanti
constexpr double Graph_line_width{1};       // Spessore della linea utilizzata per gli assi
constexpr double Graph_line_length{10};     // Lunghezza della linea utilizzata per gli assi
constexpr double Graph_font_dimension{10};  // Dimensione del font
constexpr int Axis_division{10};            // Numero di divisioni delle scale degli assi
constexpr int Rot_angle{90};                // Angolo utilizzato per le rotazioni
constexpr int Initial_size{7};              // Altezza prima colonna
constexpr int Correction_size{3};           // Fattore correttivo delle colonne sucessive alla prima

// Funzione che calcola la dimensione di una cella
double person_size(sf::RenderWindow const& window, Simulation::World const& world)
{
  return window.getSize().x / world.get_side();
}
// Funzione che visualizza graficamente la popolazione
void print(sf::RenderWindow& window, Simulation::World const& world)
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
// Funzione che consente con il mouse di cambiare lo stato di una persona
void set_status(sf::RenderWindow& window, Simulation::World& world, sf::Event& event)
{
  if (event.type == sf::Event::MouseButtonPressed) {
    if (event.mouseButton.button == sf::Mouse::Left) {
      double person_s = person_size(window, world);
      sf::Vector2i local_position = sf::Mouse::getPosition(window);
      // Conversione dalla posizione in pixel a quella sulla griglia
      int r = static_cast<int>(local_position.x / person_s);
      int c = static_cast<int>(local_position.y / person_s);

      bool good_position = (r >= Origin && c >= Origin && r < world.get_side() && c < world.get_side());  // Controllo che il mouse si trovi dentro alla finestra
      // Variazione da S->I, I->R, R->E e E->S
      if (good_position) {
        if (world.person(r, c) == Simulation::Person::S) {
          world.person(r, c) = Simulation::Person::I;
        } else if (world.person(r, c) == Simulation::Person::I) {
          world.person(r, c) = Simulation::Person::R;
        } else if (world.person(r, c) == Simulation::Person::R) {
          world.person(r, c) = Simulation::Person::E;
        } else if (world.person(r, c) == Simulation::Person::E) {
          world.person(r, c) = Simulation::Person::S;
        }
      }
    }
  }
}

void opt_screen_print(sf::RenderWindow& window, Simulation::World& world)  // Stampa la finestra delle opzioni
{
  window.clear(sf::Color::White);
  sf::Font font;
  if (!font.loadFromFile("font.ttf")) {
    throw std::runtime_error{"Font hasn't been loaded correctly"};  // Errore causato dal mancato caricamento del font
  }
  sf::Text text;
  text.setFont(font);
  double spacing{static_cast<double>(window.getSize().y) / 4};  // Calcolo dei distanziamenti delle righe su cui appaiono le opzioni
  text.setFillColor(sf::Color::Black);
  text.setCharacterSize(Font_dimension);
  // Stampa testi
  text.setPosition(sf::Vector2f(Margin, Margin + spacing / 2 - Font_dimension));
  text.setString("Masks");
  window.draw(text);
  text.move(sf::Vector2f(0, spacing));
  text.setString("Lockdown");
  window.draw(text);
  text.move(sf::Vector2f(0, spacing));
  text.setString("Vaccination");
  window.draw(text);
  // Stampa pulsanti
  sf::RectangleShape box;
  box.setSize(sf::Vector2f(Box_size, Box_size));
  box.setPosition(sf::Vector2f(window.getSize().x - Box_size - Margin, Margin));
  text.setPosition(sf::Vector2f(window.getSize().x - Box_size / 2 - Margin - Font_dimension, Margin + (Box_size - Font_dimension) / 2));
  // Controlli sugli stati dei pulsanti
  if (world.mask_status() == Simulation::Mask::ON) {
    box.setFillColor(sf::Color::Green);
    text.setString("ON");
  } else {
    box.setFillColor(sf::Color::Red);
    text.setString("OFF");
  }
  window.draw(box);
  window.draw(text);

  box.move(sf::Vector2f(0, spacing));
  text.move(sf::Vector2f(0, spacing));

  if (world.lockdown_status() == Simulation::Lockdown::ON) {
    box.setFillColor(sf::Color::Green);
    text.setString("ON");
  } else {
    box.setFillColor(sf::Color::Red);
    text.setString("OFF");
  }
  window.draw(box);
  window.draw(text);

  box.move(sf::Vector2f(0, spacing));
  text.move(sf::Vector2f(0, spacing));

  if (world.vax_status() == Simulation::Vax::ON) {
    box.setFillColor(sf::Color::Blue);
    text.setString("ON");
  } else {
    box.setFillColor(sf::Color::Red);
    text.setString("OFF");
  }
  window.draw(box);
  window.draw(text);
  // Pulsante di stop della simulazione
  box.setPosition(sf::Vector2f(Margin, window.getSize().y - Margin - Box_size));
  text.setPosition(sf::Vector2f((window.getSize().x - Font_dimension) / 2, window.getSize().y - Margin - Box_size / 2 - Font_dimension / 2));
  text.setString("STOP");
  box.setFillColor(sf::Color::Red);
  box.setSize(sf::Vector2f(window.getSize().x - 2 * Margin, Box_size));
  window.draw(box);
  window.draw(text);

  window.display();
}

void option(Simulation::World& world, sf::Event& event, sf::RenderWindow& window)  // Funzione che consente l'interazione con i pulsanti
{
  sf::Vector2f mouse_position{sf::Mouse::getPosition(window)};
  if (event.type == sf::Event::MouseButtonPressed) {
    if (event.mouseButton.button == sf::Mouse::Left) {
      double spacing{static_cast<double>(window.getSize().y) / 4};
      // Oggetti che consentono di calcolare con una funzione di sfml se la posizione del mouse
      // si trova nell'area dei pulsanti
      sf::Rect<float> mask_box(window.getSize().x - Box_size - Margin, Margin, Box_size, Box_size);
      sf::Rect<float> lockdown_box(window.getSize().x - Box_size - Margin, Margin + spacing, Box_size, Box_size);
      sf::Rect<float> vax_box(window.getSize().x - Box_size - Margin, Margin + 2 * spacing, Box_size, Box_size);
      sf::Rect<float> stop_box(Margin, window.getSize().y - Margin - Box_size, window.getSize().x - 2 * Margin, Box_size);
      // Controllo sulla posizione del mouse
      if (mask_box.contains(mouse_position)) {
        world.change_mask();
      } else if (lockdown_box.contains(mouse_position)) {
        world.change_lockdown();
      } else if (vax_box.contains(mouse_position)) {
        world.start_vax();
      } else if (stop_box.contains(mouse_position)) {
        window.close();
      }
      // Aggiornamento finestra
      opt_screen_print(window, world);
    }
  }
}
void print_I(sf::RenderWindow& window, std::vector<Simulation::World> const& data_vector)  // Stampa del grafico di I
{
  double epsilon_x{(window.getSize().x) / static_cast<double>(data_vector.size())};                                     // Dimensione in pixel di un giorno
  double epsilon_y{(window.getSize().y) / static_cast<double>(data_vector[0].get_side() * data_vector[0].get_side())};  // Dimensione in pixel di una persona
  sf::RectangleShape line(sf::Vector2f(epsilon_x, Origin));                                                             // Colonna con spessore ridotto per un piccolo deltax
  line.setPosition(sf::Vector2f(Origin, window.getSize().y));                                                           // Posizione iniziale del primo dato del grafico
  line.setFillColor(sf::Color::Red);                                                                                    // Colore del grafico
  // Stampa di una colonna per ogni elemento del vettore nella sua rispettiva posizione
  for (auto it = data_vector.begin(); it != data_vector.end(); ++it) {
    if (it != data_vector.begin() && it->get_I() <= (it - 1)->get_I())  // Dimensioni della colonna se il dato precedente era maggiore o uguale
    {
      line.setSize(sf::Vector2f(
          epsilon_x,
          Correction_size + epsilon_y * ((it - 1)->get_I() - it->get_I())));  // l'altezza è data da 3 pixel più la differenza tra gli infetti, in modo da ridurre gli spazi bianchi
    } else if (it != data_vector.begin() && it->get_I() > (it - 1)->get_I())  // Dimensioni della colonna se il dato precedente era minore
    {
      line.setSize(sf::Vector2f(epsilon_x, Correction_size + epsilon_y * (it->get_I() - (it - 1)->get_I())));
    } else {
      line.setSize(sf::Vector2f(epsilon_x, Initial_size));  // Dimensioni della prima colonna
    }

    line.move(sf::Vector2f(Origin, -it->get_I() * epsilon_y - line.getSize().y));
    window.draw(line);
    // Correzione dovuta al fatto che l'origine del piano per sfml è in alto a sinistra e non in basso a sinistra
    // Così facendo resetto la posizione sull'asse dell y della colonna per agevolare il movimento verso il giorno successivo
    line.move(sf::Vector2f(epsilon_x, it->get_I() * epsilon_y + line.getSize().y));
  }
  window.display();
}
// Le due funzioni a seguire stampano il grafico di S e R e funzionano in maniera del tutto analoga a quella precedente
void print_S(sf::RenderWindow& window, std::vector<Simulation::World> const& data_vector)
{
  double epsilon_x{(window.getSize().x) / static_cast<double>(data_vector.size())};
  double epsilon_y{window.getSize().y / static_cast<double>(data_vector[0].get_side() * data_vector[0].get_side())};
  sf::RectangleShape line(sf::Vector2f(epsilon_x, Origin));
  line.setPosition(sf::Vector2f(Origin, window.getSize().y));
  line.setFillColor(sf::Color::Green);
  for (auto it = data_vector.begin(); it != data_vector.end(); ++it) {
    if (it != data_vector.begin() && it->get_S() <= (it - 1)->get_S()) {
      line.setSize(sf::Vector2f(epsilon_x, Correction_size + epsilon_y * ((it - 1)->get_S() - it->get_S())));
    } else if (it != data_vector.begin() && it->get_S() > (it - 1)->get_S()) {
      line.setSize(sf::Vector2f(epsilon_x, Correction_size + epsilon_y * (it->get_S() - (it - 1)->get_S())));
    } else {
      line.setSize(sf::Vector2f(epsilon_x, Initial_size));
    }
    line.move(sf::Vector2f(Origin, -it->get_S() * epsilon_y - line.getSize().y));
    window.draw(line);
    line.move(sf::Vector2f(epsilon_x, it->get_S() * epsilon_y + line.getSize().y));
  }
  window.display();
}
void print_R(sf::RenderWindow& window, std::vector<Simulation::World> const& data_vector)
{
  double epsilon_x{(window.getSize().x) / static_cast<double>(data_vector.size())};
  double epsilon_y{window.getSize().y / static_cast<double>(data_vector[0].get_side() * data_vector[0].get_side())};
  sf::RectangleShape line(sf::Vector2f(epsilon_x, Origin));
  line.setPosition(sf::Vector2f(Origin, window.getSize().y));
  line.setFillColor(sf::Color::Cyan);
  for (auto it = data_vector.begin(); it != data_vector.end(); ++it) {
    if (it != data_vector.begin() && it->get_R() <= (it - 1)->get_R()) {
      line.setSize(sf::Vector2f(epsilon_x, Correction_size + epsilon_y * ((it - 1)->get_R() - it->get_R())));
    } else if (it != data_vector.begin() && it->get_R() > (it - 1)->get_R()) {
      line.setSize(sf::Vector2f(epsilon_x, Correction_size + epsilon_y * (it->get_R() - (it - 1)->get_R())));
    } else {
      line.setSize(sf::Vector2f(epsilon_x, Initial_size));
    }
    line.move(sf::Vector2f(Origin, -it->get_R() * epsilon_y - line.getSize().y));
    window.draw(line);
    line.move(sf::Vector2f(epsilon_x, it->get_R() * epsilon_y + line.getSize().y));
  }
  window.display();
}

void print_V(sf::RenderWindow& window, std::vector<Simulation::World> const& data_vector)
{
  double epsilon_x{(window.getSize().x) / static_cast<double>(data_vector.size())};
  double epsilon_y{window.getSize().y / static_cast<double>(data_vector[0].get_side() * data_vector[0].get_side())};
  sf::RectangleShape line(sf::Vector2f(epsilon_x, Origin));
  line.setPosition(sf::Vector2f(Origin, window.getSize().y));
  line.setFillColor(sf::Color::Blue);
  for (auto it = data_vector.begin(); it != data_vector.end(); ++it) {
    if (it != data_vector.begin() && it->get_V() <= (it - 1)->get_V()) {
      line.setSize(sf::Vector2f(epsilon_x, Correction_size + epsilon_y * ((it - 1)->get_V() - it->get_V())));
    } else if (it != data_vector.begin() && it->get_V() > (it - 1)->get_V()) {
      line.setSize(sf::Vector2f(epsilon_x, Correction_size + epsilon_y * (it->get_V() - (it - 1)->get_V())));
    } else {
      line.setSize(sf::Vector2f(epsilon_x, Initial_size));
    }
    line.move(sf::Vector2f(Origin, -it->get_V() * epsilon_y - line.getSize().y));
    window.draw(line);
    line.move(sf::Vector2f(epsilon_x, it->get_V() * epsilon_y + line.getSize().y));
  }
  window.display();
}
void print_axis(sf::RenderWindow& window, std::vector<Simulation::World> const& data_vector)  // Stampa degli assi del grafico
{
  sf::RectangleShape line(sf::Vector2f(Graph_line_length, Graph_line_width));  // Linea utilizzata per la scala
  // Caricamento del font utilizzato per stampare le etichette degli assi
  sf::Font font;
  if (!font.loadFromFile("font.ttf")) {
    throw std::runtime_error{"Font was not loaded correctly"};
  }  // Controllo sul corretto caricamento del font
  // Creazione dell'etichetta e impostazione di questa
  sf::Text label;
  label.setFont(font);
  label.setCharacterSize(Graph_font_dimension);
  label.setFillColor(sf::Color::Black);
  line.setFillColor(sf::Color::Black);
  label.setString("N");  // Etichetta asse y
  label.setPosition(sf::Vector2f(Origin, Origin));
  window.draw(label);
  // Stampa della scala asse Y
  for (int i{1}; i < Axis_division; ++i) {
    line.setPosition(sf::Vector2f(Origin, i * window.getSize().y / Axis_division));
    window.draw(line);
    label.setString(std::to_string(data_vector[0].get_side() * data_vector[0].get_side() / Axis_division * (Axis_division - i)));
    label.setPosition(sf::Vector2f(Graph_line_length, i * window.getSize().y / Axis_division - Graph_font_dimension / 2));
    window.draw(label);
  }
  label.setString("0");  // Etichetta origine
  label.setPosition(sf::Vector2f(Origin, window.getSize().y - Graph_font_dimension));
  window.draw(label);
  line.rotate(Rot_angle);  // Rotazione di 90° necessaria per stmpare la scala sull'asse x
  // Stampa della scalla asse x
  for (int i{1}; i < Axis_division; ++i) {
    line.setPosition(sf::Vector2f((i * (window.getSize().x) / Axis_division), window.getSize().y - Graph_line_length));
    window.draw(line);
    label.setString(std::to_string(data_vector.size() / Axis_division * i));
    label.setPosition(sf::Vector2f((i * (window.getSize().x) / Axis_division - Graph_font_dimension / 2), window.getSize().y - Graph_line_length - Graph_font_dimension));
    window.draw(label);
  }
  label.setString("Day");  // Etichetta asse x
  label.setPosition(sf::Vector2f(window.getSize().x - Graph_line_length - Graph_font_dimension, window.getSize().y - Graph_font_dimension));
  window.draw(label);
  window.display();
}

void print_graph(sf::RenderWindow& window, std::vector<Simulation::World> const& data_vector)  // Stampa del grafico
{
  window.clear(sf::Color::White);
  print_V(window, data_vector);
  print_S(window, data_vector);
  print_R(window, data_vector);
  print_I(window, data_vector);
  print_axis(window, data_vector);
}
}  // namespace Display
