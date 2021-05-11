
#include "graph.hpp"
#include <algorithm>
#include <vector>
#include <SFML/Graphics.hpp>
#include "sir.hpp"

namespace Display {
void print_I(sf::RenderWindow& window, std::vector<Simulation::Population> const& data_vector)
{
  double epsilon_x{window.getSize().x / static_cast<double>(data_vector.size())};
  double epsilon_y{window.getSize().y / static_cast<double>(data_vector[0].Total())};
  sf::RectangleShape column_bar(sf::Vector2f(epsilon_x, 0));
  column_bar.setPosition(sf::Vector2f(0, window.getSize().y));
  column_bar.setFillColor(sf::Color::Red);
  for (auto it = data_vector.begin(); it != data_vector.end(); ++it) {
    column_bar.setSize(sf::Vector2f(epsilon_x, it->I() * epsilon_y));
    column_bar.move(sf::Vector2f(0, -it->I() * epsilon_y));
    window.draw(column_bar);
    column_bar.move(sf::Vector2f(epsilon_x, it->I() * epsilon_y));
  }
  window.display();
}
void print_S(sf::RenderWindow& window, std::vector<Simulation::Population> const& data_vector)
{
  double epsilon_x{window.getSize().x / static_cast<double>(data_vector.size())};
  double epsilon_y{window.getSize().y / static_cast<double>(data_vector[0].Total())};
  sf::RectangleShape column_bar(sf::Vector2f(epsilon_x, 0));
  column_bar.setPosition(sf::Vector2f(0, window.getSize().y));
  column_bar.setFillColor(sf::Color::Green);
  for (auto it = data_vector.begin(); it != data_vector.end(); ++it) {
    column_bar.setSize(sf::Vector2f(epsilon_x, it->S() * epsilon_y));
    column_bar.move(sf::Vector2f(0, -it->S() * epsilon_y));
    window.draw(column_bar);
    column_bar.move(sf::Vector2f(epsilon_x, it->S() * epsilon_y));
  }
  window.display();
}
void print_R(sf::RenderWindow& window, std::vector<Simulation::Population> const& data_vector)
{
  double epsilon_x{window.getSize().x / static_cast<double>(data_vector.size())};
  double epsilon_y{window.getSize().y / static_cast<double>(data_vector[0].Total())};
  sf::RectangleShape column_bar(sf::Vector2f(epsilon_x, 0));
  column_bar.setPosition(sf::Vector2f(0, window.getSize().y));
  column_bar.setFillColor(sf::Color::Cyan);
  for (auto it = data_vector.begin(); it != data_vector.end(); ++it) {
    column_bar.setSize(sf::Vector2f(epsilon_x, it->R() * epsilon_y));
    column_bar.move(sf::Vector2f(0, -it->R() * epsilon_y));
    window.draw(column_bar);
    column_bar.move(sf::Vector2f(epsilon_x, it->R() * epsilon_y));
  }
  window.display();
}
void print_axis(sf::RenderWindow& window, std::vector<Simulation::Population> const& data_vector)
{
  sf::RectangleShape line(sf::Vector2f(10, 1));
  sf::Font font;
  if (!font.loadFromFile("font.ttf")) { throw std::runtime_error{"Font was not loaded correctly"}; }
  sf::Text label;
  label.setFont(font);
  label.setCharacterSize(10);
  label.setFillColor(sf::Color::Black);
  line.setFillColor(sf::Color::Black);
  label.setString("N");
  window.draw(label);
  for (int i{1}; i < 10; ++i) {
    line.setPosition(sf::Vector2f(0, i * window.getSize().y / 10));
    window.draw(line);
    label.setString(std::to_string(data_vector[0].Total() / 10 * (10 - i)));
    label.setPosition(sf::Vector2f(10, i * window.getSize().y / 10 - 5));
    window.draw(label);
  }
  label.setString("0");
  label.setPosition(sf::Vector2f(0, window.getSize().y - 10));
  window.draw(label);
  line.rotate(90);

  for (int i{1}; i < 10; ++i) {
    line.setPosition(sf::Vector2f(i * window.getSize().x / 10, window.getSize().y - 10));
    window.draw(line);
    label.setString(std::to_string(data_vector.size() / 10 * i));
    label.setPosition(sf::Vector2f(i * window.getSize().x / 10 - 7, window.getSize().y - 21));
    window.draw(label);
  }
  label.setString("Day");
  label.setPosition(sf::Vector2f(window.getSize().x - 25, window.getSize().y - 15));
  window.draw(label);
  window.display();
}
}  // namespace Display