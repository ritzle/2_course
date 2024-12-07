#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <iostream>
#include <map>

using namespace std;

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
  Empty,
  BombExplotion,
  Smile,
  SmileRip
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
    loadTexture(TextureType::BombExplotion,
                "/home/vlad/vsCode/2_course/programming/labs/lab5/image/"
                "bombExplosion.png");
    loadTexture(TextureType::Smile,
                "/home/vlad/vsCode/2_course/programming/labs/lab5/image/"
                "smile.png");
    loadTexture(TextureType::SmileRip,
                "/home/vlad/vsCode/2_course/programming/labs/lab5/image/"
                "smile_rip.png");
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

#endif  // TEXTUREMANAGER_H
