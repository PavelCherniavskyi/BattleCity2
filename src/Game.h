#pragma once
#include <iostream>

#include "Animation.h"
#include "Bullet.h"
#include "Category.h"
#include "Entity.h"
#include "InputHandler.hpp"
#include "Map.h"
#include "Player.h"
#include "ResourceHolders/TextHolder.hpp"
#include "Tank.h"

class Game
{
public:
  Game(std::unique_ptr<InputHandler> aInputHandlerUPtr);
  ~Game();
  void run();
  void Init();

private:
  void handleInput(sf::Time aTimePerFrame);
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
  bool mIsPaused;
  TextHolder mTextHolder;
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
  std::unique_ptr<InputHandler> mInputHandlerUPtr;
};
