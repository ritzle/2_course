#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

#include "Cell.h"

class GameManager {
 private:
  sf::RenderWindow& window;
  std::vector<std::vector<std::shared_ptr<Cell>>> field;
  int n;
  int bombCount;
  int flagInBombCount;
  int cellSize;
  int yOffset;
  bool gameOver;
  bool win;
  sf::RectangleShape resetButton;
  sf::Text bombCounterText;
  sf::Font font;

 public:
  GameManager(sf::RenderWindow& window, int n, int bombCount, int cellSize,
              int yOffset);
  void restartGame();
  void placeBombs();
  void populateNumbers();
  void handleEvent(const sf::Event& event);
  void handleLeftClick(int x, int y);
  void handleRightClick(int x, int y);
  void openAdjacentCells(int x, int y);
  void openAllBombs(int x, int y);
  void checkWinCondition();
  void updateBombCounterText();
  void drawWinText(sf::RenderWindow& window);
  void run();
};

#include "../source/gameManager.cpp"

#endif  // GAMEMANAGER_H
