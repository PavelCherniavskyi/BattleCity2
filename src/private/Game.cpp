#include "../Game.h"

#include "../Definitions.hpp"
#include "../ResourceHolders/SpriteHolder.hpp"
#include "config.h"

Game::Game(std::unique_ptr<InputHandler> aInputHandlerUPtr)
  : mWindow(sf::VideoMode(kWidthScreen + kWidthRightPanel * 2, kHeightScreen), "Battle City 2", sf::Style::Close)
  , mIsPaused(false)
  , mTextHolder()
  , mStatisticsUpdateTime()
  , mStatisticsNumFrames(0)
  , entities{}
  , animations{}
  , bonuses{}
  , mapSequence{}
  , player(entities, animations, gameStage, enemyTanks, mapSequence, panel, bonuses)
  , gameStage(EGamestates::RUNNING)
  , enemyTanksQuantity(20)
  , panel(0, enemyTanksQuantity, 1u)
  , mInputHandlerUPtr(std::move(aInputHandlerUPtr))
{
}

void Game::run()
{
  sf::Clock clock;
  sf::Time timeSinceLastUpdate = sf::Time::Zero;
  while (mWindow.isOpen())
  {
    sf::Time elapsedTime = clock.restart();
    timeSinceLastUpdate += elapsedTime;
    while (timeSinceLastUpdate > kTimePerFrame)
    {
      // std::cout << "Check1" << std::endl;
      timeSinceLastUpdate -= kTimePerFrame;
      if (gameStage != +EGamestates::GAME_OVER && gameStage != +EGamestates::WIN)
      {
        handleInput(kTimePerFrame);
        if (mIsPaused)
        {
          continue;
        }

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

  mIsPaused = false;
  mWindow.setKeyRepeatEnabled(false);
  panel.SetCurrentMissles(player.getPlayerTank()->GetSuperClipSize());
  panel.SetCurrentLives(static_cast<std::size_t>(player.getPlayerTank()->GetHP()));
  
  return true;
}

void Game::handleInput(sf::Time aTimePerFrame)
{
  sf::Event event;
  while (mWindow.pollEvent(event))
  {
    if (event.type == sf::Event::Closed)
      mWindow.close();
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
    {
      mIsPaused = !mIsPaused;
      break;
    }

    if (!mIsPaused)
      player.handleEvent(event, aTimePerFrame);
  }
  player.handleRealtimeInput(aTimePerFrame);

  // std::cout << "player.handleRealtimeInput" << std::endl;
}

void Game::update(sf::Time elapsedTime)
{
  Player::rangePtr retBullet;
  Player::rangePtr retSuperBullet;
  Player::rangePtr retEnemyTank;

  // Update for bullets
  retBullet = entities.equal_range(ECategory::BULLET);
  for (auto itrBullet = retBullet.first; itrBullet != retBullet.second; ++itrBullet)
  {
    itrBullet->second->Update(itrBullet->second->GetVelocity() * elapsedTime.asSeconds());
    if (player.isIntersectsBullet())
    {
      break;
    }
  }
  retSuperBullet = entities.equal_range(ECategory::SUPERBULLET);
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
  retEnemyTank = entities.equal_range(ECategory::ENEMYTANK);
  for (auto itrTank = retEnemyTank.first; itrTank != retEnemyTank.second; itrTank++)
  {
    itrTank->second->Update(itrTank->second->GetVelocity() * elapsedTime.asSeconds());
    if (player.isIntersectsEnemy())
    {
      itrTank->second->UpdateBack(itrTank->second->GetVelocity() * elapsedTime.asSeconds());
      player.handleEnemyTanks(itrTank->second);
    }
    player.handleEnemyFire(kTimePerFrame, itrTank->second);
  }

  // Update for Animation
  for (auto itrAnim = animations.begin(); itrAnim != animations.end(); itrAnim++)
  {
    if (itrAnim->second->IsAlife())
    {
      itrAnim->second->update();
    }
    else
    {
      animations.erase(itrAnim);
      break;
    }
  }

  // std::cout << "update5" << std::endl;

  player.handleEnemySpawn(kTimePerFrame);

  // std::cout << "update6" << std::endl;
  player.handleBonusEvents(kTimePerFrame);

  // std::cout << "update7" << std::endl;
  player.isIntersectsOthers();

  // std::cout << "update8" << std::endl;
}

void Game::stageRender()
{
  // Check for game states
  static bool forClockRestart = true;
  static sf::Clock clock;
  if (forClockRestart)
    clock.restart();
  auto checkIfEmpty = entities.find(ECategory::ENEMYTANK);
  auto checkIfPlayerAlife = entities.find(ECategory::PLAYERTANK);
  if (checkIfEmpty == entities.end() 
      && mapSequence.size() > 1 
      && checkIfPlayerAlife != entities.end() 
      && enemyTanks.empty())
  {
    gameStage = EGamestates::NEXT_LVL;
    forClockRestart = false;
  }
  else if (checkIfEmpty == entities.end() 
      && mapSequence.size() == 1 
      && checkIfPlayerAlife != entities.end() 
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
  auto TankPos = entities.find(ECategory::PLAYERTANK);
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
  // Draw for All entities
  for (auto itr = entities.begin(); itr != entities.end(); itr++)
  {
    itr->second->Draw(mWindow);
  }

  // Draw for map
  mapSequence.back()->Draw(mWindow);

  // Draw for Bonuses
  for (auto itrBonus = bonuses.begin(); itrBonus != bonuses.end(); itrBonus++)
  {
    itrBonus->second->Draw(mWindow);
  }

  // Draw for Animation
  for (auto itrAnim = animations.begin(); itrAnim != animations.end(); itrAnim++)
  {
    itrAnim->second->Draw(mWindow);
  }

  // Draw for Right Panel
  panel.Draw(mWindow);
}
