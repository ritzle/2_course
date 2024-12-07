#include "../headers/gameManager.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

GameManager::GameManager(sf::RenderWindow& window, int n, int bombCount,
                         int cellSize, int yOffset)
    : window(window),
      n(n),
      bombCount(bombCount),
      cellSize(cellSize),
      yOffset(yOffset),
      gameOver(false),
      win(false) {
  TextureManager::loadTextures();
  if (!font.loadFromFile("/home/vlad/vsCode/2_course/programming/labs/lab5/"
                         "font/TeletactileRus.ttf")) {
    throw runtime_error("Failed to load font!");
  }

  resetButton.setSize(sf::Vector2f(cellSize * 2, 50));
  resetButton.setPosition(cellSize * n / 2 - cellSize, 5);
  resetButton.setTexture(&TextureManager::getTexture(TextureType::Smile));

  bombCounterText.setFont(font);
  bombCounterText.setCharacterSize(20);
  bombCounterText.setFillColor(sf::Color::Black);
  bombCounterText.setPosition(10, 10);

  restartGame();
}

void GameManager::restartGame() {
  field.clear();
  field.resize(n, vector<shared_ptr<Cell>>(n));
  placeBombs();
  populateNumbers();
  gameOver = false;
  win = false;
  resetButton.setTexture(&TextureManager::getTexture(TextureType::Smile));
  updateBombCounterText();
}

void GameManager::placeBombs() {
  int placedBombs = 0;
  while (placedBombs < bombCount) {
    int x = rand() % n;
    int y = rand() % n;
    if (!field[y][x]) {
      field[y][x] = make_shared<BombCell>();
      ++placedBombs;
    }
  }
}

void GameManager::populateNumbers() {
  for (int y = 0; y < n; ++y) {
    for (int x = 0; x < n; ++x) {
      if (!field[y][x]) {
        int bombCount = 0;
        for (int dy = -1; dy <= 1; ++dy) {
          for (int dx = -1; dx <= 1; ++dx) {
            int ny = y + dy;
            int nx = x + dx;
            if (ny >= 0 && ny < n && nx >= 0 && nx < n && field[ny][nx] &&
                field[ny][nx]->isBomb()) {
              ++bombCount;
            }
          }
        }
        field[y][x] = make_shared<NumberCell>(bombCount);
      }
    }
  }
}

void GameManager::handleEvent(const sf::Event& event) {
  if (event.type == sf::Event::Closed) {
    window.close();
  }
  if (event.type == sf::Event::MouseButtonPressed) {
    int mouseX = event.mouseButton.x;
    int mouseY = event.mouseButton.y;

    if (resetButton.getGlobalBounds().contains(mouseX, mouseY)) {
      restartGame();
      return;
    }
    if (gameOver) return;

    int x = mouseX / cellSize;
    int y = (mouseY - yOffset) / cellSize;

    if (x >= 0 && x < n && y >= 0 && y < n) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        handleLeftClick(x, y);
      } else if (event.mouseButton.button == sf::Mouse::Right) {
        handleRightClick(x, y);
      }
    }
  }
}

void GameManager::handleLeftClick(int x, int y) {
  if (gameOver) return;

  if (field[y][x]->isBomb()) {
    gameOver = true;
    openAllBombs(x, y);
    resetButton.setTexture(&TextureManager::getTexture(TextureType::SmileRip));
  } else {
    openAdjacentCells(x, y);
  }
}

void GameManager::handleRightClick(int x, int y) {
  if (gameOver) return;

  if (!field[y][x]->isCellCovered()) {
    return;
  } else {
    // Переключение флага на клетке
    field[y][x]->toggleFlag();

    if (auto bombCell = dynamic_cast<BombCell*>(field[y][x].get())) {
      // Если флаг установлен, то снимаем его, если нет, то устанавливаем
      if (bombCell->isFlagged) {
        // Снимаем флаг, уменьшаем количество флагов
        bombCell->isFlagged = false;
        flagInBombCount--;
      } else {
        // Устанавливаем флаг, увеличиваем количество флагов
        bombCell->isFlagged = true;
        flagInBombCount++;
      }
    }

    updateBombCounterText();
  }
}

void GameManager::openAdjacentCells(int x, int y) {
  if (x < 0 || x >= n || y < 0 || y >= n || !field[y][x]->isCellCovered() ||
      field[y][x]->hasFlag()) {
    return;
  }

  // Открываем клетку
  shared_ptr<Cell> cell = field[y][x];

  if (cell->isCellCovered() && !cell->isBomb()) {
    cell->uncover();
    if (cell->getValue() == 0) {
      // Если значение 0, раскрываем соседние клетки
      for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
          if (dx == 0 && dy == 0) continue;

          int ny = y + dy;
          int nx = x + dx;

          if (ny >= 0 && ny < n && nx >= 0 && nx < n) {
            if (field[ny][nx] && field[ny][nx]->isCellCovered()) {
              openAdjacentCells(nx, ny);
            }
          }
        }
      }
    }
  } else {
    return;
  }
}

void GameManager::openAllBombs(int x, int y) {
  static_cast<BombCell*>(field[y][x].get())->explosion = true;

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (field[i][j]->isBomb()) {
        field[i][j]->uncover();
      }
    }
  }
}

void GameManager::checkWinCondition() {
  // Проверка на выигрыш
  int uncoveredCells = 0;
  for (int y = 0; y < n; ++y) {
    for (int x = 0; x < n; ++x) {
      if (field[y][x]->isCellCovered() && !field[y][x]->isBomb()) {
        return;
      }
      if (!field[y][x]->isCellCovered() && !field[y][x]->isBomb()) {
        uncoveredCells++;
      }
    }
  }

  if (uncoveredCells == (n * n - bombCount) && flagInBombCount == 0) {
    win = true;
    gameOver = true;
  }
}

// FIXME можно в классе создать поля отвечающие за количество поставленных
// флагов
void GameManager::updateBombCounterText() {
  int flaggedCount = 0;
  for (int y = 0; y < n; ++y) {
    for (int x = 0; x < n; ++x) {
      if (field[y][x] && field[y][x]->hasFlag()) {
        flaggedCount++;
      }
    }
  }

  int remainingBombs = bombCount - flaggedCount;
  flagInBombCount = remainingBombs;
  bombCounterText.setString("Bombs: " + to_string(remainingBombs));
}

void GameManager::drawWinText(sf::RenderWindow& window) {
  if (win) {
    // Загружаем шрифт
    sf::Font font;
    if (!font.loadFromFile("../font/TeletactileRus.ttf")) {
      std::cerr << "Error loading font!" << std::endl;
      return;
    }

    // Создаем текст
    sf::Text winText("You Win!", font, 50);  // Размер шрифта 50
    winText.setFillColor(sf::Color::White);  // Цвет текста - белый

    // Вычисляем размеры текста и прямоугольника
    sf::FloatRect textBounds = winText.getLocalBounds();
    sf::RectangleShape background(
        sf::Vector2f(textBounds.width + 20,
                     textBounds.height + 20));  // Прямоугольник вокруг текста

    // Позиционируем прямоугольник (центрируем по экрану)
    background.setPosition(window.getSize().x / 2 - background.getSize().x / 2,
                           window.getSize().y / 2 - background.getSize().y / 2);
    background.setFillColor(sf::Color::Green);  // Зеленый фон

    // Позиционируем текст (центрируем относительно прямоугольника)
    winText.setPosition(background.getPosition().x +
                            (background.getSize().x - textBounds.width) / 2,
                        background.getPosition().y +
                            (background.getSize().y - textBounds.height) / 2 -
                            20);

    // Отрисовываем прямоугольник и текст
    window.draw(background);
    window.draw(winText);
  }
}

void GameManager::run() {
  sf::Event event;
  while (window.isOpen()) {
    checkWinCondition();
    while (window.pollEvent(event)) {
      handleEvent(event);
    }

    window.clear(sf::Color::White);
    window.draw(resetButton);

    // Рисуем поле
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        field[i][j]->draw(window, j, i, cellSize, yOffset);
      }
    }
    window.draw(bombCounterText);

    if (gameOver) {
      drawWinText(window);
    }

    window.display();
  }
}
