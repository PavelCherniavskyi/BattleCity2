#ifndef BATTLECITY_PLAYER_HPP
#define BATTLECITY_PLAYER_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics.hpp>
#include "SpriteHolder.hpp"
#include "include/enum.h"

class Player
{
public:
  Player(const SpriteHolder& aSpriteHolder, ESpriteId aSpriteIconId);
  Player(const Player&) = delete;
  Player& operator=(const Player&) = delete;
  void Init();
  void Move(const sf::Vector2f& aOffset);
  void Draw(sf::RenderWindow& aWindow) const;

private:
  const SpriteHolder&         mSpriteHolder;
  ESpriteId                   mSpriteIconId;
  std::unique_ptr<sf::Sprite> mIcon;
};

#endif// BATTLECITY_PLAYER_HPP