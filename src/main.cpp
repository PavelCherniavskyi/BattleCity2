#include "Game.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] const char **argv)
{
   Game game;
   game.handleInput(argc, argv);
   return game.Run();
}