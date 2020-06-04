//
// Created by pavel on 6/4/20.
//
#include "../Enums.hpp"

const char* spriteIdtoString(SpriteId spr)
{
   switch (spr)
   {
   case SpriteId::MAIN_TANK1: return "MAIN_TANK1";
   case SpriteId::MAIN_TANK2: return "MAIN_TANK2";
   default:                   return "UNKNOWN";
   }
}

const char* texturetoString(Texture txt)
{
   switch (txt)
   {
   case Texture::MAIN_SHEET: return "MAIN_SHEET";
   default:                  return "UNKNOWN";
   }
}