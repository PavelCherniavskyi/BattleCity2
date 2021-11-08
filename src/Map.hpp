#ifndef BATTLECITY_MAP_HPP
#define BATTLECITY_MAP_HPP

#include "Entity.hpp"
#include "Definitions.hpp"
#include "Enums.hpp"

struct Node
{
  Node(const sf::FloatRect& aRect, const sf::Sprite& aSprite, EMapObjects aType)
    : Rect(aRect)
    , Sprite(aSprite)
    , Type(aType)
  {
  }

  sf::FloatRect Rect;
  sf::Sprite Sprite;
  EMapObjects Type;
};

class Map
{
public:
  explicit Map(size_t);
  bool Init();
  void Draw(sf::RenderWindow&);
  void DestroyObject(const std::unordered_multimap<EMapObjects, Node>::const_iterator& it);
  const std::unordered_multimap<EMapObjects, Node>& GetMapObjects() const;

protected:
  std::unordered_multimap<EMapObjects, Node> mLevelMap;
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