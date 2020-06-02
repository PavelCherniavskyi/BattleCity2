//
// Created by pavel on 6/2/20.
//

#include "../SpriteHolder.hpp"
#include "config.h"

SpriteHolder::SpriteHolder()
   : mResourceHolder()
{
}

sf::Sprite SpriteHolder::getSprite(SpriteId spriteId)
{
   sf::Sprite sprite;
   sprite.setTexture(mResourceHolder.get(Textures::MAIN_SHEET));

   switch (spriteId)
   {
   case SpriteId::MAIN_TANK1:
      sprite.setTextureRect(sf::IntRect(1, 2, 13, 13));
      break;
   case SpriteId::MAIN_TANK2:
      break;
   }

   return sprite;
}
void SpriteHolder::init()
{
   mResourceHolder.load(Textures::MAIN_SHEET, fmt::format("{}/media/Textures/SpriteSheet.png", CMAKE_SOURCE_DIR));
}
