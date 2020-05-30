#include "OptionsHandler.hpp"
#include "Game.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] const char **argv)
{
   handleInput(argc, argv);

   Game game;
   return game.Run();
}