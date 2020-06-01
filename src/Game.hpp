//
// Created by pavel on 5/31/20.
//

#ifndef BATTLECITY_GAME_HPP
#define BATTLECITY_GAME_HPP


#include <SFML/Graphics.hpp>
#include <stb.h>

class Game
{
public:
   Game();
   void run();
   void handleInput(int argc, const char **argv);
   void processEvents();
   void update(sf::Time elapsedTime);
   void render();
   void updateStatistics(sf::Time elapsedTime);
   void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);

private:
   sf::RenderWindow mMainWindow;
   long mTestOption;

   sf::Texture mTexture;
   sf::Sprite mPlayer;
   sf::Font mFont;
   sf::Text mStatisticsText;
   sf::Time mStatisticsUpdateTime;

   std::size_t mStatisticsNumFrames;
   bool mIsMovingUp;
   bool mIsMovingDown;
   bool mIsMovingRight;
   bool mIsMovingLeft;
};


#endif// BATTLECITY_GAME_HPP
