
#include "graph.hpp"
#include <algorithm>
#include <vector>
#include <SFML/Graphics.hpp>
#include "sir.hpp"




namespace Display {
constexpr double Origin{0};
constexpr double Margin{10};
constexpr double Line_width{1};
constexpr double Line_length{10};
constexpr double Font_dimension{10};
constexpr int Axis_division{10};
void print_I(sf::RenderWindow& window, std::vector<Simulation::Population> const& data_vector)
{
  double epsilon_x{window.getSize().x / static_cast<double>(data_vector.size())};
  double epsilon_y{window.getSize().y / static_cast<double>(data_vector[0].Total())};
  sf::RectangleShape column_bar(sf::Vector2f(epsilon_x, Origin));
  column_bar.setPosition(sf::Vector2f(Origin, window.getSize().y));
  column_bar.setFillColor(sf::Color::Red);
  for (auto it = data_vector.begin(); it != data_vector.end(); ++it) {
    column_bar.setSize(sf::Vector2f(epsilon_x, it->I() * epsilon_y));
    column_bar.move(sf::Vector2f(Origin, -it->I() * epsilon_y));
    window.draw(column_bar);
    column_bar.move(sf::Vector2f(epsilon_x, it->I() * epsilon_y));
  }
  window.display();
}
void print_S(sf::RenderWindow& window, std::vector<Simulation::Population> const& data_vector)
{
  double epsilon_x{window.getSize().x / static_cast<double>(data_vector.size())};
  double epsilon_y{window.getSize().y / static_cast<double>(data_vector[0].Total())};
  sf::RectangleShape column_bar(sf::Vector2f(epsilon_x, Origin));
  column_bar.setPosition(sf::Vector2f(Origin, window.getSize().y));
  column_bar.setFillColor(sf::Color::Green);
  for (auto it = data_vector.begin(); it != data_vector.end(); ++it) {
    column_bar.setSize(sf::Vector2f(epsilon_x, it->S() * epsilon_y));
    column_bar.move(sf::Vector2f(Origin, -it->S() * epsilon_y));
    window.draw(column_bar);
    column_bar.move(sf::Vector2f(epsilon_x, it->S() * epsilon_y));
  }
  window.display();
}
void print_R(sf::RenderWindow& window, std::vector<Simulation::Population> const& data_vector)
{
  double epsilon_x{window.getSize().x / static_cast<double>(data_vector.size())};
  double epsilon_y{window.getSize().y / static_cast<double>(data_vector[0].Total())};
  sf::RectangleShape column_bar(sf::Vector2f(epsilon_x, Origin));
  column_bar.setPosition(sf::Vector2f(Origin, window.getSize().y));
  column_bar.setFillColor(sf::Color::Cyan);
  for (auto it = data_vector.begin(); it != data_vector.end(); ++it) {
    column_bar.setSize(sf::Vector2f(epsilon_x, it->R() * epsilon_y));
    column_bar.move(sf::Vector2f(Origin, -it->R() * epsilon_y));
    window.draw(column_bar);
    column_bar.move(sf::Vector2f(epsilon_x, it->R() * epsilon_y));
  }
  window.display();
}
void print_axis(sf::RenderWindow& window, std::vector<Simulation::Population> const& data_vector)
{
  sf::RectangleShape line(sf::Vector2f(Line_length, Line_width));
  sf::Font font;
  if (!font.loadFromFile("font.ttf")) { throw std::runtime_error{"Font was not loaded correctly"}; }
  sf::Text label;
  label.setFont(font);
  label.setCharacterSize(Font_dimension);
  label.setFillColor(sf::Color::Black);
  line.setFillColor(sf::Color::Black);
  label.setString("N");
  window.draw(label);
  for (int i{1}; i < Axis_division; ++i) {
    line.setPosition(sf::Vector2f(Origin, i * window.getSize().y / Axis_division));
    window.draw(line);
    label.setString(std::to_string(data_vector[0].Total() / Axis_division * (Axis_division - i)));
    label.setPosition(sf::Vector2f(Line_length, i * window.getSize().y / Axis_division - Font_dimension/2));
    window.draw(label);
  }
  label.setString("0");
  label.setPosition(sf::Vector2f(Origin, window.getSize().y - Font_dimension));
  window.draw(label);
  line.rotate(90);

  for (int i{1}; i < Axis_division; ++i) {
    line.setPosition(sf::Vector2f(i * window.getSize().x / Axis_division, window.getSize().y - Line_length));
    window.draw(line);
    label.setString(std::to_string(data_vector.size() / Axis_division * i));
    label.setPosition(sf::Vector2f(i * window.getSize().x / Axis_division - Font_dimension/2, window.getSize().y - Line_length-Font_dimension));
    window.draw(label);
  }
  label.setString("Day");
  label.setPosition(sf::Vector2f(window.getSize().x - Line_length-Font_dimension, window.getSize().y - Font_dimension));
  window.draw(label);
  window.display();
}
}  // namespace Display