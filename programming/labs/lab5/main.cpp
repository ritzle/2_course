// #include <SFML/Graphics.hpp>
// #include <cstdlib>
// #include <ctime>
// #include <iostream>
// #include <map>
// #include <memory>
// #include <vector>

// using namespace std;

// bool gameOver = false;

// // class Cell {
// //  protected:
// //   bool isCovered;  // Закрыта ли ячейка
// //   bool isFlagged;  // Есть ли флаг

// //  public:
// //   Cell() : isCovered(true), isFlagged(false) {}
// //   virtual ~Cell() = default;

// //   virtual void draw(sf::RenderWindow& window, int x, int y, int cellSize,
// //                     int yOffset) = 0;
// //   virtual bool isBomb() const = 0;
// //   virtual int getValue() const = 0;

// //   void toggleFlag() { isFlagged = !isFlagged; }
// //   void uncover() { isCovered = isFlagged ? isCovered : false; }
// //   bool isCellCovered() const { return isCovered; }
// //   bool hasFlag() const { return isFlagged; }
// // };

// // class BombCell : public Cell {
// //  public:
// //   bool explosion = false;

// //   void draw(sf::RenderWindow& window, int x, int y, int cellSize,
// //             int yOffset) override {
// //     sf::RectangleShape cell(sf::Vector2f(cellSize - 1, cellSize - 1));
// //     cell.setPosition(x * cellSize, y * cellSize + yOffset);
// //     cell.setOutlineThickness(1);
// //     cell.setOutlineColor(sf::Color::Black);

// //     if (isCovered) {
// //       cell.setTexture(hasFlag()
// //                           ? &TextureManager::getTexture(TextureType::Flag)
// //                           :
// // &TextureManager::getTexture(TextureType::Covered));
// //     } else if (explosion == 0) {
// //       cell.setTexture(&TextureManager::getTexture(TextureType::Bomb));
// //     } else {
// // cell.setTexture(&TextureManager::getTexture(TextureType::BombExplotion));
// //     }

// //     window.draw(cell);
// //   }

// //   bool isBomb() const override { return true; }
// //   int getValue() const override { return -1; }
// // };

// // class NumberCell : public Cell {
// //  private:
// //   int value;  // количество соседних бомб

// //  public:
// //   explicit NumberCell(int value) : value(value) {}

// //   void draw(sf::RenderWindow& window, int x, int y, int cellSize,
// //             int yOffset) override {
// //     sf::RectangleShape cell(sf::Vector2f(cellSize - 1, cellSize - 1));
// //     cell.setPosition(x * cellSize, y * cellSize + yOffset);
// //     cell.setOutlineThickness(1);
// //     cell.setOutlineColor(sf::Color::Black);

// //     if (isCovered) {
// //       cell.setTexture(hasFlag()
// //                           ? &TextureManager::getTexture(TextureType::Flag)
// //                           :
// // &TextureManager::getTexture(TextureType::Covered));
// //     } else if (value == 0) {
// //       cell.setTexture(&TextureManager::getTexture(TextureType::Empty));
// //     } else if (value > 0 && value <= 8) {
// //       cell.setTexture(&TextureManager::getTexture(
// //           static_cast<TextureType>(static_cast<int>(TextureType::One) +
// value
// //           -
// //                                    1)));  // бомба - 0, числа начинаются с
// 1
// //     }

// //     window.draw(cell);
// //   }

// //   bool isBomb() const override { return false; }
// //   int getValue() const override { return value; }
// // };

// // class CellFactory {
// //  public:
// //   static shared_ptr<Cell> createCell(bool isBomb, int value = 0) {
// //     if (isBomb) {
// //       return make_shared<BombCell>();
// //     } else {
// //       return make_shared<NumberCell>(value);
// //     }
// //   }
// // };

// map<TextureType, sf::Texture> TextureManager::textures;

// // Функция для установки бомб
// void placeBombs(vector<vector<shared_ptr<Cell>>>& field, int n, int
// bombCount) {
//   int bombsPlaced = 0;
//   while (bombsPlaced < bombCount) {
//     int x = rand() % n;
//     int y = rand() % n;

//     if (!field[y][x]) {  // Если ячейка ещё не занята
//       field[y][x] = CellFactory::createCell(true);
//       bombsPlaced++;
//     }
//   }
// }

// // Функция для подсчета соседних бомб и установки числовых ячеек
// void populateNumbers(vector<vector<shared_ptr<Cell>>>& field, int n) {
//   for (int i = 0; i < n; ++i) {
//     for (int j = 0; j < n; ++j) {
//       if (!field[i][j]) {
//         int bombsAround = 0;

//         // Подсчёт соседних бомб
//         for (int dy = -1; dy <= 1; ++dy) {
//           for (int dx = -1; dx <= 1; ++dx) {
//             int nx = j + dx;
//             int ny = i + dy;
//             if (nx >= 0 && nx < n && ny >= 0 && ny < n && field[ny][nx] &&
//                 field[ny][nx]->isBomb()) {
//               bombsAround++;
//             }
//           }
//         }

//         field[i][j] = CellFactory::createCell(false, bombsAround);
//       }
//     }
//   }
// }

// // FIXME надо как-то по другому оргонизовать создание кнопока
// sf::RectangleShape createButton(int width, int height, int x, int y,
//                                 sf::Color color) {
//   sf::RectangleShape button(sf::Vector2f(width, height));
//   button.setPosition(x, y);
//   button.setFillColor(color);
//   button.setOutlineThickness(2);
//   button.setOutlineColor(sf::Color::Black);
//   return button;
// }

// sf::RectangleShape createButton(int width, int height, int x, int y) {
//   sf::RectangleShape button(sf::Vector2f(width, height));
//   button.setPosition(x, y);
//   button.setTexture(gameOver == 0
//                         ? &TextureManager::getTexture(TextureType::Smile)
//                         :
//                         &TextureManager::getTexture(TextureType::SmileRip));
//   button.setOutlineThickness(2);
//   button.setOutlineColor(sf::Color::Black);
//   return button;
// }

// void restartGame(vector<vector<shared_ptr<Cell>>>& field, int n,
//                  int countBobm) {
//   field.clear();
//   field.resize(n, vector<shared_ptr<Cell>>(n));

//   placeBombs(field, n, countBobm);
//   populateNumbers(field, n);
//   gameOver = false;
// }

// void revealAllBombs(vector<vector<shared_ptr<Cell>>>& field, int n) {
//   for (int i = 0; i < n; ++i) {
//     for (int j = 0; j < n; ++j) {
//       if (field[i][j]->isBomb()) {
//         field[i][j]->uncover();  // Открываем все бомбы
//       }
//     }
//   }
// }

// void openAdjacentCells(vector<vector<shared_ptr<Cell>>>& field, int i, int j,
//                        int n) {
//   // Пропускаем ячейки вне поля, открытые или с флагами
//   if (i < 0 || i >= n || j < 0 || j >= n || !field[i][j]->isCellCovered() ||
//       field[i][j]->hasFlag())
//     return;

//   // Открываем текущую ячейку
//   field[i][j]->uncover();

//   // Проверяем, является ли ячейка с числом
//   auto numberCell = dynamic_cast<NumberCell*>(field[i][j].get());
//   if (numberCell) {
//     // Если число больше 0, рекурсия не нужна
//     if (numberCell->getValue() > 0) return;
//   }

//   // Если число == 0, открываем соседние клетки рекурсивно
//   for (int dy = -1; dy <= 1; ++dy) {
//     for (int dx = -1; dx <= 1; ++dx) {
//       // Пропускаем центральную ячейку
//       if (dx == 0 && dy == 0) continue;

//       int nx = j + dx;  // Новая координата X
//       int ny = i + dy;  // Новая координата Y

//       openAdjacentCells(field, ny, nx, n);
//     }
//   }
// }

// void updateBombCounterText(sf::Text& bombCounterText,
//                            const vector<vector<shared_ptr<Cell>>>& field, int
//                            n, int countBomb) {
//   int flagsCount = 0;
//   for (int i = 0; i < n; ++i) {
//     for (int j = 0; j < n; ++j) {
//       if (field[i][j]->hasFlag()) {
//         ++flagsCount;
//       }
//     }
//   }
//   int remainingBombs = countBomb - flagsCount;
//   bombCounterText.setString("Bombs left: " + std::to_string(remainingBombs));
// }

// void gameLoop(sf::RenderWindow& window, vector<vector<shared_ptr<Cell>>>&
// field,
//               int n, int cellSize, int yOffset, int countBomb) {
//   // Создаём кнопку перезапуска
//   sf::RectangleShape button =
//       createButton(cellSize * 2, 50, cellSize * n / 2 - cellSize, 5);

//   // Создаём текст для отображения оставшихся бомб
//   sf::Font font;
//   if (!font.loadFromFile("/home/vlad/vsCode/2_course/programming/labs/lab5/"
//                          "font/TeletactileRus.ttf")) {
//     throw runtime_error("Не удалось загрузить шрифт!");
//   }

//   sf::Text bombCounterText;
//   bombCounterText.setFont(font);
//   bombCounterText.setCharacterSize(20);
//   bombCounterText.setFillColor(sf::Color::Black);
//   bombCounterText.setPosition(10, 10);

//   // Инициализация текста с количеством оставшихся бомб
//   updateBombCounterText(bombCounterText, field, n, countBomb);

//   while (window.isOpen()) {
//     sf::Event event;
//     while (window.pollEvent(event)) {
//       if (event.type == sf::Event::Closed) {
//         window.close();
//       }

//       if (event.type == sf::Event::MouseButtonPressed) {
//         int mouseX = event.mouseButton.x;
//         int mouseY = event.mouseButton.y;

//         // Проверяем нажатие на кнопку
//         if (button.getGlobalBounds().contains(mouseX, mouseY)) {
//           restartGame(field, n, countBomb);
//           button.setTexture(&TextureManager::getTexture(TextureType::Smile));
//           updateBombCounterText(bombCounterText, field, n, countBomb);
//           continue;
//         }

//         if (gameOver) {
//           continue;
//         }

//         // Проверяем нажатие на игровое поле
//         int x = mouseX / cellSize;
//         int y = (mouseY - yOffset) / cellSize;  // костыль со смещением

//         if (x >= 0 && x < n && y >= 0 && y < n) {
//           if (event.mouseButton.button == sf::Mouse::Left) {
//             // Проверяем, не стоит ли флаг на клетке
//             if (field[y][x]->hasFlag()) {
//               continue;
//             } else {
//               if (field[y][x]->isBomb()) {
//                 static_cast<BombCell*>(field[y][x].get())->explosion = true;
//                 revealAllBombs(field, n);
//                 // gameOver = true;
//                 button.setTexture(
//                     &TextureManager::getTexture(TextureType::SmileRip));
//               } else {
//                 openAdjacentCells(field, y, x, n);
//               }
//             }
//           } else if (event.mouseButton.button == sf::Mouse::Right) {
//             if (!field[y][x]->isCellCovered()) {
//               continue;
//             } else {
//               // Переключение флага на клетке
//               field[y][x]->toggleFlag();
//               updateBombCounterText(bombCounterText, field, n, countBomb);
//             }
//           }
//         }
//       }
//     }

//     window.clear(sf::Color::White);

//     // Отображаем кнопку перезапуска
//     window.draw(button);

//     // Отображаем игровое поле
//     for (int i = 0; i < n; ++i) {
//       for (int j = 0; j < n; ++j) {
//         field[i][j]->draw(window, j, i, cellSize, yOffset);
//       }
//     }

//     // Отображаем текст с количеством оставшихся бомб
//     window.draw(bombCounterText);

//     window.display();
//   }
// }

// #define BOMB_COUNT 20

// int main(int argc, char* argv[]) {
//   int n = 10;          // Размер поля (по умолчанию)
//   int bombCount = 10;  // Количество бомб (по умолчанию)

//   if (argc == 3) {
//     try {
//       n = stoi(argv[1]);
//       bombCount = stoi(argv[2]);
//       if (n <= 0 || bombCount <= 0 || bombCount >= n * n) {
//         throw invalid_argument("Некорректные параметры!");
//       }
//     } catch (const exception& e) {
//       cerr << "Ошибка: " << e.what() << endl;
//       cerr << "Используются значения по умолчанию: поле 10x10, бомб 10."
//            << endl;
//     }
//   } else if (argc != 1) {
//     cerr
//         << "Неверное количество параметров. Используются значения по
//         умолчанию."
//         << endl;
//   }

//   const int fixedWindowWidth = 600;  // Фиксированная ширина окна
//   const int fixedWindowHeight = 600;  // Фиксированная высота окна
//   int yOffset = 60;  // Смещение игрового поля вниз

//   // Рассчитываем размер клетки и размеры окна
//   int cellSize =
//       std::min(fixedWindowWidth / n, (fixedWindowHeight - yOffset) / n);
//   const int windowWidth = cellSize * n;
//   const int windowHeight = cellSize * n + yOffset;

//   TextureManager::loadTextures();

//   sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight),
//                           "Minesweeper");

//   srand(static_cast<unsigned>(time(0)));

//   vector<vector<shared_ptr<Cell>>> field(n, vector<shared_ptr<Cell>>(n));

//   placeBombs(field, n, bombCount);
//   populateNumbers(field, n);

//   gameLoop(window, field, n, cellSize, yOffset, bombCount);

//   return 0;
// }

#include <SFML/Graphics.hpp>

#include "/home/vlad/vsCode/2_course/programming/labs/lab5/headers/gameManager.h"

map<TextureType, sf::Texture> TextureManager::textures;

int main(int argc, char* argv[]) {
  int n = 10;          // Размер поля (10x10)
  int bombCount = 10;  // Количество бомб

  if (argc == 3) {
    try {
      n = stoi(argv[1]);
      bombCount = stoi(argv[2]);
      if (n <= 0 || bombCount <= 0 || bombCount > n * n) {
        throw invalid_argument("Некорректные параметры!");
      }
    } catch (const exception& e) {
      cerr << "Ошибка: " << e.what() << endl;
      cerr << "Используются значения по умолчанию: поле 10x10, бомб 10."
           << endl;
    }
  } else if (argc != 1) {
    cerr << "Неверное количество параметров. Используются значения по умолчанию"
         << endl;
  }

  srand(static_cast<unsigned>(time(0)));

  const int fixedWindowWidth = 600;  // Фиксированная ширина окна
  const int fixedWindowHeight = 600;  // Фиксированная высота окна
  int yOffset = 60;  // Смещение игрового поля вниз

  // Рассчитываем размер клетки и размеры окна
  int cellSize =
      std::min(fixedWindowWidth / n, (fixedWindowHeight - yOffset) / n);
  const int windowWidth = cellSize * n;
  const int windowHeight = cellSize * n + yOffset;

  // Создание окна
  sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight),
                          "Minesweeper");

  // Создание менеджера игры
  GameManager gameManager(window, n, bombCount, cellSize, yOffset);

  // Запуск игры
  gameManager.run();

  return 0;
}
