#include "../Game.h"

#include "../Definitions.hpp"
#include "../ResourceHolders/SpriteHolder.hpp"
#include "config.h"
#include "../Eagle.hpp"

constexpr auto kEnemyTanksQuantity = 20u;

Game::Game(std::unique_ptr<InputHandler> aInputHandlerUPtr)
  : mWindow(sf::VideoMode(kWidthScreen + kWidthRightPanel, kHeightScreen), "Battle City 2", sf::Style::Close)
  , mIsPaused(false)
  , mTextHolder()
  , mEntities{}
  , mAnimationHandler()
  , mapSequence{}
  , mBonusHandler()
  , player(mapSequence, panel, mBonusHandler)
  , gameStage(EGamestates::RUNNING)
  , panel()
  , mInputHandlerUPtr(std::move(aInputHandlerUPtr))
  , mText(EText::_size())
  , mEnemyControlUnit(mAnimationHandler)
  , mPlayerBackups()
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
        handleInput(kTimePerFrame);
        if (!mIsPaused)
        {
          update(kTimePerFrame);
        }
      }
    }
    updateFPS(elapsedTime);
    draw();
  }
}

bool Game::Init()
{
  if(!SpriteHolder::Init() 
    || !mTextHolder.Init() 
    || !mEnemyControlUnit.Init(this) 
    || !mEnemyControlUnit.LoadLevel(ELevels::_1) 
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

  auto playerTankPtr = player.GetPlayerTank();
  auto eagle = std::make_shared<Eagle>();
  if(!playerTankPtr || !eagle->Init() || !playerTankPtr->Init())
  {
    SPDLOG_ERROR("Something wrong with Eagle or Player tank");
    return false;
  }

  mEntities.insert({ECategory::PLAYERTANK, playerTankPtr});
  mEntities.insert({ECategory::EAGLE, std::move(eagle)});

  mWindow.setKeyRepeatEnabled(false);
  panel.SetCurrentMissles(playerTankPtr->GetSuperClipSize());
  panel.SetCurrentLives(playerTankPtr->GetHP());
  panel.IncrementCurrentLvl();
  mAnimationHandler.SetAppearanceFinishCallback([this](){appearanceIsFinished();});
  mEnemyControlUnit.SetNewBulletCallback([this](const auto& aBullet){insertNewBullet(aBullet);});
  player.SetNewBulletCallback([this](const auto& aBullet){insertNewBullet(aBullet);});

  mapSequence.emplace_back(std::make_shared<Map4>());
  mapSequence.emplace_back(std::make_shared<Map3>());
  mapSequence.emplace_back(std::make_shared<Map2>());
  mapSequence.emplace_back(std::make_shared<Map1>());

  if(auto level = mInputHandlerUPtr->GetOption(EInputOption::LEVEL); !level.empty())
  {
    const auto expectedLevel = static_cast<size_t>(stoi(level));
    auto popedLevel = 1u;
    while (popedLevel != ELevels::_size() && popedLevel != expectedLevel)
    {
      if(popedLevel++ == expectedLevel)
      {
        break;
      }
      mapSequence.pop_back();
    }

    auto elevel2Load = Utils::Num2Elevels(expectedLevel);

    mEnemyControlUnit.ResetTanksOnField();
    mEnemyControlUnit.ResetTanksQueue();
    mEnemyControlUnit.LoadLevel(elevel2Load);
    panel.SetCurrentLevel(expectedLevel);
  }

  if(!mInputHandlerUPtr->GetOption(EInputOption::DEBUG).empty())
  {
    mapSequence.emplace_back(std::make_shared<Map0>()); // Test mode
    mEnemyControlUnit.ResetTanksOnField();
    mEnemyControlUnit.ResetTanksQueue();
    mEnemyControlUnit.LoadLevel(ELevels::_0);
    panel.SetCurrentLevel(0u);
  }

  for(auto& map : mapSequence)
  {
    if(!map->Init())
    {
      SPDLOG_ERROR("Something wrong with map");
      return false;
    }
  }
  
  return true;
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
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
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
  const auto map = mapSequence.back();
  
  // Bullet vs Walls
  for (auto itrBullet = retBullet.first; itrBullet != retBullet.second; ++itrBullet)
  {
    for (auto itrMap = map->GetMapObjects().begin(); itrMap != map->GetMapObjects().end(); itrMap++)
    {
      if (Utils::Intersection(itrBullet->second->GetGlobalBounds(), itrMap->second.getGlobalBounds()))
      {
        if(itrMap->first == +EMapObjects::GREENWALL || itrMap->first == +EMapObjects::WATERWALL)
        {
          continue;
        }
        else if(itrMap->first == +EMapObjects::WALL)
        {
          map->DestroyObject(itrMap);
        }
        
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
      tankIter->get()->MakeDamage(1);
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
    if (Utils::Intersection(player.GetPlayerTank()->GetGlobalBounds(), itrBullet->second->GetGlobalBounds()))
    {
      mAnimationHandler.CreateAnimation(itrBullet->second->GetGlobalBounds(), EImage::BULLETCOLLISION);
      mEntities.erase(itrBullet);
      player.GetPlayerTank()->MakeDamage(1);
      if (!player.GetPlayerTank()->IsAlife())
      {
        mAnimationHandler.CreateAnimation(player.GetPlayerTank()->GetGlobalBounds(), EImage::TANKCOLLISION);
        mEntities.erase(ECategory::PLAYERTANK);
        gameStage = EGamestates::GAME_OVER;
      }
      else
      {
        auto lives = player.GetPlayerTank()->GetHP();
        panel.SetCurrentLives(lives);
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
      eagle->second->MakeDamage(1);
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
  const auto map = mapSequence.back();

  // SuperBullet vs Walls
  for (auto itrSuperBullet = retSuperBullet.first; itrSuperBullet != retSuperBullet.second; ++itrSuperBullet)
  {
    for (auto itrMap = map->GetMapObjects().begin(); itrMap != map->GetMapObjects().end(); itrMap++)
    {
      if (Utils::Intersection(itrSuperBullet->second->GetGlobalBounds(), itrMap->second.getGlobalBounds()))
      {
        const auto type = itrMap->first;
        if(type == +EMapObjects::WATERWALL)
        {
          continue;
        }
        if(type == +EMapObjects::WALL || type == +EMapObjects::STEELWALL)
        {
          map->DestroyObject(itrMap);
        }
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
    if (Utils::Intersection(player.GetPlayerTank()->GetGlobalBounds(), itrSuperBullet->second->GetGlobalBounds()))
    {
      mAnimationHandler.CreateAnimation(itrSuperBullet->second->GetGlobalBounds(), EImage::SUPERBULLETCOLLISION);
      mAnimationHandler.CreateAnimation(player.GetPlayerTank()->GetGlobalBounds(), EImage::TANKCOLLISION);
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
      eagle->second->MakeDamage(1);
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
  // EnemyTank vs Walls
  const auto mapObjects = mapSequence.back()->GetMapObjects();
  for (auto itrMap = mapObjects.begin(); itrMap != mapObjects.end(); itrMap++)
  {
    if(itrMap->first == +EMapObjects::GREENWALL)
    {
      continue;
    }
    EnemyControlUnit::EnemyTankIter tankIter;
    if (mEnemyControlUnit.Intersection(itrMap->second.getGlobalBounds(), tankIter))
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
    eagle->second->MakeDamage(1);
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
  if (auto playerTank = player.GetPlayerTank(); mEnemyControlUnit.Intersection(playerTank->GetGlobalBounds(), tankIter))
  {
    playerTank->MakeDamage(2u);
    mAnimationHandler.CreateAnimation(tankIter->get()->GetGlobalBounds(), EImage::TANKCOLLISION);
    mEnemyControlUnit.DeleteTank(tankIter);
    
    if(!playerTank->IsAlife())
    {
      mAnimationHandler.CreateAnimation(playerTank->GetGlobalBounds(), EImage::TANKCOLLISION);
      mEntities.erase(ECategory::PLAYERTANK);
      gameStage = EGamestates::GAME_OVER;
    }
    else if(playerTank->IsAlife())
    {
      panel.SetCurrentLives(playerTank->GetHP());
      playerTank->SetInitialPosition();
      mEntities.erase(ECategory::PLAYERTANK);
      auto playerApperance = [this]() {
          mEntities.insert({ECategory::PLAYERTANK, player.GetPlayerTank()});
        };
      mAnimationHandler.CreateAnimation(playerTank->GetGlobalBounds(), EImage::APPERANCE, playerApperance);
    }
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
  auto isPlayerAlife = player.GetPlayerTank()->IsAlife();
  if (noEnemyOnField
      && mapSequence.size() > 1 
      && isPlayerAlife 
      && noEnemyInQueue)
  {
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
  mEnemyControlUnit.LoadLevel(Utils::Num2Elevels(panel.GetCurrentLvl()));
  player.GetPlayerTank()->SetInitialPosition();
  gameStage = EGamestates::RUNNING;
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
  mWindow.clear();

  // Draw for All mEntities
  for (auto itr = mEntities.begin(); itr != mEntities.end(); itr++)
  {
    itr->second->Draw(mWindow);
  }

  mEnemyControlUnit.Draw(mWindow, mIsPaused);
  mapSequence.back()->Draw(mWindow);
  mBonusHandler.Draw(mWindow);
  mAnimationHandler.Draw(mWindow);
  panel.Draw(mWindow);

  stageRender();
  mWindow.display();
}
