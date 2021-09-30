#include <spdlog/spdlog.h>
#include "Game.h"
#include "InputHandler.hpp"

int main(int argc, const char **argv)
{
   spdlog::set_pattern("[%H:%M:%S.%e][%^-%L-%$][t:%t]%@: %v");

   InputHandler inputHandler;
   if(inputHandler.Parse(argc, argv) == -1)
   {
      return 0;
   }

   spdlog::info("before game");
   Game game;
   spdlog::info("game");
   game.Init();
   game.run();

   return 0;
}