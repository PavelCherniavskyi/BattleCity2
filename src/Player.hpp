#ifndef BATTLECITY_PLAYER_HPP
#define BATTLECITY_PLAYER_HPP

#include "Entity.hpp"
#include "Bullet.hpp"
#include "Map.hpp"
#include "Tank.hpp"
#include "BonusHandler.hpp"
#include "AnimationHandler.hpp"
#include "RightPanel.hpp"
#include "Commands.hpp"
#include <SFML/Window/Event.hpp>

class Player
{
public:
  Player(std::unordered_multimap<ECategory, std::shared_ptr<Entity>> &,
     AnimationHandler&,
     EGamestates &,
     std::vector<std::shared_ptr<Entity>> &,
     std::vector<std::shared_ptr<Map>> &,
     RightPanel &,
     BonusHandler &);

  void HandleActionEvent(const sf::Event &event, sf::Time TimePerFrame);
  void HandleMovingInput(sf::Time TimePerFrame);
  void HandleBonusEvents();
  bool isIntersectsWalls();
  bool isIntersectsBullet();
  bool isIntersectsSuperBullet();
  bool isIntersectsEnemy();
  bool Intersection(const sf::FloatRect&, const sf::FloatRect&) const;
  std::shared_ptr<PlayerTank> getPlayerTank();
  void SetNewBulletCallback(std::function<void(std::shared_ptr<BulletBase>)>);
  bool Init();

private:
  void initializeActions();

private:
  std::unordered_multimap<ECategory, std::shared_ptr<Entity>> &entities;
  AnimationHandler& mAnimationHandler;
  EGamestates &gameStage;
  std::vector<std::shared_ptr<Entity>> &mEnemyTanksQueue;
  std::vector<std::shared_ptr<Map>> &mapSequence;
  RightPanel &panel;
  BonusHandler& mBonusHandler;
  std::unordered_map<sf::Keyboard::Key, EActions> mKeyboardBinding;
  std::unordered_map<sf::Mouse::Button, EActions> mMousedBinding;
  std::map<EActions, std::unique_ptr<KeyboardCommand>> mKeyboardActions;
  std::map<EActions, std::unique_ptr<MouseCommand>> mMouseActions;
  std::shared_ptr<PlayerTank> mPlayerTank;
  std::function<void(std::shared_ptr<BulletBase>)> mNewBulletCallback;
};

#endif// BATTLECITY_PLAYER_HPP