#ifndef BATTLECITY_TANK_HPP
#define BATTLECITY_TANK_HPP

#include <queue>

#include "Bullet.hpp"
#include "Entity.hpp"
#include "Enums.hpp"

class BaseTank : public Entity
{
public:
  BaseTank(ECategory, EImage);
  virtual ~BaseTank() = default;
  void Draw(sf::RenderWindow&) override;
  void Update(const sf::Vector2f&) override;
  const std::vector<sf::Sprite>& GetSprite() const override;
  void UpdateBack(const sf::Vector2f&) override;
  void SetPosition(const sf::Vector2f&) override;
  void Rotate(EActions) override;
  sf::FloatRect GetGlobalBounds() const override;
  sf::FloatRect GetLocalBounds() const override;
  size_t GetSuperClipSize() const override;
  EImage GetType() const override;
  std::shared_ptr<BulletBase> DoFire(ECategory) override;
  void SetBulletSpeed(const float) override;
  float GetBulletSpeed() const override;
  void SuperClipLoad(const size_t) override;


  size_t GetSpritesCount() const;
  
  
  void SuperClipPop();
  
  void SetBulletFrequency(const float);
  float GetBulletFrequency() const;
  

protected:
  std::queue<std::unique_ptr<SuperBullet>> mSuperBulletClip;
  float mBulletSpeed;
  float mBulletFrequency;
  ECategory mCategory;
  EImage mType;
  std::vector<sf::Sprite> mSprites;
};

class PlayerTank : public BaseTank
{
public:
  PlayerTank();
  void SetInitialPosition() override;
  bool CanIDoFire() const override;
  bool Init() override;
};

class EnemyBaseTank : public BaseTank
{
public:
  EnemyBaseTank(ECategory, EImage);
  void Draw(sf::RenderWindow&) override;
  bool Init() override;
  bool CanIDoFire() const override;

protected:
  std::vector<sf::Sprite> mNumbers;
};

class EnemyTank_10 : public EnemyBaseTank
{
public:
  EnemyTank_10();
  bool Init() override;
};

class EnemyTank_20 : public EnemyBaseTank
{
public:
  EnemyTank_20();
  bool Init() override;
};

class EnemyTank_30 : public EnemyBaseTank
{
public:
  EnemyTank_30();
  bool Init() override;
};

class EnemyTank_40 : public EnemyBaseTank
{
public:
  EnemyTank_40();
  bool Init() override;
};

#endif// BATTLECITY_TANK_HPP