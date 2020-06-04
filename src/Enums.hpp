#ifndef BATTLECITY_ENUMS_HPP
#define BATTLECITY_ENUMS_HPP

enum class SpriteId
{
   UNKNOWN = -1,
   MAIN_TANK1 = 0,
   MAIN_TANK2
};

enum class Texture
{
   UNKNOWN = -1,
   MAIN_SHEET = 0
};

const char* spriteIdtoString(SpriteId spr);
const char* texturetoString(Texture txt);

#endif// BATTLECITY_ENUMS_HPP