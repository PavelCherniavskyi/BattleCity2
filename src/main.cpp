#include <spdlog/spdlog.h>
#include "Game.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] const char **argv)
{
   spdlog::set_pattern("[%H:%M:%S.%e][%^-%L-%$][t:%t]%@: %v");

   Game game;
   game.handleInput(argc, argv);
   game.run();

   return 0;
}