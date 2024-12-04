#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

using namespace std;

#define BOMB_COUNT 20  // Количество бомб

enum class TextureType {
  Bomb,
  One,
  Two,
  Three,
  Four,
  Five,
  Six,
  Seven,
  Eight,
  Covered,
  Flag,
  Empty
};

class TextureManager {
 private:
  static map<TextureType, sf::Texture> textures;

 public:
  static void loadTextures() {
    loadTexture(
        TextureType::Bomb,
        "/home/vlad/vsCode/2_course/programming/labs/lab5/image/bomb.png");
    loadTexture(TextureType::One,
                "/home/vlad/vsCode/2_course/programming/labs/lab5/image/1.png");
    loadTexture(TextureType::Two,
                "/home/vlad/vsCode/2_course/programming/labs/lab5/image/2.png");
    loadTexture(TextureType::Three,
                "/home/vlad/vsCode/2_course/programming/labs/lab5/image/3.png");
    loadTexture(TextureType::Four,
                "/home/vlad/vsCode/2_course/programming/labs/lab5/image/4.png");
    loadTexture(TextureType::Five,
                "/home/vlad/vsCode/2_course/programming/labs/lab5/image/5.png");
    loadTexture(TextureType::Six,
                "/home/vlad/vsCode/2_course/programming/labs/lab5/image/6.png");
    loadTexture(TextureType::Seven,
                "/home/vlad/vsCode/2_course/programming/labs/lab5/image/7.png");
    loadTexture(TextureType::Eight,
                "/home/vlad/vsCode/2_course/programming/labs/lab5/image/8.png");
    loadTexture(
        TextureType::Covered,
        "/home/vlad/vsCode/2_course/programming/labs/lab5/image/topLayer.png");
    loadTexture(TextureType::Flag,
                "/home/vlad/vsCode/2_course/programming/labs/lab5/image/"
                "topLayerWithFlag.png");
    loadTexture(
        TextureType::Empty,
        "/home/vlad/vsCode/2_course/programming/labs/lab5/image/openLayer.png");
  }

  static const sf::Texture& getTexture(TextureType type) {
    return textures.at(type);
  }

 private:
  static void loadTexture(TextureType type, const string& filename) {
    sf::Texture texture;
    if (!texture.loadFromFile(filename)) {
      cerr << "Failed to load texture: " << filename << endl;
      exit(-1);
    }
    textures[type] = move(texture);
  }
};

class Cell {
 protected:
  bool isCovered;  // Закрыта ли ячейка
  bool isFlagged;  // Есть ли флаг

 public:
  Cell() : isCovered(true), isFlagged(false) {}
  virtual ~Cell() = default;

  virtual void draw(sf::RenderWindow& window, int x, int y, int cellSize) = 0;
  virtual bool isBomb() const = 0;
  virtual int getValue() const = 0;

  void toggleFlag() { isFlagged = !isFlagged; }
  void uncover() { isCovered = isFlagged ? isCovered : false; }
  bool isCellCovered() const { return isCovered; }
  bool hasFlag() const { return isFlagged; }
};

class BombCell : public Cell {
 public:
  void draw(sf::RenderWindow& window, int x, int y, int cellSize) override {
    sf::RectangleShape cell(sf::Vector2f(cellSize - 1, cellSize - 1));
    cell.setPosition(x * cellSize, y * cellSize);

    if (isCovered) {
      cell.setTexture(hasFlag()
                          ? &TextureManager::getTexture(TextureType::Flag)
                          : &TextureManager::getTexture(TextureType::Covered));
    } else {
      cell.setTexture(&TextureManager::getTexture(TextureType::Bomb));
    }

    window.draw(cell);
  }

  bool isBomb() const override { return true; }
  int getValue() const override { return -1; }
};

class NumberCell : public Cell {
 private:
  int value;  // количество соседних бомб

 public:
  explicit NumberCell(int value) : value(value) {}

  void draw(sf::RenderWindow& window, int x, int y, int cellSize) override {
    sf::RectangleShape cell(sf::Vector2f(cellSize - 1, cellSize - 1));
    cell.setPosition(x * cellSize, y * cellSize);

    if (isCovered) {
      cell.setTexture(hasFlag()
                          ? &TextureManager::getTexture(TextureType::Flag)
                          : &TextureManager::getTexture(TextureType::Covered));
    } else if (value == 0) {
      cell.setTexture(&TextureManager::getTexture(
          static_cast<TextureType>(TextureType::Empty)));
    } else if (value > 0 && value <= 8) {
      cell.setTexture(&TextureManager::getTexture(
          static_cast<TextureType>(static_cast<int>(TextureType::One) + value -
                                   1)));  // бомба - 0, числа начинаются с 1
    }

    window.draw(cell);
  }

  bool isBomb() const override { return false; }
  int getValue() const override { return value; }
};

class CellFactory {
 public:
  static shared_ptr<Cell> createCell(bool isBomb, int value = 0) {
    if (isBomb) {
      return make_shared<BombCell>();
    } else {
      return make_shared<NumberCell>(value);
    }
  }
};

map<TextureType, sf::Texture> TextureManager::textures;

int main() {
  const int n = 10;         // Размер поля
  const int cellSize = 50;  // Размер клетки
  const int windowSize = n * cellSize;

  TextureManager::loadTextures();

  sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "Minesweeper");

  srand(static_cast<unsigned>(time(0)));

  vector<vector<shared_ptr<Cell>>> field(n, vector<shared_ptr<Cell>>(n));

  // Ставим бомбы
  int bombsPlaced = 0;
  while (bombsPlaced < BOMB_COUNT) {
    int x = rand() % n;
    int y = rand() % n;

    if (!field[y][x]) {  // Если ячейка ещё не занята
      field[y][x] = CellFactory::createCell(true);
      bombsPlaced++;
    }
  }

  // Заполняем оставшиеся клетки числами
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (!field[i][j]) {
        int bombsAround = 0;

        // Подсчёт соседних бомб
        for (int dy = -1; dy <= 1; ++dy) {
          for (int dx = -1; dx <= 1; ++dx) {
            int nx = j + dx;
            int ny = i + dy;
            if (nx >= 0 && nx < n && ny >= 0 && ny < n && field[ny][nx] &&
                field[ny][nx]->isBomb()) {
              bombsAround++;
            }
          }
        }

        field[i][j] = CellFactory::createCell(false, bombsAround);
      }
    }
  }

  // Главный цикл
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }

      if (event.type == sf::Event::MouseButtonPressed) {
        int x = event.mouseButton.x / cellSize;
        int y = event.mouseButton.y / cellSize;

        if (x >= 0 && x < n && y >= 0 && y < n) {
          if (event.mouseButton.button == sf::Mouse::Left) {
            field[y][x]->uncover();
          } else if (event.mouseButton.button == sf::Mouse::Right) {
            field[y][x]->toggleFlag();
          }
        }
      }
    }

    window.clear(sf::Color::White);

    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        field[i][j]->draw(window, j, i, cellSize);
      }
    }

    window.display();
  }

  return 0;
}
