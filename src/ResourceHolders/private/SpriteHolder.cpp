//
// Created by pavel on 6/2/20.
//

#include "../SpriteHolder.hpp"
#include "config.h"

constexpr auto KScaleFactor = 2.F;

ResourceHolder<EImage, std::vector<sf::Texture>> SpriteHolder::mTextureHolder;
ResourceHolder<EImage, std::vector<sf::Sprite>> SpriteHolder::mSpriteHolder;


std::shared_ptr<std::vector<sf::Sprite>> SpriteHolder::GetSprite(EImage aImage)
{
  return mSpriteHolder.Get(aImage);
}

void SpriteHolder::ShutDown()
{
	mTextureHolder.Reset();
	mSpriteHolder.Reset();
}

bool SpriteHolder::Init()
{
	const std::string path = fmt::format("{}/src/media/Textures/SpriteSheet.png", CMAKE_SOURCE_DIR);

  // Player tank
  std::vector<sf::IntRect> playerTankRec = { { 1, 2, 13, 13 }, { 17, 2, 13, 13 } };
  mTextureHolder.LoadFromFile(EImage::T_10, path, playerTankRec);
	insert(EImage::T_10, mTextureHolder.Get(EImage::T_10));

  // Enemies
  std::vector<sf::IntRect> enemy_10 = { { 129, 64, 13, 13 }, { 145, 64, 13, 13 } };
  mTextureHolder.LoadFromFile(EImage::ENEMY_10, path, enemy_10);
	insert(EImage::ENEMY_10, mTextureHolder.Get(EImage::ENEMY_10));
  std::vector<sf::IntRect> enemy_20 = { { 128, 80, 15, 15 }, { 144, 80, 15, 15 } };
  mTextureHolder.LoadFromFile(EImage::ENEMY_20, path, enemy_20);
	insert(EImage::ENEMY_20, mTextureHolder.Get(EImage::ENEMY_20));
  std::vector<sf::IntRect> enemy_30 = { { 128, 48, 16, 16 }, { 144, 48, 16, 16 } };
  mTextureHolder.LoadFromFile(EImage::ENEMY_30, path, enemy_30);
	insert(EImage::ENEMY_30, mTextureHolder.Get(EImage::ENEMY_30));
  std::vector<sf::IntRect> enemy_40 = { { 128, 112, 15, 15 }, { 144, 112, 15, 15 } };
  mTextureHolder.LoadFromFile(EImage::ENEMY_40, path, enemy_40);
	insert(EImage::ENEMY_40, mTextureHolder.Get(EImage::ENEMY_40));

	// Walls
	std::vector<sf::IntRect> mainWall = { { 368, 0, 16, 16 } };
	mTextureHolder.LoadFromFile(EImage::MAINWALL, path, mainWall);
	insert(EImage::MAINWALL, mTextureHolder.Get(EImage::MAINWALL));
	std::vector<sf::IntRect> Wall_1 = { { 256, 64, 8, 8 } };
	mTextureHolder.LoadFromFile(EImage::WALL_1, path, Wall_1);
	insert(EImage::WALL_1, mTextureHolder.Get(EImage::WALL_1));
	std::vector<sf::IntRect> Wall_2 = { { 256, 72, 8, 8 } };
	mTextureHolder.LoadFromFile(EImage::WALL_2, path, Wall_2);
	insert(EImage::WALL_2, mTextureHolder.Get(EImage::WALL_2));
	std::vector<sf::IntRect> GreenWall = { { 264, 72, 8, 8 } };
	mTextureHolder.LoadFromFile(EImage::GREENWALL, path, GreenWall);
	insert(EImage::GREENWALL, mTextureHolder.Get(EImage::GREENWALL));
	std::vector<sf::IntRect> WaterWall = { { 256, 80, 8, 8 } };
	mTextureHolder.LoadFromFile(EImage::WATERWALL, path, WaterWall);
	insert(EImage::WATERWALL, mTextureHolder.Get(EImage::WATERWALL));
	
	// Bullet
	std::vector<sf::IntRect> Bullet = { { 323, 102, 3, 4 } };
	mTextureHolder.LoadFromFile(EImage::BULLET, path, Bullet);
	insert(EImage::BULLET, mTextureHolder.Get(EImage::BULLET));

	// Bonus
	std::vector<sf::IntRect> BonusStar = { { 304, 112, 16, 15 } };
	mTextureHolder.LoadFromFile(EImage::BONUSSTAR, path, BonusStar);
	insert(EImage::BONUSSTAR, mTextureHolder.Get(EImage::BONUSSTAR));
	std::vector<sf::IntRect> BonusLife = { { 336, 112, 16, 15 } };
	mTextureHolder.LoadFromFile(EImage::BONUSLIFE, path, BonusLife);
	insert(EImage::BONUSLIFE, mTextureHolder.Get(EImage::BONUSLIFE));
	std::vector<sf::IntRect> BonusMissle = { { 352, 112, 16, 15 } };
	mTextureHolder.LoadFromFile(EImage::BONUSMISSLE, path, BonusMissle);
	insert(EImage::BONUSMISSLE, mTextureHolder.Get(EImage::BONUSMISSLE));

	// Collisions
	std::vector<sf::IntRect> BulletCollision = { { 256, 128, 16, 16 }, { 272, 128, 16, 16 } };
	mTextureHolder.LoadFromFile(EImage::BULLETCOLLISION, path, BulletCollision);
	insert(EImage::BULLETCOLLISION, mTextureHolder.Get(EImage::BULLETCOLLISION));
	std::vector<sf::IntRect> TankCollision = { { 256, 128, 16, 16 }, { 272, 128, 16, 16 }, { 288, 128, 16, 16 } };
	mTextureHolder.LoadFromFile(EImage::TANKCOLLISION, path, TankCollision);
	insert(EImage::TANKCOLLISION, mTextureHolder.Get(EImage::TANKCOLLISION));
	std::vector<sf::IntRect> SuperBulletCollision = { { 256, 128, 16, 16 }, { 272, 128, 16, 16 }, { 288, 128, 16, 16 } };
	mTextureHolder.LoadFromFile(EImage::SUPERBULLETCOLLISION, path, SuperBulletCollision);
	insert(EImage::SUPERBULLETCOLLISION, mTextureHolder.Get(EImage::SUPERBULLETCOLLISION));
	std::vector<sf::IntRect> EagleCollision = { { 304, 128, 32, 32 }, { 336, 128, 32, 32 } };
	mTextureHolder.LoadFromFile(EImage::EAGLECOLLISION, path, EagleCollision);
	insert(EImage::EAGLECOLLISION, mTextureHolder.Get(EImage::EAGLECOLLISION));

	// Eagle
	std::vector<sf::IntRect> Eagle = { { 304, 32, 16, 16 }, { 320, 32, 16, 16 } };
	mTextureHolder.LoadFromFile(EImage::EAGLE, path, Eagle);
	insert(EImage::EAGLE, mTextureHolder.Get(EImage::EAGLE));

	// Right panel
	std::vector<sf::IntRect> RightPanel = { { 375, 0, 26, 245 } };
	mTextureHolder.LoadFromFile(EImage::RIGHTPANEL, path, RightPanel);
	insert(EImage::RIGHTPANEL, mTextureHolder.Get(EImage::RIGHTPANEL));

	// Tank icon
	std::vector<sf::IntRect> TankIcon = { { 320, 192, 8, 8 } };
	mTextureHolder.LoadFromFile(EImage::TANKICON, path, TankIcon);
	insert(EImage::TANKICON, mTextureHolder.Get(EImage::TANKICON));

	// Digits
	std::vector<sf::IntRect> Digits = { 
		{ 328, 183, 8, 9 },
		{ 337, 183, 7, 9 },
		{ 344, 183, 8, 9 },
		{ 352, 183, 8, 9 },
		{ 360, 183, 8, 9 },
		{ 328, 191, 8, 9 },
		{ 336, 191, 8, 9 },
		{ 344, 191, 8, 9 },
		{ 352, 191, 8, 9 },
		{ 360, 191, 8, 9 }
	};
	mTextureHolder.LoadFromFile(EImage::DIGITS, path, Digits);
	insert(EImage::DIGITS, mTextureHolder.Get(EImage::DIGITS));
	
	// Appearance
	std::vector<sf::IntRect> Apperance = { { 256, 96, 16, 16 }, { 272, 96, 16, 16 }, { 288, 96, 16, 16 }, { 304, 96, 16, 16 } };
	mTextureHolder.LoadFromFile(EImage::APPERANCE, path, Apperance);
	insert(EImage::APPERANCE, mTextureHolder.Get(EImage::APPERANCE));
	return true;
}


void SpriteHolder::insert(EImage aImageId, const std::shared_ptr<std::vector<sf::Texture>>& aTexturesPtr)
{
  if(aTexturesPtr)
  {
		std::vector<sf::Sprite> sprites;
    for(const auto& t : *aTexturesPtr)
    {
      sprites.emplace_back(sf::Sprite(t));
    }
		mSpriteHolder.Insert(aImageId, std::move(sprites));
  }
}