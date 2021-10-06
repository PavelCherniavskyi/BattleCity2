#ifndef BATTLECITY_DEFINITIONS_HPP
#define BATTLECITY_DEFINITIONS_HPP

#include <SFML/System/Time.hpp>

const sf::Time kTimePerFrame = sf::seconds(1.f / 60.f);
const size_t kHeightMap = 28;
const size_t kWidthMap = 29;
const int kWidthScreen = 440;
const int kHeightScreen = 445;
const size_t kWidthRightPanel = 25;
const auto kRectTileSize = 16u;
const uint16_t kTextureWidth = 400U;
const uint16_t kTextureHeight = 256U;

#endif// BATTLECITY_DEFINITIONS_HPP