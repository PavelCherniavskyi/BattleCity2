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
#include "BonusHandler.hpp"

class Game
{
public:
  Game(std::unique_ptr<InputHandler> aInputHandlerUPtr);
  void Run();
  bool Init();

private:
  void handleInput(sf::Time aTimePerFrame);
  void update(sf::Time elapsedTime);
  void render();
  bool intersection(ECategory category);
  void updateStatistics(sf::Time elapsedTime);
  void draw();
  void stageRender();
  void nextLvlInitialize();
  bool tankLoad(size_t);
  void textInitialization();

private:
  sf::RenderWindow mWindow;
  bool mIsPaused;
  TextHolder mTextHolder;
  sf::Time mStatisticsUpdateTime;
  std::size_t mStatisticsNumFrames;
  std::unordered_multimap<ECategory, std::shared_ptr<Entity>> mEntities;
  std::unordered_multimap<EImage, std::shared_ptr<Animation>> mAnimations;
  std::vector<std::shared_ptr<Map>> mapSequence;
  BonusHandler mBonusHandler;
  Player player;
  EGamestates gameStage;
  std::vector<std::shared_ptr<Entity>> enemyTanks;
  bool mIsMoving;
  RightPanel panel;
  std::unique_ptr<InputHandler> mInputHandlerUPtr;
};
