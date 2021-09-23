#include <gtest/gtest.h>
#include <config.h>
#include "SpriteHolder.hpp"
#include "Enums.hpp"
#include "SpriteCoords.hpp"

TEST(ResourceHolder, ResourceHolderFunctionalityTest)
{
   ResourceHolder<Texture, sf::Texture> resourceHolder;
   resourceHolder.load(Texture::MAIN_SHEET, fmt::format("{}/media/Textures/SpriteSheet.png", CMAKE_SOURCE_DIR));
   const sf::Texture& texture = resourceHolder.get(Texture::MAIN_SHEET);
   EXPECT_TRUE(texture.getSize() == sf::Vector2u(TEXTURE_WIDTH, TEXTURE_HEIGHT));
}

TEST(SpriteHolder, SpriteHolderFunctionality)
{
   SpriteHolder spriteHolder;
   spriteHolder.init();
   sf::Sprite sprite = spriteHolder.getSprite(SpriteId::MAIN_TANK1);
   EXPECT_TRUE(SpriteCoords.find(SpriteId::MAIN_TANK1) != SpriteCoords.end());
   EXPECT_TRUE(sprite.getTextureRect() == SpriteCoords.at(SpriteId::MAIN_TANK1));

   sf::Sprite spriteFail = spriteHolder.getSprite(SpriteId::UNKNOWN);
   EXPECT_TRUE(spriteFail.getTextureRect() == sf::IntRect(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT));
}