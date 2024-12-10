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
