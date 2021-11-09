#ifndef BATTLECITY_BULLET_HPP
#define BATTLECITY_BULLET_HPP

#include "Entity.hpp"
#include "Enums.hpp"

class BulletBase : public Entity
{
public:
  BulletBase(ECategory);
  ~BulletBase() = default;
  void Draw(sf::RenderWindow& window) const override;
  void Update(const sf::Vector2f& vector) override;
  ECategory GetBulletType() const;

protected:
  bool setInit(BulletBase& aBullet, const float aScale);

protected:
  ECategory mBulletType;
};

class SimpleBullet : public BulletBase
{
public:
  SimpleBullet();
  bool Init() override;
};

class SuperBullet : public BulletBase
{
public:
  SuperBullet();
  bool Init() override;
};

#endif// BATTLECITY_BULLET_HPP