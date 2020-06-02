#include <spdlog/spdlog.h>
#include "Game.hpp"

auto main([[maybe_unused]] int argc, [[maybe_unused]] const char **argv) -> int
{
   spdlog::set_pattern("[%H:%M:%S.%e][%^-%L-%$][t:%t]%@: %v");

   Game game;
   game.handleInput(argc, argv);
   game.init();
   game.run();

   return 0;
}