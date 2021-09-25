//
// Created by pavel on 5/31/20.
//

#ifndef BATTLECITY_GAME_HPP
#define BATTLECITY_GAME_HPP


#include <SFML/Graphics.hpp>
#include <stb.h>
#include "SpriteHolder.hpp"
#include "Player.hpp"
#include "InputHandler.hpp"
#include "FontHolder.hpp"

class Game
{
public:
  Game(std::unique_ptr<InputHandler> aInputHandler);
  void run();
  void processEvents();
  void update(sf::Time aElapsedTime);
  void render();
  void updateStatistics(sf::Time aElapsedTime);
  void handlePlayerInput(sf::Keyboard::Key aKey, bool aIsPressed);
  void init();


private:
  sf::RenderWindow mMainWindow;
  std::unique_ptr<InputHandler> mInputHandler;
  FontHolder mFontHolder;
  SpriteHolder mSpriteHolder;
  Player mPlayer; // Should be a class player.
  sf::Text mStatisticsText;
  sf::Time mStatisticsUpdateTime;
  std::size_t mStatisticsNumFrames;
  const sf::Time mTimePerFrame;
  bool mIsMovingUp;
  bool mIsMovingDown;
  bool mIsMovingRight;
  bool mIsMovingLeft;
};


#endif // BATTLECITY_GAME_HPP
