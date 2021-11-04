#include "../Game.h"

#include "../Definitions.hpp"
#include "../ResourceHolders/SpriteHolder.hpp"
#include "config.h"

constexpr auto kEnemyTanksQuantity = 20u;

Game::Game(std::unique_ptr<InputHandler> aInputHandlerUPtr)
  : mWindow(sf::VideoMode(kWidthScreen + kWidthRightPanel, kHeightScreen), "Battle City 2", sf::Style::Close)
  , mIsPaused(false)
  , mTextHolder()
  , mEntities{}
  , mAnimationHandler()
  , mapSequence{}
  , mBonusHandler()
  , player(mEntities, mAnimationHandler, gameStage, enemyTanks, mapSequence, panel, mBonusHandler)
  , gameStage(EGamestates::RUNNING)
  , panel()
  , mInputHandlerUPtr(std::move(aInputHandlerUPtr))
  , mText(EText::_size())
  , mEnemyControlUnit(mAnimationHandler)
{
}

void Game::ShutDown()
{
  SpriteHolder::ShutDown();
}

void Game::Run()
{
  sf::Clock clock;
  sf::Time timeSinceLastUpdate = sf::Time::Zero;
  while (mWindow.isOpen())
  {
    sf::Time elapsedTime = clock.restart();
    timeSinceLastUpdate += elapsedTime;
    while (timeSinceLastUpdate > kTimePerFrame)
    {
      timeSinceLastUpdate -= kTimePerFrame;
      if (gameStage != +EGamestates::GAME_OVER && gameStage != +EGamestates::WIN)
      {
        if (mIsPaused)
        {
          continue;
        }
        handleInput(kTimePerFrame);
        update(kTimePerFrame);
      }
    }
    updateFPS(elapsedTime);
    render();
  }
}

bool Game::Init()
{
  if(!SpriteHolder::Init() 
    || !mTextHolder.Init() 
    || !mEnemyControlUnit.Init(this) 
    || !mEnemyControlUnit.LoadLevel(1) 
    || !player.Init() 
    || !panel.Init())
  {
    SPDLOG_ERROR("Init failure");
    return false;
  }

  for(const auto& idx : EText::_values())
  {
    if(const auto text = mTextHolder.GetText(idx); !text)
    {
      SPDLOG_ERROR("No text for: {}", idx._to_string());
      return false;
    }
    else
    {
      mText[idx] = *text;
    }
  }

  auto playerTankPtr = player.getPlayerTank();
  if(!playerTankPtr)
  {
    SPDLOG_ERROR("Player tank is null");
    return false;
  }
  mWindow.setKeyRepeatEnabled(false);
  panel.SetCurrentMissles(playerTankPtr->GetSuperClipSize());
  panel.SetCurrentLives(static_cast<std::size_t>(playerTankPtr->GetHP()));
  panel.IncrementCurrentLvl();
  mAnimationHandler.SetAppearanceFinishCallback([this](){appearanceIsFinished();});
  mEnemyControlUnit.SetNewBulletCallback([this](const auto& aBullet){insertNewBullet(aBullet);});
  player.SetNewBulletCallback([this](const auto& aBullet){insertNewBullet(aBullet);});
  
  return true;
}

void Game::SetGameStage(EGamestates aGameStage)
{
  gameStage = aGameStage;
}

void Game::handleInput(sf::Time aTimePerFrame)
{
  sf::Event event;
  while (mWindow.pollEvent(event))
  {
    if (event.type == sf::Event::Closed)
    {
      mWindow.close();
    }
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
    {
      mIsPaused = !mIsPaused;
    }
  }
  if (!mIsPaused)
  {
    player.HandleActionEvent(event, aTimePerFrame);
    player.HandleMovingInput(aTimePerFrame);
  }
}

void Game::appearanceIsFinished()
{
  panel.PopIcon();
}

bool Game::isIntersectsBullet()
{
  auto retBullet = mEntities.equal_range(ECategory::BULLET);
  
  // Bullet vs Wall_1
  auto retWall_1 = mapSequence.back()->GetMap().equal_range(EImage::WALL_1);
  for (auto itrBullet = retBullet.first; itrBullet != retBullet.second; ++itrBullet)
  {
    for (auto itrMap = retWall_1.first; itrMap != retWall_1.second; itrMap++)
    {
      if (Utils::Intersection(itrBullet->second->GetGlobalBounds(), itrMap->second.Rect))
      {
        mAnimationHandler.CreateAnimation(itrBullet->second->GetGlobalBounds(), EImage::BULLETCOLLISION);
        mapSequence.back()->GetMap().erase(itrMap);
        mEntities.erase(itrBullet);
        return true;
      }
    }
  }
  
  // Bullet vs Wall_2
  auto retWall_2 = mapSequence.back()->GetMap().equal_range(EImage::WALL_2);
  for (auto itrBullet = retBullet.first; itrBullet != retBullet.second; ++itrBullet)
  {
    for (auto itrMap = retWall_2.first; itrMap != retWall_2.second; itrMap++)
    {
      if (Utils::Intersection(itrBullet->second->GetGlobalBounds(), itrMap->second.Rect))
      {
        mAnimationHandler.CreateAnimation(itrBullet->second->GetGlobalBounds(), EImage::BULLETCOLLISION);
        mEntities.erase(itrBullet);
        return true;
      }
    }
  }
  
  // Bullet vs MainWall
  auto retMainWall = mapSequence.back()->GetMap().equal_range(EImage::MAINWALL);
  for (auto itrBullet = retBullet.first; itrBullet != retBullet.second; ++itrBullet)
  {
    for (auto itrMap = retMainWall.first; itrMap != retMainWall.second; itrMap++)
    {
      if (Utils::Intersection(itrBullet->second->GetGlobalBounds(), itrMap->second.Rect))
      {
        mAnimationHandler.CreateAnimation(itrBullet->second->GetGlobalBounds(), EImage::BULLETCOLLISION);
        mEntities.erase(itrBullet);
        return true;
      }
    }
  }

  // Bullet vs EnemyTank
  for (auto itrBullet = retBullet.first; itrBullet != retBullet.second; ++itrBullet)
  {
    EnemyControlUnit::EnemyTankIter tankIter;
    if (mEnemyControlUnit.Intersection(itrBullet->second->GetGlobalBounds(), tankIter))
    {
      mAnimationHandler.CreateAnimation(itrBullet->second->GetGlobalBounds(), EImage::BULLETCOLLISION);
      mEntities.erase(itrBullet);
      tankIter->get()->Kill();
      if (!tankIter->get()->IsAlife())
      {
        mAnimationHandler.CreateAnimation(tankIter->get()->GetGlobalBounds(), EImage::TANKCOLLISION);
        mEnemyControlUnit.DeleteTank(tankIter);
      }
      return true;
    }
  }

  // Bullet vs PlayerTank
  for (auto itrBullet = retBullet.first; itrBullet != retBullet.second; ++itrBullet)
  {
    if (Utils::Intersection(player.getPlayerTank()->GetGlobalBounds(), itrBullet->second->GetGlobalBounds()))
    {
      mAnimationHandler.CreateAnimation(itrBullet->second->GetGlobalBounds(), EImage::BULLETCOLLISION);
      mEntities.erase(itrBullet);
      player.getPlayerTank()->Kill();
      if (!player.getPlayerTank()->IsAlife())
      {
        mAnimationHandler.CreateAnimation(player.getPlayerTank()->GetGlobalBounds(), EImage::TANKCOLLISION);
        mEntities.erase(ECategory::PLAYERTANK);
        gameStage = EGamestates::GAME_OVER;
      }
      else
      {
        auto lives = player.getPlayerTank()->GetHP();
        if (lives < 0)
        {
          lives = 0;
        }

        panel.SetCurrentLives(static_cast<std::size_t>(lives));
      }

      return true;
    }
  }

  // Bullet vs Eagle
  auto eagle = mEntities.find(ECategory::EAGLE);
  for (auto itrBullet = retBullet.first; itrBullet != retBullet.second; ++itrBullet)
  {
    if (Utils::Intersection(eagle->second->GetGlobalBounds(), itrBullet->second->GetGlobalBounds()))
    {
      mAnimationHandler.CreateAnimation(itrBullet->second->GetGlobalBounds(), EImage::BULLETCOLLISION);
      mAnimationHandler.CreateAnimation(eagle->second->GetGlobalBounds(), EImage::EAGLECOLLISION);
      eagle->second->Kill();
      mEntities.erase(itrBullet);
      gameStage = EGamestates::GAME_OVER;
      return true;
    }
  }

  return false;
}

bool Game::isIntersectsSuperBullet()
{
  auto retSuperBullet = mEntities.equal_range(ECategory::SUPERBULLET);
  auto retWall_1 = mapSequence.back()->GetMap().equal_range(EImage::WALL_1);
  

  // SuperBullet vs Wall_1
  for (auto itrSuperBullet = retSuperBullet.first; itrSuperBullet != retSuperBullet.second; ++itrSuperBullet)
  {
    for (auto itrMap = retWall_1.first; itrMap != retWall_1.second; itrMap++)
    {
      if (Utils::Intersection(itrSuperBullet->second->GetGlobalBounds(), itrMap->second.Rect))
      {
        mAnimationHandler.CreateAnimation(itrSuperBullet->second->GetGlobalBounds(), EImage::SUPERBULLETCOLLISION);
        mapSequence.back()->GetMap().erase(itrMap);
        mEntities.erase(itrSuperBullet);
        return true;
      }
    }
  }

  auto retWall_2 = mapSequence.back()->GetMap().equal_range(EImage::WALL_2);
  
  // SuperBullet vs Wall_2
  for (auto itrSuperBullet = retSuperBullet.first; itrSuperBullet != retSuperBullet.second; ++itrSuperBullet)
  {
    for (auto itrMap = retWall_2.first; itrMap != retWall_2.second; itrMap++)
    {
      if (Utils::Intersection(itrSuperBullet->second->GetGlobalBounds(), itrMap->second.Rect))
      {
        mAnimationHandler.CreateAnimation(itrSuperBullet->second->GetGlobalBounds(), EImage::SUPERBULLETCOLLISION);
        mapSequence.back()->GetMap().erase(itrMap);
        mEntities.erase(itrSuperBullet);
        return true;
      }
    }
  }

  auto retMainWall = mapSequence.back()->GetMap().equal_range(EImage::MAINWALL);
  
  // SuperBullet vs MainWall
  for (auto itrSuperBullet = retSuperBullet.first; itrSuperBullet != retSuperBullet.second; ++itrSuperBullet)
  {
    for (auto itrMap = retMainWall.first; itrMap != retMainWall.second; itrMap++)
    {
      if (Utils::Intersection(itrSuperBullet->second->GetGlobalBounds(), itrMap->second.Rect))
      {
        mAnimationHandler.CreateAnimation(itrSuperBullet->second->GetGlobalBounds(), EImage::SUPERBULLETCOLLISION);
        mEntities.erase(itrSuperBullet);
        return true;
      }
    }
  }

  // SuperBullet vs EnemyTank
  for (auto itrSuperBullet = retSuperBullet.first; itrSuperBullet != retSuperBullet.second; ++itrSuperBullet)
  {
    EnemyControlUnit::EnemyTankIter tankIter;
    if (mEnemyControlUnit.Intersection(itrSuperBullet->second->GetGlobalBounds(), tankIter))
    {
      mAnimationHandler.CreateAnimation(itrSuperBullet->second->GetGlobalBounds(), EImage::SUPERBULLETCOLLISION);
      mAnimationHandler.CreateAnimation(tankIter->get()->GetGlobalBounds(), EImage::TANKCOLLISION);
      mEntities.erase(itrSuperBullet);
      mEnemyControlUnit.DeleteTank(tankIter);
      return true;
    }
  }

  // SuperBullet vs PlayerTank
  for (auto itrSuperBullet = retSuperBullet.first; itrSuperBullet != retSuperBullet.second; ++itrSuperBullet)
  {
    if (Utils::Intersection(player.getPlayerTank()->GetGlobalBounds(), itrSuperBullet->second->GetGlobalBounds()))
    {
      mAnimationHandler.CreateAnimation(itrSuperBullet->second->GetGlobalBounds(), EImage::SUPERBULLETCOLLISION);
      mAnimationHandler.CreateAnimation(player.getPlayerTank()->GetGlobalBounds(), EImage::TANKCOLLISION);
      mEntities.erase(itrSuperBullet);
      mEntities.erase(ECategory::PLAYERTANK);
      panel.SetCurrentLives(0);
      gameStage = EGamestates::GAME_OVER;
      return true;
    }
  }

  // SuperBullet vs Eagle
  auto eagle = mEntities.find(ECategory::EAGLE);
  for (auto itrSuperBullet = retSuperBullet.first; itrSuperBullet != retSuperBullet.second; ++itrSuperBullet)
  {
    if (Utils::Intersection(eagle->second->GetGlobalBounds(), itrSuperBullet->second->GetGlobalBounds()))
    {
      mAnimationHandler.CreateAnimation(itrSuperBullet->second->GetGlobalBounds(), EImage::SUPERBULLETCOLLISION);
      mAnimationHandler.CreateAnimation(eagle->second->GetGlobalBounds(), EImage::EAGLECOLLISION);
      eagle->second->Kill();
      mEntities.erase(itrSuperBullet);
      gameStage = EGamestates::GAME_OVER;
      return true;
    }
  }

  return false;
}

void Game::insertNewBullet(std::shared_ptr<BulletBase> aBullet)
{
  mEntities.emplace(aBullet->GetBulletType(), aBullet);
}

bool Game::isIntersectsEnemy()
{

  // EnemyTank vs Wall_1
  auto retWall_1 = mapSequence.back()->GetMap().equal_range(EImage::WALL_1);
  for (auto itrMap = retWall_1.first; itrMap != retWall_1.second; itrMap++)
  {
    EnemyControlUnit::EnemyTankIter tankIter;
    if (mEnemyControlUnit.Intersection(itrMap->second.Rect, tankIter))
    {
      return true;
    }
  }

  // EnemyTank vs Wall_2
  auto retWall_2 = mapSequence.back()->GetMap().equal_range(EImage::WALL_2);

  for (auto itrMap = retWall_2.first; itrMap != retWall_2.second; itrMap++)
  {
    EnemyControlUnit::EnemyTankIter tankIter;
    if (mEnemyControlUnit.Intersection(itrMap->second.Rect, tankIter))
    {
      return true;
    }
  }

  // EnemyTank vs MainWall
  auto retMainWall = mapSequence.back()->GetMap().equal_range(EImage::MAINWALL);
  for (auto itrMap = retMainWall.first; itrMap != retMainWall.second; itrMap++)
  {
    EnemyControlUnit::EnemyTankIter tankIter;
    if (mEnemyControlUnit.Intersection(itrMap->second.Rect, tankIter))
    {
      return true;
    }
  }

  // EnemyTank vs WaterWall
  auto retWaterWall = mapSequence.back()->GetMap().equal_range(EImage::WATERWALL);
  for (auto itrMap = retWaterWall.first; itrMap != retWaterWall.second; itrMap++)
  {
    EnemyControlUnit::EnemyTankIter tankIter;
    if (mEnemyControlUnit.Intersection(itrMap->second.Rect, tankIter))
    {
      return true;
    }
  }

  // EnemyTank vs Eagle
  auto eagle = mEntities.find(ECategory::EAGLE);
  EnemyControlUnit::EnemyTankIter tankIter;
  if (mEnemyControlUnit.Intersection(eagle->second->GetGlobalBounds(), tankIter))
  {
    mAnimationHandler.CreateAnimation(eagle->second->GetGlobalBounds(), EImage::EAGLECOLLISION);
    eagle->second->Kill();
    gameStage = EGamestates::GAME_OVER;
    return true;
  }

  return false;
}

void Game::update(sf::Time elapsedTime)
{
  // Update for bullets
  auto retBullet = mEntities.equal_range(ECategory::BULLET);
  for (auto itrBullet = retBullet.first; itrBullet != retBullet.second; ++itrBullet)
  {
    itrBullet->second->Update(itrBullet->second->GetVelocity() * elapsedTime.asSeconds());
    if (isIntersectsBullet())
    {
      break;
    }
  }

  auto retSuperBullet = mEntities.equal_range(ECategory::SUPERBULLET);
  // Update for SuperBullets
  for (auto itrSuperBullet = retSuperBullet.first; itrSuperBullet != retSuperBullet.second; ++itrSuperBullet)
  {
    itrSuperBullet->second->Update(itrSuperBullet->second->GetVelocity() * elapsedTime.asSeconds());
    if (isIntersectsSuperBullet())
    {
      break;
    }
  }

  // EnemyTank vs PlayerTank
  EnemyControlUnit::EnemyTankIter tankIter;
  if (mEnemyControlUnit.Intersection(player.getPlayerTank()->GetGlobalBounds(), tankIter))
  {
    mAnimationHandler.CreateAnimation(player.getPlayerTank()->GetGlobalBounds(), EImage::TANKCOLLISION);
    mAnimationHandler.CreateAnimation(tankIter->get()->GetGlobalBounds(), EImage::TANKCOLLISION);
    mEnemyControlUnit.DeleteTank(tankIter);
    mEntities.erase(ECategory::PLAYERTANK);
    panel.SetCurrentLives(0);
    gameStage = EGamestates::GAME_OVER;
  }


  // Update for Animation
  mAnimationHandler.Update();

  mBonusHandler.Update();
  mEnemyControlUnit.Update(elapsedTime);
  mEnemyControlUnit.Spawn();
  player.HandleBonusEvents();
}

void Game::stageRender()
{
  // Check for game states
  static bool forClockRestart = true;
  static sf::Clock clock;
  if (forClockRestart)
    clock.restart();
  auto noEnemyOnField = mEnemyControlUnit.GetTanksOnFieldCount() == 0;
  auto noEnemyInQueue = mEnemyControlUnit.GetTanksQueueCount() == 0;
  auto isPlayerAlife = player.getPlayerTank()->IsAlife();
  if (noEnemyOnField
      && mapSequence.size() > 1 
      && isPlayerAlife 
      && noEnemyInQueue)
  {
    auto enemySize = mEnemyControlUnit.GetTanksOnFieldCount();
    auto enemyQueue = mEnemyControlUnit.GetTanksQueueCount();
    SPDLOG_INFO("enemySize: {}, enemyQueue: {}", enemySize, enemyQueue);
    gameStage = EGamestates::NEXT_LVL;
    forClockRestart = false;
  }
  else if (noEnemyOnField
      && mapSequence.size() == 1 
      && isPlayerAlife
      && noEnemyInQueue)
  {
    gameStage = EGamestates::WIN;
    forClockRestart = false;
  }

  if (gameStage == +EGamestates::RUNNING)
  {
    mWindow.draw(mText[EText::FPS]);
  }

  if (mIsPaused)
  {
    mWindow.draw(mText[EText::PAUSE]);
  }
  else if (gameStage == +EGamestates::GAME_OVER)
  {
    mWindow.draw(mText[EText::GAME_OVER]);
    static sf::Clock forGameOver;
    sf::Time time = forGameOver.getElapsedTime();
    if (time.asSeconds() > 3)
    {
      mWindow.close();
    }
  }
  else if (gameStage == +EGamestates::NEXT_LVL)
  {
    mWindow.draw(mText[EText::NEXT_LVL]);
    sf::Time time = clock.getElapsedTime();
    if (time.asSeconds() > 3)
    {
      nextLvlInitialize();
      forClockRestart = true;
    }
  }
  else if (gameStage == +EGamestates::WIN)
  {
    mWindow.draw(mText[EText::WIN]);
    sf::Time time = clock.getElapsedTime();
    if (time.asSeconds() > 3)
    {
      mWindow.close();
    }
  }
}

void Game::nextLvlInitialize()
{
  mapSequence.pop_back();
  panel.ResetIcons();

  panel.IncrementCurrentLvl();
  mEnemyControlUnit.LoadLevel(panel.GetCurrentLvl());
  player.getPlayerTank()->SetInitialPosition();
  gameStage = EGamestates::RUNNING;
}

void Game::render()
{
  mWindow.clear();
  draw();
  stageRender();

  mWindow.display();
}

void Game::updateFPS(sf::Time elapsedTime)
{
  static sf::Time FPSUpdateTime;
  static size_t FPSNumFrames = 0u;
  FPSUpdateTime += elapsedTime;
  FPSNumFrames += 1;

  if (FPSUpdateTime >= sf::seconds(1.0f))
  {
    mText[EText::FPS].setString("FPS: " + std::to_string(FPSNumFrames));
    FPSUpdateTime = sf::seconds(0);
    FPSNumFrames = 0;
  }
}

void Game::draw()
{
  // Draw for All mEntities
  for (auto itr = mEntities.begin(); itr != mEntities.end(); itr++)
  {
    itr->second->Draw(mWindow);
  }

  mEnemyControlUnit.Draw(mWindow);
  mapSequence.back()->Draw(mWindow);
  mBonusHandler.Draw(mWindow);
  mAnimationHandler.Draw(mWindow);
  

  // Draw for Right Panel
  panel.Draw(mWindow);
}
