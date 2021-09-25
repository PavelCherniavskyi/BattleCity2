#include <spdlog/spdlog.h>
#include "Game.hpp"
#include "InputHandler.hpp"

int main(int argc, const char **argv)
{
   spdlog::set_pattern("[%H:%M:%S.%e][%^-%L-%$][t:%t]%@: %v");

   InputHandler inputHandler;
   if(inputHandler.Parse(argc, argv) == -1)
   {
      return 0;
   }

   Game game(std::make_unique<InputHandler>(inputHandler));
   game.init();
   game.run();

   return 0;
}