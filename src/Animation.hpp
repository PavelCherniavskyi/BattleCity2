#ifndef BATTLECITY_ANIMATION_HPP
#define BATTLECITY_ANIMATION_HPP

#include "Entity.hpp"
#include "Enums.hpp"

class Animation : public std::enable_shared_from_this<Animation>
{
public:
  Animation(const size_t aSpritesCount, const EImage aType);
  virtual ~Animation(){}
	virtual bool Init() = 0;
  virtual void Draw(sf::RenderWindow&) = 0;
  virtual void update();
  virtual void Bang(const sf::FloatRect&) = 0;
  virtual bool IsAlife() const;

	friend bool setInit(const std::shared_ptr<Animation> aAnimationPtr, const float aScale, const float aLifeTime);

protected:
  static sf::Clock mClockForTimeOfLiving;
  static sf::Clock mClockForDraw;
  std::vector<sf::Sprite> mSprites;
  bool mIsLiving;
  float mTimeOfLiving;
	const size_t mSpritesCount;
	const EImage mType;
};

class BulletCollision : public Animation
{
public:
  BulletCollision();
  void Draw(sf::RenderWindow&) override;
  void Bang(const sf::FloatRect&) override;
	bool Init() override;
};

class TankCollision : public Animation
{
public:
  TankCollision();
  void Draw(sf::RenderWindow&) override;
  void Bang(const sf::FloatRect&) override;
	bool Init() override;
};

class SuperBulletCollision : public Animation
{
public:
  SuperBulletCollision();
  void Draw(sf::RenderWindow&) override;
  void Bang(const sf::FloatRect&) override;
	bool Init() override;
};

class EagleCollision : public Animation
{
public:
  EagleCollision();
  void Draw(sf::RenderWindow&) override;
  void Bang(const sf::FloatRect&) override;
	bool Init() override;
};

class Apperance : public Animation
{
public:
  Apperance();
  void Draw(sf::RenderWindow&) override;
  void Bang(const sf::FloatRect&) override;
	bool Init() override;
};

#endif// BATTLECITY_ANIMATION_HPP