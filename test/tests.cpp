#include <catch2/catch.hpp>
#include <config.h>
#include "SpriteHolder.hpp"
#include "Enums.hpp"
#include "SpriteCoords.hpp"

TEST_CASE("ResourceHolder functionality", "[ResourceHolder]")
{
   ResourceHolder<Texture, sf::Texture> resourceHolder;
   resourceHolder.load(Texture::MAIN_SHEET, fmt::format("{}/media/Textures/SpriteSheet.png", CMAKE_SOURCE_DIR));
   const sf::Texture& texture = resourceHolder.get(Texture::MAIN_SHEET);
   REQUIRE(texture.getSize() == sf::Vector2u(TEXTURE_WIDTH, TEXTURE_HEIGHT));
}

TEST_CASE("SpriteHolder functionality", "[SpriteHolder]")
{
   SpriteHolder spriteHolder;
   spriteHolder.init();
   sf::Sprite sprite = spriteHolder.getSprite(SpriteId::MAIN_TANK1);
   REQUIRE(SpriteCoords.find(SpriteId::MAIN_TANK1) != SpriteCoords.end());
   REQUIRE(sprite.getTextureRect() == SpriteCoords.at(SpriteId::MAIN_TANK1));

   sf::Sprite spriteFail = spriteHolder.getSprite(SpriteId::UNKNOWN);
   REQUIRE(spriteFail.getTextureRect() == sf::IntRect(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT));
}