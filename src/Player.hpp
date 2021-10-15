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

struct TankControl;
struct BulletControl;


class Player
{
public:

  struct TankControl
  {
    TankControl(float vx, float vy, EActions aSide)
       : velocity(vx, vy)
       , side(aSide)
    {
    }

    void tankAction(sf::Time time, std::shared_ptr<Entity> entity, bool back);

    sf::Vector2f velocity;
    EActions side;
  };

  struct BulletControl
  {
    BulletControl(const std::multimap<ECategory, std::shared_ptr<Entity>> &ent, ECategory t)
       : type(t)
       , entities(ent)
    {
    }

    void bulletAction(std::shared_ptr<Entity> entity);

    ECategory type;
    std::multimap<ECategory, std::shared_ptr<Entity>> entities;
    sf::Vector2f velocity;
  };


public:
  typedef std::pair<std::multimap<ECategory, std::shared_ptr<Entity>>::iterator, std::multimap<ECategory, std::shared_ptr<Entity>>::iterator> rangePtr;
  typedef std::pair<std::unordered_multimap<EImage, Node>::iterator, std::unordered_multimap<EImage, Node>::iterator> rangeMapPtr;

public:
  Player(std::multimap<ECategory, std::shared_ptr<Entity>> &,
     std::multimap<EImage, Animation*> &,
     EGamestates &,
     std::queue<std::shared_ptr<Entity>> &,
     std::queue<Map *> &,
     RightPanel &,
     std::multimap<EImage, BaseBonus *> &);

  void handleEvent(const sf::Event &event, sf::Time TimePerFrame);
  void handleRealtimeInput(sf::Time TimePerFrame);
  void handleEnemyTanks(std::shared_ptr<Entity>);
  void handleBonusEvents(sf::Time TimePerFrame);
  void handleAnimation(sf::FloatRect, EImage);
  bool isIntersectsPlayerTank();
  bool isIntersectsBullet();
  bool isIntersectsSuperBullet();
  bool isIntersectsEnemy();
  bool isIntersectsBonus();
  void isIntersectsOthers();
  bool myIntersection(sf::FloatRect, sf::FloatRect);
  void handleEnemySpawn(sf::Time);
  bool handleEnemyApperanceEffect();
  void initialPlayer();
  void handleEnemyFire(sf::Time, std::shared_ptr<Entity>);
  BaseTank *getPlayerTank() { return playerTank; }
  void Init();

private:
  void initializeObjects();
  void initializeActions();
  static bool isRealtimeAction(EActions action);
  void getTestMap(std::map<sf::FloatRect *, sf::Sprite *> &) {}


private:
  std::multimap<ECategory, std::shared_ptr<Entity>> &entities;
  std::multimap<EImage, Animation *> &animations;
  EGamestates &gameStage;
  std::queue<std::shared_ptr<Entity>> &enemyTanks;
  float spawnEnemyTanksTime;
  std::queue<Map *> &mapSequence;
  RightPanel &panel;
  size_t enemyTanksOnFieldNumber;
  std::multimap<EImage, BaseBonus *> &bonuses;
  BaseTank *playerTank;
  std::map<sf::Keyboard::Key, EActions> mKeyBinding;
  std::map<EActions, std::unique_ptr<TankControl>> mActionBinding;
  std::map<EActions, std::unique_ptr<BulletControl>> mFireBinding;
  rangePtr retBullet;
  rangePtr retSuperBullet;
  rangePtr retEnemy;
  rangePtr retEagle;
  rangeMapPtr retMainWall;
  rangeMapPtr retWall_1;
  rangeMapPtr retWall_2;
  rangeMapPtr retWaterWall;
};

#endif// BATTLECITY_PLAYER_HPP