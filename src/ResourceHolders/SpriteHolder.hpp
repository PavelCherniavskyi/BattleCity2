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
   SpriteHolder();
   void Init();
   std::shared_ptr<std::vector<sf::Sprite>> GetSprite(EImage aImage) const;
   std::shared_ptr<std::vector<sf::Texture>> GetTexture(EImage aImage) const;
private:
   void insert(EImage aTextureId, const std::shared_ptr<std::vector<sf::Texture>>& aTexturesPtr);

   ResourceHolder<EImage, std::vector<sf::Sprite>> mSpriteHolder;
   ResourceHolder<EImage, std::vector<sf::Texture>> mTextureHolder;
};


#endif// BATTLECITY_SPRITEHOLDER_HPP
