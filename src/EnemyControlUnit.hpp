
#ifndef BATTLECITY_ENEMYCONTROLUNIT_HPP
#define BATTLECITY_ENEMYCONTROLUNIT_HPP

#include "Tank.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include "AnimationHandler.hpp"
#include "RightPanel.hpp"
#include <functional>
#include "Bullet.hpp"

class Game;

class EnemyControlUnit
{
public:
  using EnemyTankIter = std::vector<std::shared_ptr<EnemyBaseTank>>::const_iterator;

  EnemyControlUnit(AnimationHandler&);
  bool Init(Game*);
  void Update(const sf::Time);
  void Draw(sf::RenderWindow&) const;
  bool LoadLevel(size_t);
  bool Intersection(const sf::FloatRect&, EnemyTankIter&) const;
  void DeleteTank(const EnemyControlUnit::EnemyTankIter&);
  void Spawn();
  void SetNewBulletCallback(std::function<void(std::shared_ptr<BulletBase>)>);
  size_t GetTanksOnFieldCount() const;
  size_t GetTanksQueueCount() const;

private:
  template <typename T>
  void loadLevelHelper(size_t);
  void setDirectionsForTank(std::shared_ptr<EnemyBaseTank>);
  void doFire(std::shared_ptr<EnemyBaseTank>);

private:
  std::vector<std::shared_ptr<EnemyBaseTank>> mTanksQueue;
  std::vector<std::shared_ptr<EnemyBaseTank>> mTanksOnField;
  AnimationHandler& mAnimationHandler;
  Game* mGameInstance;
  std::function<void(std::shared_ptr<BulletBase>)> mNewBulletCallback;
};


#endif// BATTLECITY_ENEMYCONTROLUNIT_HPP