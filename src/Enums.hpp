#ifndef BATTLECITY_ENUMS_HPP
#define BATTLECITY_ENUMS_HPP

#include "include/enum.h"

BETTER_ENUM(EImage, unsigned int,  
                            UNKNOWN, 
                            MAIN_SHEET,
                            EAGLE,
                            T_10,
                            ENEMY_10,
                            ENEMY_20,
                            ENEMY_30,
                            ENEMY_40,
                            WALL_1,
                            WALL_2,
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
                            DIGIT_0,
                            DIGIT_1,
                            DIGIT_2,
                            DIGIT_3,
                            DIGIT_4,
                            DIGIT_5,
                            DIGIT_6,
                            DIGIT_7,
                            DIGIT_8,
                            DIGIT_9,
                            APPERANCE)

BETTER_ENUMS_DECLARE_STD_HASH(EImage)

BETTER_ENUM(EFont, unsigned int, SANSATION)

BETTER_ENUMS_DECLARE_STD_HASH(EFont)

BETTER_ENUM(EInputOption, unsigned int, TEST)

BETTER_ENUMS_DECLARE_STD_HASH(EInputOption)

BETTER_ENUM(EText, unsigned int, GAME_OVER, NEXT_LVL, WIN, LOSE, PAUSE, STATISTIC)

BETTER_ENUMS_DECLARE_STD_HASH(EText)

#endif// BATTLECITY_ENUMS_HPP