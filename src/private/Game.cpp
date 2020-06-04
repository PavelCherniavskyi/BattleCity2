//
// Created by pavel on 5/31/20.
//

#include <imgui-SFML.h>
#include <imgui.h>
#include <spdlog/spdlog.h>
#include <fmt/format.h>
#include <map>
#include <docopt/docopt.h>
#include "../Game.hpp"
#include "config.h"

static constexpr auto OPTIONS_USAGE =
   R"(Battle_City_v2.0
  Usage:
      BattleCity
      BattleCity -t <x>
      BattleCity -h | --help
      BattleCity --version

  Options:
      -h --help     Show this screen.
      -t            Test option.
      --version     Show version.
)";

static constexpr auto SCREEN_WIDTH = 1280U;
static constexpr auto SCREEN_HEIGHT = 760U;
static constexpr auto GAME_TITLE = "Battle_City_v2.0";
static constexpr auto cPlayerSpeed = 500.F;
static constexpr auto cSecondsInMinute = 60.F;
static constexpr auto cTextSize = 10U;


Game::Game()
   : mMainWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), GAME_TITLE)
   , mTestOption(0)
   , mStatisticsNumFrames(0)
   , mTimePerFrame(sf::seconds(1.F / cSecondsInMinute))
   , mIsMovingUp(false)
   , mIsMovingDown(false)
   , mIsMovingRight(false)
   , mIsMovingLeft(false)
{
   //TODO: create resource manager for Font and not assert in constructor
   assert(mFont.loadFromFile(fmt::format("{}/media/Sansation.ttf", CMAKE_SOURCE_DIR)));

   mStatisticsText.setFont(mFont);
   mStatisticsText.setPosition(5.F, 5.F);
   mStatisticsText.setCharacterSize(cTextSize);
   mStatisticsText.setStyle(sf::Text::Regular);
}

void Game::run()
{
   sf::Clock clock;
   sf::Time timeSinceLastUpdate = sf::Time::Zero;
   while (mMainWindow.isOpen())
   {
      sf::Time elapsedTime = clock.restart();
      timeSinceLastUpdate += elapsedTime;
      while (timeSinceLastUpdate > mTimePerFrame)
      {
         timeSinceLastUpdate -= mTimePerFrame;

         processEvents();
         update(mTimePerFrame);
      }

      updateStatistics(elapsedTime);
      render();
   }
}

void Game::handleInput(int argc, const char **argv)
{
   std::map<std::string, docopt::value> args = docopt::docopt(OPTIONS_USAGE, { std::next(argv), std::next(argv, argc) }, true, "v2.0");

   if (args["-t"].asBool())
   {
      mTestOption = args["<x>"].asLong();
   }

   spdlog::info("Hello, mTestOption: {}", mTestOption);
}

void Game::processEvents()
{
   sf::Event event{};
   while (mMainWindow.pollEvent(event))
   {
      auto key = event.key;
      switch (event.type)
      {
      case sf::Event::KeyPressed:
         handlePlayerInput(key.code, true);
         break;
      case sf::Event::KeyReleased:
         handlePlayerInput(key.code, false);
         break;
      case sf::Event::Closed:
         mMainWindow.close();
         break;
      default:
         break;
      }
   }
}

void Game::update(sf::Time elapsedTime)
{
   sf::Vector2f movement(0.F, 0.F);
   if (mIsMovingUp)
   {
      movement.y -= cPlayerSpeed;
   }
   if (mIsMovingDown)
   {
      movement.y += cPlayerSpeed;
   }
   if (mIsMovingLeft)
   {
      movement.x -= cPlayerSpeed;
   }
   if (mIsMovingRight)
   {
      movement.x += cPlayerSpeed;
   }

   mPlayer.move(movement * elapsedTime.asSeconds());
}

void Game::render()
{
   mMainWindow.clear();
   mMainWindow.draw(mPlayer);
   mMainWindow.draw(mStatisticsText);
   mMainWindow.display();
}

void Game::updateStatistics(sf::Time elapsedTime)
{
   mStatisticsUpdateTime += elapsedTime;
   mStatisticsNumFrames += 1;

   if (mStatisticsUpdateTime >= sf::seconds(1.F))
   {
      mStatisticsText.setString(fmt::format("Frames / Second = {}\nTime / Update = {}us", mStatisticsNumFrames, static_cast<u_int64_t>(mStatisticsUpdateTime.asMicroseconds()) / mStatisticsNumFrames));

      mStatisticsUpdateTime -= sf::seconds(1.F);
      mStatisticsNumFrames = 0;
   }
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
   spdlog::info("Key:{} is {}", key, (isPressed ? "pressed" : "released"));
   if (key == sf::Keyboard::W)
   {
      mIsMovingUp = isPressed;
   }
   else if (key == sf::Keyboard::S)
   {
      mIsMovingDown = isPressed;
   }
   else if (key == sf::Keyboard::A)
   {
      mIsMovingLeft = isPressed;
   }
   else if (key == sf::Keyboard::D)
   {
      mIsMovingRight = isPressed;
   }
}
void Game::init()
{
   //spdlog::info("Function: {}, File: {}, Line: {}", __FUNCTION__, __FILE__, __LINE__);
   mSpriteHolder.init();

   mPlayer = mSpriteHolder.getSprite(SpriteId::MAIN_TANK1);
   mPlayer.setPosition(100.F, 100.F);
}
