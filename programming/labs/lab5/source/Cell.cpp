#include "../headers/Cell.h"

#include <SFML/Graphics.hpp>

// Метод отрисовки для клетки с бомбой
void BombCell::draw(sf::RenderWindow& window, int x, int y, int cellSize,
                    int yOffset) {
  sf::RectangleShape cell(sf::Vector2f(cellSize - 1, cellSize - 1));
  cell.setPosition(x * cellSize, y * cellSize + yOffset);
  cell.setOutlineThickness(1);
  cell.setOutlineColor(sf::Color::Black);

  if (isCovered) {
    cell.setTexture(hasFlag()
                        ? &TextureManager::getTexture(TextureType::Flag)
                        : &TextureManager::getTexture(TextureType::Covered));
  } else if (explosion == 0) {
    cell.setTexture(&TextureManager::getTexture(TextureType::Bomb));
  } else {
    cell.setTexture(&TextureManager::getTexture(TextureType::BombExplotion));
  }

  window.draw(cell);
}

// Метод отрисовки для клетки с числом
void NumberCell::draw(sf::RenderWindow& window, int x, int y, int cellSize,
                      int yOffset) {
  sf::RectangleShape cell(sf::Vector2f(cellSize - 1, cellSize - 1));
  cell.setPosition(x * cellSize, y * cellSize + yOffset);
  cell.setOutlineThickness(1);
  cell.setOutlineColor(sf::Color::Black);

  if (isCovered) {
    cell.setTexture(hasFlag()
                        ? &TextureManager::getTexture(TextureType::Flag)
                        : &TextureManager::getTexture(TextureType::Covered));
  } else if (value == 0) {
    cell.setTexture(&TextureManager::getTexture(TextureType::Empty));
  } else if (value > 0 && value <= 8) {
    cell.setTexture(&TextureManager::getTexture(
        static_cast<TextureType>(static_cast<int>(TextureType::One) + value -
                                 1)));  // бомба - 0, числа начинаются с 1
  }

  window.draw(cell);
}
