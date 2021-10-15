#pragma once
#include <iostream>

#include "Animation.hpp"
#include "Bullet.hpp"
#include "Entity.hpp"
#include "InputHandler.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "ResourceHolders/TextHolder.hpp"
#include "Tank.hpp"

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
  bool intersection(ECategory category);
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
  std::multimap<ECategory, std::shared_ptr<Entity>> entities;
  std::multimap<EImage, Animation *> animations;
  std::multimap<EImage, BaseBonus *> bonuses;
  std::queue<Map *> mapSequence;
  Player player;
  EGamestates gameStage;
  std::queue<std::shared_ptr<Entity>> enemyTanks;
  const size_t enemyTanksQuantity;
  bool mIsMoving;
  RightPanel panel;
  std::unique_ptr<InputHandler> mInputHandlerUPtr;
};
