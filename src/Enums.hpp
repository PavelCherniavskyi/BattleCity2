#ifndef BATTLECITY_ENUMS_HPP
#define BATTLECITY_ENUMS_HPP

// make default constructor available
// http://aantron.github.io/better-enums/OptInFeatures.html#DefaultConstructors

#ifndef BETTER_ENUMS_DEFAULT_CONSTRUCTOR
#   define BETTER_ENUMS_DEFAULT_CONSTRUCTOR(Enum)                              \
      public:                                                                 \
        Enum() : _value(0) { }
#endif

#include "include/enum.h"

BETTER_ENUM(EImage,
  unsigned int,
  UNKNOWN,
  MAIN_SHEET,
  EAGLE,
  T_10,
  ENEMY_10,
  ENEMY_20,
  ENEMY_30,
  ENEMY_40,
  WALL,
  STEELWALL,
  GREENWALL,
  WATERWALL,
  MAINWALL,
  BULLET,
  BONUSSTAR,
  BONUSMISSLE,
  BONUSLIFE,
  BULLETCOLLISION,
  TANKCOLLISION,
  SUPERBULLETCOLLISION,
  EAGLECOLLISION,
  RIGHTPANEL,
  TANKICON,
  DIGITS,
  APPERANCE)

BETTER_ENUMS_DECLARE_STD_HASH(EImage)

BETTER_ENUM(EFont, unsigned int, SANSATION)

BETTER_ENUMS_DECLARE_STD_HASH(EFont)

BETTER_ENUM(EInputOption, unsigned int, TEST, DEBUG, LEVEL)

BETTER_ENUMS_DECLARE_STD_HASH(EInputOption)

BETTER_ENUM(EText, unsigned int, GAME_OVER, NEXT_LVL, WIN, PAUSE, FPS)

BETTER_ENUMS_DECLARE_STD_HASH(EText)

BETTER_ENUM(EPanelWindow, unsigned int, LIVES, LEVEL, MISSLES)

BETTER_ENUMS_DECLARE_STD_HASH(EPanelWindow)

BETTER_ENUM(ECategory, unsigned int, EAGLE, PLAYERTANK, ENEMYTANK, BULLET, SUPERBULLET, BONUS)

BETTER_ENUMS_DECLARE_STD_HASH(ECategory)

BETTER_ENUM(EActions, unsigned int, LEFT, RIGHT, UP, DOWN, FIRE, SUPERFIRE, PAUSE)

BETTER_ENUMS_DECLARE_STD_HASH(EActions)

BETTER_ENUM(EGamestates, unsigned int, RUNNING, NEXT_LVL, GAME_OVER, WIN)

BETTER_ENUMS_DECLARE_STD_HASH(EGamestates)

BETTER_ENUM(EMapObjects, unsigned int, WALL, STEELWALL, GREENWALL, WATERWALL, MAINWALL)

BETTER_ENUMS_DECLARE_STD_HASH(EMapObjects)

BETTER_ENUM(ELevels, unsigned int, _0, _1, _2, _3, _4)

#endif // BATTLECITY_ENUMS_HPP