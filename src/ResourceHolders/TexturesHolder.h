#pragma once

#include "../Category.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>


class TexturesHolder
{
public:
  void Init();
  virtual ~TexturesHolder();
  sf::Texture *getTexturePtr(Textures::ID id) { return mResourceMap.find(id)->second; }

private:
  void loadTexture(Textures::ID, const std::string &, sf::IntRect *, size_t aSize = 1);

private:
  std::map<Textures::ID, sf::Texture *> mResourceMap;
};
