#ifndef BATTLECITY_TANK_HPP
#define BATTLECITY_TANK_HPP

#include <queue>

#include "Bullet.hpp"
#include "Entity.hpp"
#include "Enums.hpp"

class BaseTank : public Entity
{
public:
  class Snapshot
  {
  public:
    Snapshot(BaseTank* aTank,
      const std::queue<std::shared_ptr<SuperBullet>>& aClip,
      const float aBulletSpeed,
      const float aBulletFrequency,
      const ECategory aCategory,
      const bool aIsMoving,
      const sf::Vector2f& aVelocity, 
      const float aSpeed,
      const size_t aHP,
      const std::vector<sf::Sprite>& aSprites,
      const EImage aImageType);

    void Restore();

  private : 
    BaseTank* mTank;
    std::queue<std::shared_ptr<SuperBullet>> mSuperBulletClip;
    float mBulletSpeed;
    float mBulletFrequency;
    ECategory mCategory;
    bool mIsMoving;
    sf::Vector2f mVelocity;
    float mSpeed;
    size_t mHP;
    std::vector<sf::Sprite> mSprites;
    EImage mImageType;
  };

  BaseTank(ECategory, EImage);
  BaseTank(const BaseTank&);
  virtual ~BaseTank() = default;
  void Draw(sf::RenderWindow&) const override;
  void Update(const sf::Vector2f&) override;
  size_t GetSuperClipSize() const;
  EImage GetType() const;
  std::shared_ptr<BulletBase> DoFire(ECategory);
  void SetBulletSpeed(const float);
  float GetBulletSpeed() const;
  void SuperClipLoad(const size_t);

  void MoveBack(const sf::Vector2f&);
  size_t GetSpritesCount() const;
  void SuperClipPop();
  void SetBulletFrequency(const float);
  float GetBulletFrequency() const;
  ECategory GetCategory() const;
  Snapshot CreateSnapshot();

protected:
  std::queue<std::shared_ptr<SuperBullet>> mSuperBulletClip;
  float mBulletSpeed;
  float mBulletFrequency;
  ECategory mCategory;
};

class PlayerTank : public BaseTank
{
public:
  PlayerTank();
  PlayerTank(const PlayerTank&);
  void SetInitialPosition();
  bool CanIDoFire() const;
  bool Init() override;
};

class EnemyBaseTank : public BaseTank
{
public:
  EnemyBaseTank(ECategory, EImage);
  void Draw(sf::RenderWindow&) const override;
  void Update(const sf::Vector2f&) override;
  bool Init() override;
  bool CanIDoFire() const;
  void SetCallbackToRotate(std::function<void(EnemyBaseTank&)>);
  void UpdateRotationTime();

protected:
  std::vector<sf::Sprite> mNumbers;
  sf::Time mRotationTime;
  std::function<void(EnemyBaseTank&)> mCallbackToRotate;
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

#endif // BATTLECITY_TANK_HPP