#pragma once
#include <iostream>


#include "Entity.h"
#include "Tank.h"
#include "Map.h"
#include "Bullet.h"
#include "Player.h"
#include "Category.h"
#include "Animation.h"


class Game
{
public:
  Game();
  ~Game();
  void run();
  void Init();
  sf::Time getTimePerFrame() { return TimePerFrame; }

private:
  void handleInput(sf::Time TimePerFrame);
  void update(sf::Time elapsedTime);
  void render();
  bool intersection(Category::Type category);
  void updateStatistics(sf::Time elapsedTime);
  void draw();
  void stageRender();
  void nextLvlInitialize();
  void tankLoad(size_t);
  void textInitialization();

private:
  sf::RenderWindow mWindow;
  const sf::Time TimePerFrame;
  bool isPaused;
  sf::Font mFont;
  std::map<TextHolder::text, sf::Text> texts;
  sf::Time mStatisticsUpdateTime;
  std::size_t mStatisticsNumFrames;
  std::multimap<Category::Type, Entity *> entities;
  std::multimap<Textures::ID, Animation *> animations;
  std::multimap<Textures::ID, BaseBonus *> bonuses;
  std::queue<Map *> mapSequence;
  Player player;
  Player::Gamestates gameStage;
  std::queue<Entity *> enemyTanks;
  const size_t enemyTanksQuantity;
  bool mIsMoving;
  RightPanel panel;
};
