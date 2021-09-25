//
// Created by pavel on 6/2/20.
//

#ifndef BATTLECITY_SPRITEHOLDER_HPP
#define BATTLECITY_SPRITEHOLDER_HPP

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "ResourceHolder.hpp"
#include "Enums.hpp"

class SpriteHolder
{
public:
   SpriteHolder();
   void init();
   std::unique_ptr<sf::Sprite> getSprite(ESpriteId aSpriteId) const;
private:
   ResourceHolder<ETexture, sf::Texture> mResourceHolder;
};


#endif// BATTLECITY_SPRITEHOLDER_HPP
