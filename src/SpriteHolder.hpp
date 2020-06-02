//
// Created by pavel on 6/2/20.
//

#ifndef BATTLECITY_SPRITEHOLDER_HPP
#define BATTLECITY_SPRITEHOLDER_HPP

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "ResourceHolder.hpp"

class SpriteHolder
{
public:
   SpriteHolder();
   void init();
   sf::Sprite getSprite(SpriteId sprite);
private:
   ResourceHolder<Textures, sf::Texture> mResourceHolder;
};


#endif// BATTLECITY_SPRITEHOLDER_HPP
