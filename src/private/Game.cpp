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
static const sf::Time TimePerFrame = sf::seconds(1.f/60.f);
static const float PlayerSpeed = 1000.f;


Game::Game()
   : mMainWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), GAME_TITLE)
   , mTestOption(0)
   , mTexture()
   , mPlayer()
   , mFont()
   , mStatisticsText()
   , mStatisticsUpdateTime()
   , mStatisticsNumFrames(0)
   , mIsMovingUp(false)
   , mIsMovingDown(false)
   , mIsMovingRight(false)
   , mIsMovingLeft(false)
{
   //TODO: create resource manager and abort not in constructor
   if(!mTexture.loadFromFile(fmt::format("{}/media/Textures/SpriteSheet.png", CMAKE_SOURCE_DIR)))
   {
      spdlog::critical("texture loading failed");
      abort();
   }
   if(!mFont.loadFromFile(fmt::format("{}/media/Sansation.ttf", CMAKE_SOURCE_DIR)))
   {
      spdlog::critical("font loading failed");
      abort();
   }

   mPlayer.setTexture(mTexture);
   mPlayer.setTextureRect(sf::IntRect(1, 2, 13, 13));
   mPlayer.setPosition(100.f, 100.f);


   mStatisticsText.setFont(mFont);
   mStatisticsText.setPosition(5.f, 5.f);
   mStatisticsText.setCharacterSize(10);
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
      while (timeSinceLastUpdate > TimePerFrame)
      {
         timeSinceLastUpdate -= TimePerFrame;

         processEvents();
         update(TimePerFrame);
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
   sf::Event event;
   while (mMainWindow.pollEvent(event))
   {
      switch (event.type)
      {
      case sf::Event::KeyPressed:
         handlePlayerInput(event.key.code, true);
         break;
      case sf::Event::KeyReleased:
         handlePlayerInput(event.key.code, false);
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
   sf::Vector2f movement(0.f, 0.f);
   if (mIsMovingUp)
   {
      movement.y -= PlayerSpeed;
   }
   if (mIsMovingDown)
   {
      movement.y += PlayerSpeed;
   }
   if (mIsMovingLeft)
   {
      movement.x -= PlayerSpeed;
   }
   if (mIsMovingRight)
   {
      movement.x += PlayerSpeed;
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

   if (mStatisticsUpdateTime >= sf::seconds(1.0f))
   {
      mStatisticsText.setString(fmt::format("Frames / Second = {}\nTime / Update = {}us", mStatisticsNumFrames, static_cast<u_int64_t>(mStatisticsUpdateTime.asMicroseconds()) / mStatisticsNumFrames));

      mStatisticsUpdateTime -= sf::seconds(1.0f);
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
