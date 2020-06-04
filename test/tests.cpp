#include <catch2/catch.hpp>
#include <config.h>
#include "SpriteHolder.hpp"

TEST_CASE("ResourceHolder functionality", "[ResourceHolder]")
{
   ResourceHolder<Textures, sf::Texture> resourceHolder;
   resourceHolder.load(Textures::MAIN_SHEET, fmt::format("{}/media/Textures/SpriteSheet.png", CMAKE_SOURCE_DIR));
   const sf::Texture& texture = resourceHolder.get(Textures::MAIN_SHEET);
   REQUIRE(texture.getSize() != sf::Vector2u());
}

TEST_CASE("SpriteHolder functionality", "[SpriteHolder]")
{
   SpriteHolder spriteHolder;
   spriteHolder.init();
   sf::Sprite sprite = spriteHolder.getSprite(SpriteId::MAIN_TANK1);
   REQUIRE(sprite.getTextureRect() != sf::IntRect());
}