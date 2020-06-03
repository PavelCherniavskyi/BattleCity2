#include <catch2/catch.hpp>
#include "SpriteHolder.hpp"

TEST_CASE("SpriteHolder functionality", "[SpriteHolder]")
{
   SpriteHolder spriteHolder;
   spriteHolder.init();
   sf::Sprite sprite = spriteHolder.getSprite(SpriteId::MAIN_TANK1);
   REQUIRE(sprite.getTextureRect() != sf::IntRect());
}