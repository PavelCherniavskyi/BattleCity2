//
// Created by pavel on 6/2/20.
//

#include "../SpriteHolder.hpp"
#include "config.h"
#include "../SpriteCoords.hpp"

constexpr auto SCALE_FACTOR = 2.F;

SpriteHolder::SpriteHolder()
   : mResourceHolder()
{
}

sf::Sprite SpriteHolder::getSprite(SpriteId spriteId)
{
   sf::Sprite sprite;
   sprite.setTexture(mResourceHolder.get(Texture::MAIN_SHEET));

   const auto& it = SpriteCoords.find(spriteId);
   if (it != SpriteCoords.end())
   {
      sprite.setTextureRect(SpriteCoords.at(spriteId));
   } else
   {
      spdlog::warn("Sprite coords for: {}, not found", spriteIdtoString(spriteId));
   }

   sprite.setScale(SCALE_FACTOR, SCALE_FACTOR);
   return sprite;
}
void SpriteHolder::init()
{
   mResourceHolder.load(Texture::MAIN_SHEET, fmt::format("{}/media/Textures/SpriteSheet.png", CMAKE_SOURCE_DIR));
}
