//
// Created by pavel on 6/2/20.
//

#ifndef BATTLECITY_SPRITEHOLDER_HPP
#define BATTLECITY_SPRITEHOLDER_HPP

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "ResourceHolder.hpp"
#include "../Enums.hpp"

class SpriteHolder
{
public:
   static bool Init();
   static std::shared_ptr<std::vector<sf::Sprite>> GetSprite(EImage aImage);

private:
   static void insert(EImage aTextureId, const std::shared_ptr<std::vector<sf::Texture>>& aTexturesPtr);

private:
   static ResourceHolder<EImage, std::vector<sf::Texture>> mTextureHolder;
   static ResourceHolder<EImage, std::vector<sf::Sprite>> mSpriteHolder;
};


#endif// BATTLECITY_SPRITEHOLDER_HPP
