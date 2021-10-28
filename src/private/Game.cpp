#include "../Game.h"

#include "../Definitions.hpp"
#include "../ResourceHolders/SpriteHolder.hpp"
#include "config.h"

constexpr auto kEnemyTanksQuantity = 20u;

Game::Game(std::unique_ptr<InputHandler> aInputHandlerUPtr)
  : mWindow(sf::VideoMode(kWidthScreen + kWidthRightPanel * 2, kHeightScreen), "Battle City 2", sf::Style::Close)
  , mIsPaused(false)
  , mTextHolder()
  , mStatisticsUpdateTime()
  , mStatisticsNumFrames(0)
  , mEntities{}
  , mAnimations{}
  , mBonuses{}
  , mapSequence{}
  , player(mEntities, mAnimations, gameStage, enemyTanks, mapSequence, panel, mBonuses)
  , gameStage(EGamestates::RUNNING)
  , panel()
  , mInputHandlerUPtr(std::move(aInputHandlerUPtr))
{
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
    updateStatistics(elapsedTime);
    render();
  }
}

bool Game::Init()
{
  if(!SpriteHolder::Init() || !mTextHolder.Init() || !tankLoad(1) || !player.Init() || !panel.Init())
  {
    SPDLOG_ERROR("Init failure");
    return false;
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

void Game::update(sf::Time elapsedTime)
{
  Player::rangePtr retBullet;
  Player::rangePtr retSuperBullet;
  Player::rangePtr retEnemyTank;

  // Update for bullets
  retBullet = mEntities.equal_range(ECategory::BULLET);
  for (auto itrBullet = retBullet.first; itrBullet != retBullet.second; ++itrBullet)
  {
    itrBullet->second->Update(itrBullet->second->GetVelocity() * elapsedTime.asSeconds());
    if (player.isIntersectsBullet())
    {
      break;
    }
  }
  retSuperBullet = mEntities.equal_range(ECategory::SUPERBULLET);
  // Update for SuperBullets
  for (auto itrSuperBullet = retSuperBullet.first; itrSuperBullet != retSuperBullet.second; ++itrSuperBullet)
  {
    itrSuperBullet->second->Update(itrSuperBullet->second->GetVelocity() * elapsedTime.asSeconds());
    if (player.isIntersectsSuperBullet())
    {
      break;
    }
  }
  // Update for Enemy tanks
  retEnemyTank = mEntities.equal_range(ECategory::ENEMYTANK);
  for (auto itrTank = retEnemyTank.first; itrTank != retEnemyTank.second; itrTank++)
  {
    itrTank->second->Update(itrTank->second->GetVelocity() * elapsedTime.asSeconds());
    if (player.isIntersectsEnemy())
    {
      itrTank->second->MoveBack(itrTank->second->GetVelocity() * elapsedTime.asSeconds());
      player.handleEnemyTanks(itrTank->second);
    }
    player.handleEnemyFire(kTimePerFrame, itrTank->second);
  }

  // Update for Animation
  for (auto itrAnim = mAnimations.begin(); itrAnim != mAnimations.end(); itrAnim++)
  {
    if (itrAnim->second->IsAlife())
    {
      itrAnim->second->update();
    }
    else
    {
      mAnimations.erase(itrAnim);
      break;
    }
  }

  player.handleEnemySpawn(kTimePerFrame);
  player.handleBonusEvents(kTimePerFrame);
  player.isIntersectsOthers();
}

void Game::stageRender()
{
  // Check for game states
  static bool forClockRestart = true;
  static sf::Clock clock;
  if (forClockRestart)
    clock.restart();
  auto checkIfEmpty = mEntities.find(ECategory::ENEMYTANK);
  auto checkIfPlayerAlife = mEntities.find(ECategory::PLAYERTANK);
  if (checkIfEmpty == mEntities.end() 
      && mapSequence.size() > 1 
      && checkIfPlayerAlife != mEntities.end() 
      && enemyTanks.empty())
  {
    gameStage = EGamestates::NEXT_LVL;
    forClockRestart = false;
  }
  else if (checkIfEmpty == mEntities.end() 
      && mapSequence.size() == 1 
      && checkIfPlayerAlife != mEntities.end() 
      && enemyTanks.empty())
  {
    gameStage = EGamestates::WIN;
    forClockRestart = false;
    // std::cout << "win" << std::endl;
  }

  if (gameStage == +EGamestates::RUNNING)
  {
    auto text = mTextHolder.GetText(EText::STATISTIC);
    if (text)
    {
      mWindow.draw(*text);
    }
    else
    {
      SPDLOG_ERROR("No text for STATISTIC");
    }
  }

  if (mIsPaused)
  {
    auto text = mTextHolder.GetText(EText::PAUSE);
    if (text)
    {
      mWindow.draw(*text);
    }
    else
    {
      SPDLOG_ERROR("No text for PAUSE");
    }
  }

  else if (gameStage == +EGamestates::GAME_OVER)
  {
    auto text = mTextHolder.GetText(EText::GAME_OVER);
    if (text)
    {
      mWindow.draw(*text);
    }
    else
    {
      SPDLOG_ERROR("No text for GAME OVER");
    }
    static sf::Clock forGameOver;
    sf::Time time = forGameOver.getElapsedTime();
    if (time.asSeconds() > 2)
    {
      mWindow.close();
    }
  }
  else if (gameStage == +EGamestates::NEXT_LVL)
  {
    auto text = mTextHolder.GetText(EText::NEXT_LVL);
    if (text)
    {
      mWindow.draw(*text);
    }
    else
    {
      SPDLOG_ERROR("No text for NEXT_LVL");
    }
    sf::Time time = clock.getElapsedTime();
    if (time.asSeconds() > 3)
    {
      nextLvlInitialize();
      forClockRestart = true;
    }
  }
  else if (gameStage == +EGamestates::WIN)
  {
    auto text = mTextHolder.GetText(EText::WIN);
    if (text)
    {
      mWindow.draw(*text);
    }
    else
    {
      SPDLOG_ERROR("No text for WIN");
    }
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
  tankLoad(panel.GetCurrentLvl());
  auto TankPos = mEntities.find(ECategory::PLAYERTANK);
  TankPos->second->SetInitialPosition();
  gameStage = EGamestates::RUNNING;
}

bool Game::tankLoad(size_t levl)
{
  switch (levl)
  {
  case 1:
    for (int i = 0; i < 2; i++)
    {
      enemyTanks.emplace_back(std::make_shared<EnemyTank_20>());
    }
    for (int i = 0; i < 18; i++)
    {
      enemyTanks.emplace_back(std::make_shared<EnemyTank_10>());
    }
    break;
  case 2:
    for (int i = 0; i < 2; i++)
    {
      enemyTanks.emplace_back(std::make_shared<EnemyTank_30>());
    }
    for (int i = 0; i < 4; i++)
    {
      enemyTanks.emplace_back(std::make_shared<EnemyTank_20>());
    }
    for (int i = 0; i < 14; i++)
    {
      enemyTanks.emplace_back(std::make_shared<EnemyTank_10>());
    }
    break;
  case 3:
    for (int i = 0; i < 4; i++)
    {
      enemyTanks.emplace_back(std::make_shared<EnemyTank_30>());
    }
    for (int i = 0; i < 6; i++)
    {
      enemyTanks.emplace_back(std::make_shared<EnemyTank_20>());
    }
    for (int i = 0; i < 10; i++)
    {
      enemyTanks.emplace_back(std::make_shared<EnemyTank_10>());
    }
    break;
  case 4:
    for (int i = 0; i < 4; i++)
    {
      enemyTanks.emplace_back(std::make_shared<EnemyTank_40>());
    }
    for (int i = 0; i < 6; i++)
    {
      enemyTanks.emplace_back(std::make_shared<EnemyTank_30>());
    }
    for (int i = 0; i < 8; i++)
    {
      enemyTanks.emplace_back(std::make_shared<EnemyTank_20>());
    }
    for (int i = 0; i < 2; i++)
    {
      enemyTanks.emplace_back(std::make_shared<EnemyTank_10>());
    }
    break;
  default:
    for (int i = 0; i < 20; i++)
    {
      enemyTanks.emplace_back(std::make_shared<EnemyTank_40>());
    }
    break;
  }

  for(auto& tank : enemyTanks)
  {
    if(!tank->Init())
    {
      SPDLOG_ERROR("Enemy tank init failed");
      return false;
    }
  }

  player.handleEnemySpawn(kTimePerFrame); // initial start for check game states wouldn't get
                                          // nextLvl because of empty entity field
  return true;
}

void Game::render()
{
  mWindow.clear();
  draw();
  stageRender();

  mWindow.display();
}

void Game::updateStatistics(sf::Time elapsedTime)
{
  mStatisticsUpdateTime += elapsedTime;
  mStatisticsNumFrames += 1;

  if (mStatisticsUpdateTime >= sf::seconds(1.0f))
  {
    auto text = mTextHolder.GetText(EText::STATISTIC);
    if (text)
    {
      text->setString("Frames / Second = " + std::to_string(mStatisticsNumFrames) + "\n"
                      + "Time / Update = " + std::to_string(mStatisticsUpdateTime.asMicroseconds() / static_cast<sf::Int64>(mStatisticsNumFrames)) + "us");
    }
    else
    {
      SPDLOG_ERROR("No text for STATISTIC");
    }

    mStatisticsUpdateTime -= sf::seconds(1.0f);
    mStatisticsNumFrames = 0;
  }
}

void Game::draw()
{
  // Draw for All mEntities
  for (auto itr = mEntities.begin(); itr != mEntities.end(); itr++)
  {
    itr->second->Draw(mWindow);
  }

  // Draw for map
  mapSequence.back()->Draw(mWindow);

  // Draw for Bonuses
  for (auto itrBonus = mBonuses.begin(); itrBonus != mBonuses.end(); itrBonus++)
  {
    itrBonus->second->Draw(mWindow);
  }

  // Draw for Animation
  for (auto itrAnim = mAnimations.begin(); itrAnim != mAnimations.end(); itrAnim++)
  {
    itrAnim->second->Draw(mWindow);
  }

  // Draw for Right Panel
  panel.Draw(mWindow);
}
