#include <gtest/gtest.h>
#include <config.h>
#include "SpriteHolder.hpp"
#include "Enums.hpp"
#include "SpriteCoords.hpp"

TEST(ResourceHolder, ResourceHolderFunctionalityTest)
{
   ResourceHolder<ETexture, sf::Texture> resourceHolder;
   resourceHolder.load(ETexture::MAIN_SHEET, fmt::format("{}/media/Textures/SpriteSheet.png", CMAKE_SOURCE_DIR));
   auto texture = resourceHolder.get(ETexture::MAIN_SHEET);
   ASSERT_TRUE(texture);
   EXPECT_TRUE(texture->getSize() == sf::Vector2u(TEXTURE_WIDTH, TEXTURE_HEIGHT));
}

TEST(SpriteHolder, SpriteHolderFunctionality)
{
   SpriteHolder spriteHolder;
   spriteHolder.init();
   auto sprite = spriteHolder.getSprite(ESpriteId::MAIN_TANK1);
   EXPECT_TRUE(KSpriteCoords.find(ESpriteId::MAIN_TANK1) != KSpriteCoords.end());
   EXPECT_TRUE(sprite->getTextureRect() == KSpriteCoords.at(ESpriteId::MAIN_TANK1));

   auto spriteFail = spriteHolder.getSprite(ESpriteId::UNKNOWN);
   EXPECT_TRUE(spriteFail->getTextureRect() == sf::IntRect(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT));
}