#ifndef BATTLECITY_SPRITECOORDS_HPP
#define BATTLECITY_SPRITECOORDS_HPP

#include "Enums.hpp"

constexpr uint16_t TEXTURE_WIDTH = 400U;
constexpr uint16_t TEXTURE_HEIGHT = 256U;

const std::map<SpriteId, sf::IntRect> SpriteCoords
{
   {SpriteId::MAIN_TANK1, sf::IntRect(1, 2, 13, 13)}
};

#endif// BATTLECITY_SPRITECOORDS_HPP