#pragma once

#include "Entity.h"
#include "Bullet.h"
#include "Category.h"
#include "Map.h"
#include "Tank.h"
#include "Bonus.h"
#include "Animation.h"
#include "RightPanel.h"

#include <SFML/Window/Event.hpp>

#include <map>

struct TankControl;
struct BulletControl;


class Player
{
public:
  enum Action
  {
    MoveLeft,
    MoveRight,
    MoveUp,
    MoveDown,
    Fire,
    SuperFire,
    Pause,
    ActionCount
  };

  enum Gamestates
  {
    runing,
    nextLvl,
    gameOver,
    win
  };


  struct TankControl
  {
    TankControl(float vx, float vy, Entity::actions aSide)
       : velocity(vx, vy)
       , side(aSide)
    {
    }

    void tankAction(sf::Time time, Entity *entity, bool back);

    sf::Vector2f velocity;
    Entity::actions side;
  };

  struct BulletControl
  {
    BulletControl(std::multimap<Category::Type, Entity *> &ent, Category::Type t)
       : type(t)
       , entities(ent)
    {
    }

    void bulletAction(Entity *entity);

    Category::Type type;
    std::multimap<Category::Type, Entity *> &entities;
    sf::Vector2f velocity;
  };


public:
  typedef std::pair<std::multimap<Category::Type, Entity *>::iterator, std::multimap<Category::Type, Entity *>::iterator> rangePtr;
  typedef std::pair<std::multimap<Textures::ID, Node>::iterator, std::multimap<Textures::ID, Node>::iterator> rangeMapPtr;

public:
  Player(std::multimap<Category::Type, Entity *> &,
     std::multimap<Textures::ID, Animation *> &,
     Gamestates &,
     std::queue<Entity *> &,
     std::queue<Map *> &,
     RightPanel &,
     std::multimap<Textures::ID, BaseBonus *> &);

  void handleEvent(const sf::Event &event, sf::Time TimePerFrame);
  void handleRealtimeInput(sf::Time TimePerFrame);
  void handleEnemyTanks(Entity *);
  void handleBonusEvents(sf::Time TimePerFrame);
  void handleAnimation(sf::FloatRect, Textures::ID);
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
  void handleEnemyFire(sf::Time, Entity *);
  Tank *getPlayerTank() { return playerTank; }
  void Init();

private:
  void initializeObjects();
  void initializeActions();
  static bool isRealtimeAction(Action action);
  void getTestMap(std::map<sf::FloatRect *, sf::Sprite *> &) {}


private:
  std::multimap<Category::Type, Entity *> &entities;
  std::multimap<Textures::ID, Animation *> &animations;
  Gamestates &gameStage;
  std::queue<Entity *> &enemyTanks;
  float spawnEnemyTanksTime;
  std::queue<Map *> &mapSequence;
  RightPanel &panel;
  size_t enemyTanksOnFieldNumber;
  std::multimap<Textures::ID, BaseBonus *> &bonuses;
  Tank *playerTank;
  std::map<sf::Keyboard::Key, Action> mKeyBinding;
  std::map<Action, std::unique_ptr<TankControl>> mActionBinding;
  std::map<Action, std::unique_ptr<BulletControl>> mFireBinding;
  rangePtr retBullet;
  rangePtr retSuperBullet;
  rangePtr retEnemy;
  rangePtr retEagle;
  rangeMapPtr retMainWall;
  rangeMapPtr retWall_1;
  rangeMapPtr retWall_2;
  rangeMapPtr retWaterWall;
};
