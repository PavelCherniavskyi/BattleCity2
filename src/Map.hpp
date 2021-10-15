#ifndef BATTLECITY_MAP_HPP
#define BATTLECITY_MAP_HPP

#include "Entity.hpp"
#include "Definitions.hpp"
#include "Enums.hpp"

struct Node
{
  Node(const sf::FloatRect& aRect, const sf::Sprite& aSprite)
    : Rect(aRect)
    , Sprite(aSprite)
  {
  }

  sf::FloatRect Rect;
  sf::Sprite Sprite;
};

class Map
{
public:
  explicit Map(size_t aMapIndex);
  bool Init();
  void Draw(sf::RenderWindow& window);
  std::unordered_multimap<EImage, Node>& GetMap();

protected:
  std::unordered_multimap<EImage, Node> mLevelMap;
  size_t mMapIndex;
};

class Map0 : public Map
{
public:
  Map0();
};

class Map1 : public Map
{
public:
  Map1();
};

class Map2 : public Map
{
public:
  Map2();
};

class Map3 : public Map
{
public:
  Map3();
};

class Map4 : public Map
{
public:
  Map4();
};

#endif// BATTLECITY_MAP_HPP