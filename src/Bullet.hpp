#ifndef BATTLECITY_BULLET_HPP
#define BATTLECITY_BULLET_HPP

#include "Entity.hpp"
#include "Enums.hpp"

class BulletBase : public Entity
{
public:
  BulletBase(ECategory);
  ~BulletBase() = default;
  void Draw(sf::RenderWindow& window) override;
  void Update(const sf::Vector2f& vector) override;
  sf::FloatRect GetGlobalBounds() const override;
  void SetPosition(const sf::Vector2f& aPosition) override;
  void Rotate(EActions aAction) override;
  sf::FloatRect GetLocalBounds() const override;
  const std::vector<sf::Sprite>& GetSprite() const override;
  const sf::Vector2f& GetPosition() const;
  ECategory GetBulletType() const;

protected:
  bool setInit(BulletBase& aBullet, const float aScale);

protected:
  std::vector<sf::Sprite> mSprite;
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