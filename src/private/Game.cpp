#include "../Game.h"

#include "../Definitions.hpp"
#include "../StringHelpers.h"
#include "config.h"

Game::Game(std::unique_ptr<InputHandler> aInputHandlerUPtr)
  : mWindow(sf::VideoMode(kWidthScreen + kWidthRightPanel * 2, kHeightScreen), "Battle City", sf::Style::Close)
  , mIsPaused(false)
  , mTextHolder()
  , mStatisticsUpdateTime()
  , mStatisticsNumFrames(0)
  , entities{}
  , animations{}
  , bonuses{}
  , mapSequence{}
  , player(entities, animations, gameStage, enemyTanks, mapSequence, panel, bonuses)
  , gameStage(Player::Gamestates::runing)
  , enemyTanksQuantity(20)
  , panel(0, enemyTanksQuantity, 1u, 0)
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
      if (gameStage != Player::Gamestates::gameOver && gameStage != Player::Gamestates::win)
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

void Game::Init()
{
  Resourses::Init();
  mTextHolder.Init();
  tankLoad(1);
  mIsPaused = false;
  mWindow.setKeyRepeatEnabled(false);
  player.Init();
  panel.setCurrentMissles(player.getPlayerTank()->superClipSize());
  panel.setCurrentLives(static_cast<std::size_t>(player.getPlayerTank()->getHP()));
  panel.Init();
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
  retBullet = entities.equal_range(Category::Bullet);
  for (auto itrBullet = retBullet.first; itrBullet != retBullet.second; ++itrBullet)
  {
    itrBullet->second->update(itrBullet->second->getVelocity() * elapsedTime.asSeconds());
    if (player.isIntersectsBullet())
    {
      break;
    }
  }
  // std::cout << "update1" << std::endl;
  retSuperBullet = entities.equal_range(Category::SuperBullet);
  // Update for SuperBullets
  for (auto itrSuperBullet = retSuperBullet.first; itrSuperBullet != retSuperBullet.second; ++itrSuperBullet)
  {
    itrSuperBullet->second->update(itrSuperBullet->second->getVelocity() * elapsedTime.asSeconds());
    if (player.isIntersectsSuperBullet())
    {
      break;
    }
  }
  // Update for Enemy tanks
  retEnemyTank = entities.equal_range(Category::EnemyTank);
  for (auto itrTank = retEnemyTank.first; itrTank != retEnemyTank.second; itrTank++)
  {
    itrTank->second->update(itrTank->second->getVelocity() * elapsedTime.asSeconds());
    if (player.isIntersectsEnemy())
    {
      itrTank->second->updateBack(itrTank->second->getVelocity() * elapsedTime.asSeconds());
      player.handleEnemyTanks(itrTank->second);
    }
    player.handleEnemyFire(kTimePerFrame, itrTank->second);
  }

  // Update for Animation
  for (auto itrAnim = animations.begin(); itrAnim != animations.end(); itrAnim++)
  {
    if (itrAnim->second->isAlife())
    {
      itrAnim->second->update();
    }
    else
    {
      auto temp = itrAnim;
      itrAnim->second->~Animation();
      animations.erase(temp);
      itrAnim = animations.begin();
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
  auto checkIfEmpty = entities.find(Category::EnemyTank);
  auto checkIfPlayerAlife = entities.find(Category::PlayerTank);
  if (checkIfEmpty == entities.end() 
      && mapSequence.size() > 1 
      && checkIfPlayerAlife != entities.end() 
      && enemyTanks.empty())
  {
    gameStage = Player::Gamestates::nextLvl;
    forClockRestart = false;
  }
  else if (checkIfEmpty == entities.end() 
      && mapSequence.size() == 1 
      && checkIfPlayerAlife != entities.end() 
      && enemyTanks.empty())
  {
    gameStage = Player::Gamestates::win;
    forClockRestart = false;
    // std::cout << "win" << std::endl;
  }

  if (gameStage == Player::Gamestates::runing)
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

  else if (gameStage == Player::Gamestates::gameOver)
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
  else if (gameStage == Player::Gamestates::nextLvl)
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
  else if (gameStage == Player::Gamestates::win)
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
  mapSequence.pop();
  panel.resetIcons();

  panel.setCurrentLvl(1);
  // std::cout << panel.getCurrentLvl() << std::endl;
  tankLoad(panel.getCurrentLvl());
  auto TankPos = entities.find(Category::PlayerTank);
  Tank *tank = dynamic_cast<Tank *>(TankPos->second);
  tank->setInitialPosition();
  // TankPos->second->rotate(Entity::actions::Up);
  gameStage = Player::Gamestates::runing;
}

void Game::tankLoad(size_t levl)
{
  switch (levl)
  {
  case 1:
    for (int i = 0; i < 18; i++)
    {
      enemyTanks.push(new Tank(Category::EnemyTank, Textures::Enemy_10));
    }
    for (int i = 0; i < 2; i++)
    {
      enemyTanks.push(new Tank(Category::EnemyTank, Textures::Enemy_20));
    }
    break;
  case 2:
    for (int i = 0; i < 14; i++)
    {
      enemyTanks.push(new Tank(Category::EnemyTank, Textures::Enemy_10));
    }
    for (int i = 0; i < 4; i++)
    {
      enemyTanks.push(new Tank(Category::EnemyTank, Textures::Enemy_20));
    }

    for (int i = 0; i < 2; i++)
    {
      enemyTanks.push(new Tank(Category::EnemyTank, Textures::Enemy_30));
    }
    break;
  case 3:
    for (int i = 0; i < 10; i++)
    {
      enemyTanks.push(new Tank(Category::EnemyTank, Textures::Enemy_10));
    }
    for (int i = 0; i < 6; i++)
    {
      enemyTanks.push(new Tank(Category::EnemyTank, Textures::Enemy_20));
    }

    for (int i = 0; i < 4; i++)
    {
      enemyTanks.push(new Tank(Category::EnemyTank, Textures::Enemy_30));
    }
    break;
  case 4:
    for (int i = 0; i < 2; i++)
    {
      enemyTanks.push(new Tank(Category::EnemyTank, Textures::Enemy_10));
    }
    for (int i = 0; i < 8; i++)
    {
      enemyTanks.push(new Tank(Category::EnemyTank, Textures::Enemy_20));
    }

    for (int i = 0; i < 6; i++)
    {
      enemyTanks.push(new Tank(Category::EnemyTank, Textures::Enemy_30));
    }
    for (int i = 0; i < 4; i++)
    {
      enemyTanks.push(new Tank(Category::EnemyTank, Textures::Enemy_40));
    }
    break;
  default:
    for (int i = 0; i < 20; i++)
    {
      enemyTanks.push(new Tank(Category::EnemyTank, Textures::Enemy_40));
    }
    break;
  }

  player.handleEnemySpawn(kTimePerFrame); // initial start for check game states wouldn't get
                                          // nextLvl because of empty entity field
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
      text->setString("Frames / Second = " + toString(mStatisticsNumFrames) + "\n"
                      + "Time / Update = " + toString(mStatisticsUpdateTime.asMicroseconds() / static_cast<sf::Int64>(mStatisticsNumFrames)) + "us");
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
  for (auto itr = entities.begin(); itr != entities.end(); itr++) itr->second->draw(mWindow);

  // Draw for map
  mapSequence.front()->draw(mWindow);

  // Draw for Bonuses
  for (auto itrBonus = bonuses.begin(); itrBonus != bonuses.end(); itrBonus++)
  {
    itrBonus->second->draw(mWindow);
  }

  // Draw for Animation
  for (auto itrAnim = animations.begin(); itrAnim != animations.end(); itrAnim++)
  {
    itrAnim->second->draw(mWindow);
  }

  // Draw for Right Panel
  panel.draw(mWindow);
}

Game::~Game()
{
  for (auto itr = entities.begin(); itr != entities.end(); itr++) itr->second->~Entity();
}
