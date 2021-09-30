#pragma once


const size_t HEIGHT_MAP = 28;
const size_t WIDTH_MAP = 29;
const int WIDTH_SCREEN = 440;
const int HEIGHT_SCREEN = 445;
const size_t WIDTH_RIGHT_PANEL = 25;

namespace Category
{
	enum Type
	{
		Eagle,
		PlayerTank,
		EnemyTank,
		Bullet,
		SuperBullet,
		Bonus
	};
}

namespace TextHolder
{
	enum text{
		nextlvl,
		gameover,
		win,
		pause,
		mStatistics
	};
}

namespace Textures
{
	enum ID
	{
		None,
		Eagle,
		T_10,
		Enemy_10,
		Enemy_20,
		Enemy_30,
		Enemy_40,
		Wall_1,
		Wall_2,
		GreenWall,
		WaterWall,
		MainWall,
		Bullet,
		BonusStar,
		BonusMissle,
		BonusLife,
		BulletCollision,
		TankCollision,
		SuperBulletCollision,
		EagleCollision,
		RightPanel,
		TankIcon,
		Digit_0,
		Digit_1,
		Digit_2,
		Digit_3,
		Digit_4,
		Digit_5,
		Digit_6,
		Digit_7,
		Digit_8,
		Digit_9,
		Apperance
	};
}

