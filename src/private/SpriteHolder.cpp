//
// Created by pavel on 6/2/20.
//

#include "../SpriteHolder.hpp"
#include "config.h"
#include "../SpriteCoords.hpp"

constexpr auto KScaleFactor = 2.F;

SpriteHolder::SpriteHolder()
   : mResourceHolder()
{
}

std::unique_ptr<sf::Sprite> SpriteHolder::getSprite(ESpriteId aSpriteId) const
{
  sf::Sprite sprite;
  auto mainSheet = mResourceHolder.get(ETexture::MAIN_SHEET);
  if (mainSheet)
  {
    sprite.setTexture(*mainSheet);
  }


  const auto it = KSpriteCoords.find(aSpriteId);
  if (it != KSpriteCoords.end())
  {
    sprite.setTextureRect(KSpriteCoords.at(aSpriteId));
  }
  else
  {
    spdlog::error("Sprite coords for: {}, not found", aSpriteId._to_string());
  }

  sprite.setScale(KScaleFactor, KScaleFactor);
  return std::make_unique<sf::Sprite>(sprite);
}
void SpriteHolder::init()
{
  mResourceHolder.load(ETexture::MAIN_SHEET, fmt::format("{}/media/Textures/SpriteSheet.png", CMAKE_SOURCE_DIR));
}
