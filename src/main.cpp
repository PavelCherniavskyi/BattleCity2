#include <spdlog/spdlog.h>
#include "Game.h"
#include "InputHandler.hpp"
#include <array>
#include <string>

int main(int argc, char ** argv)
{
   spdlog::set_pattern("[%^-%L-%$][%S.%e][%s:%# %!] %v");

   InputHandler inputHandler;
   if(inputHandler.Parse(argc, argv) == -1)
   {
      return 0;
   }

   Game game(std::make_unique<InputHandler>(inputHandler));
   if(!game.Init())
   {
      return 0;
   }
   
   game.Run();

   return 0;
}