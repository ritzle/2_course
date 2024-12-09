#ifndef CELL_H
#define CELL_H

#include <SFML/Graphics.hpp>
#include <memory>

#include "TextureManager.h"
using namespace std;

class Cell {
 protected:
  bool isCovered;  // Закрыта ли ячейка
  bool isFlagged;  // Есть ли флаг

 public:
  Cell() : isCovered(true), isFlagged(false) {}
  virtual ~Cell() = default;

  virtual void draw(sf::RenderWindow& window, int x, int y, int cellSize,
                    int yOffset) = 0;
  virtual bool isBomb() const = 0;
  virtual int getValue() const = 0;

  void toggleFlag() { isFlagged = !isFlagged; }
  void uncover() { isCovered = isFlagged ? isCovered : false; }
  bool isCellCovered() const { return isCovered; }
  bool hasFlag() const { return isFlagged; }
};

class BombCell : public Cell {
 public:
  bool explosion = false;
  bool isFlagged;

  void draw(sf::RenderWindow& window, int x, int y, int cellSize,
            int yOffset) override;

  bool isBomb() const override { return true; }
  int getValue() const override { return -1; }
};

class NumberCell : public Cell {
 private:
  int value;

 public:
  NumberCell(int value) : value(value) {}

  void draw(sf::RenderWindow& window, int x, int y, int cellSize,
            int yOffset) override;

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

#include "../source/Cell.cpp"

#endif
