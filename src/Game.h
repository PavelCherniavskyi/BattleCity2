#pragma once
#include <iostream>

#include "AnimationHandler.hpp"
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
  void updateFPS(sf::Time elapsedTime);
  void draw();
  void stageRender();
  void nextLvlInitialize();
  bool tankLoad(size_t);
  void textInitialization();

private:
  sf::RenderWindow mWindow;
  bool mIsPaused;
  TextHolder mTextHolder;
  std::unordered_multimap<ECategory, std::shared_ptr<Entity>> mEntities;
  AnimationHandler mAnimationHandler;
  std::vector<std::shared_ptr<Map>> mapSequence;
  BonusHandler mBonusHandler;
  Player player;
  EGamestates gameStage;
  std::vector<std::shared_ptr<Entity>> enemyTanks;
  bool mIsMoving;
  RightPanel panel;
  std::unique_ptr<InputHandler> mInputHandlerUPtr;
  std::vector<sf::Text> mText;
};
