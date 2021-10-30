#ifndef BATTLECITY_PLAYER_HPP
#define BATTLECITY_PLAYER_HPP

#include "Entity.hpp"
#include "Bullet.hpp"
#include "Map.hpp"
#include "Tank.hpp"
#include "Bonus.hpp"
#include "Animation.hpp"
#include "RightPanel.hpp"

#include <SFML/Window/Event.hpp>

struct KeyboardControl;
struct MouseControl;


class Player
{
public:

  struct KeyboardControl
  {
    KeyboardControl(float vx, float vy, EActions aSide)
       : Velocity(vx, vy)
       , Side(aSide)
    {
    }

    void KeyboardAction(sf::Time, std::shared_ptr<PlayerTank>, bool);

    sf::Vector2f Velocity;
    EActions Side;
  };

  struct MouseControl
  {
    MouseControl(std::unordered_multimap<ECategory, std::shared_ptr<Entity>> &ent, ECategory t)
       : type(t)
       , entities(ent)
    {
    }

    void MouseAction(std::shared_ptr<Entity> entity);

    ECategory type;
    std::unordered_multimap<ECategory, std::shared_ptr<Entity>>& entities;
    sf::Vector2f velocity;
  };


public:
  typedef std::pair<std::unordered_multimap<ECategory, std::shared_ptr<Entity>>::iterator, std::unordered_multimap<ECategory, std::shared_ptr<Entity>>::iterator> rangePtr;
  typedef std::pair<std::unordered_multimap<EImage, Node>::iterator, std::unordered_multimap<EImage, Node>::iterator> rangeMapPtr;

public:
  Player(std::unordered_multimap<ECategory, std::shared_ptr<Entity>> &,
     std::unordered_multimap<EImage, std::shared_ptr<Animation>> &,
     EGamestates &,
     std::vector<std::shared_ptr<Entity>> &,
     std::vector<std::shared_ptr<Map>> &,
     RightPanel &,
     std::unordered_multimap<EImage, std::shared_ptr<BaseBonus>> &);

  void HandleActionEvent(const sf::Event &event, sf::Time TimePerFrame);
  void HandleMovingInput(sf::Time TimePerFrame);
  void handleEnemyTanks(std::shared_ptr<Entity>);
  void handleBonusEvents(sf::Time TimePerFrame);
  void handleAnimation(sf::FloatRect, EImage);
  bool isIntersectsPlayerTank();
  bool isIntersectsBullet();
  bool isIntersectsSuperBullet();
  bool isIntersectsEnemy();
  bool isIntersectsBonus();
  void isIntersectsOthers();
  bool Intersection(const sf::FloatRect&, const sf::FloatRect&) const;
  void handleEnemySpawn(sf::Time);
  bool handleEnemyApperanceEffect();
  void handleEnemyFire(sf::Time, std::shared_ptr<Entity>);
  std::shared_ptr<PlayerTank> getPlayerTank();
  bool Init();

private:
  void initializeObjects();
  void initializeActions();
  void getTestMap(std::map<sf::FloatRect *, sf::Sprite *> &) {}


private:
  std::unordered_multimap<ECategory, std::shared_ptr<Entity>> &entities;
  std::unordered_multimap<EImage, std::shared_ptr<Animation>> &animations;
  EGamestates &gameStage;
  std::vector<std::shared_ptr<Entity>> &enemyTanks;
  float spawnEnemyTanksTime;
  std::vector<std::shared_ptr<Map>> &mapSequence;
  RightPanel &panel;
  size_t enemyTanksOnFieldNumber;
  std::unordered_multimap<EImage, std::shared_ptr<BaseBonus>> &bonuses;
  std::unordered_map<sf::Keyboard::Key, EActions> mKeyboardBinding;
  std::unordered_map<sf::Mouse::Button, EActions> mMousedBinding;
  std::map<EActions, std::unique_ptr<KeyboardControl>> mKeyboardMovingBinding;
  std::map<EActions, std::unique_ptr<MouseControl>> mMouseActionBinding;
  rangePtr retBullet;
  rangePtr retSuperBullet;
  rangePtr retEnemy;
  rangePtr retEagle;
  rangeMapPtr retMainWall;
  rangeMapPtr retWall_1;
  rangeMapPtr retWall_2;
  rangeMapPtr retWaterWall;
  std::shared_ptr<PlayerTank> mPlayerTank;
};

#endif// BATTLECITY_PLAYER_HPP