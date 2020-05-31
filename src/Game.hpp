//
// Created by pavel on 5/31/20.
//

#ifndef BATTLECITY_GAME_HPP
#define BATTLECITY_GAME_HPP


#include <stb.h>
class Game
{
public:
   Game();
   int32 Run();
   void handleInput(int argc, const char **argv);

private:
    sf::RenderWindow mMainWindow;
   long mTestOption;
};


#endif//BATTLECITY_GAME_HPP
