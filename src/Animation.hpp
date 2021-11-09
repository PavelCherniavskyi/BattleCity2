#ifndef BATTLECITY_ANIMATION_HPP
#define BATTLECITY_ANIMATION_HPP

#include "Entity.hpp"
#include "Enums.hpp"

class Animation : public Entity
{
public:
  Animation(const size_t aSpritesCount, const EImage aType);
  void Update(const sf::Vector2f& = sf::Vector2f()) override;
  bool IsLiving();

protected:
  bool setInit(const float aScale, const float aLifeTime);

protected:
  sf::Clock mClockForTimeOfLiving;
  sf::Clock mClockForDraw;
  bool mIsLiving;
  float mTimeOfLiving;
  size_t mSpritesCount;
};

class BulletCollision : public Animation
{
public:
  BulletCollision();
  void Draw(sf::RenderWindow&) const override;
	bool Init() override;
};

class TankCollision : public Animation
{
public:
  TankCollision();
  void Draw(sf::RenderWindow&) const override;
	bool Init() override;
};

class SuperBulletCollision : public Animation
{
public:
  SuperBulletCollision();
  void Draw(sf::RenderWindow&) const override;
	bool Init() override;
};

class EagleCollision : public Animation
{
public:
  EagleCollision();
  void Draw(sf::RenderWindow&) const override;
	bool Init() override;
};

class Apperance : public Animation
{
public:
  Apperance();
  void Draw(sf::RenderWindow&) const override;
	bool Init() override;
};

#endif// BATTLECITY_ANIMATION_HPP