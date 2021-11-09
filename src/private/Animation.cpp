#include "../Animation.hpp"
#include "../ResourceHolders/SpriteHolder.hpp"

constexpr auto kBulletCollisionSpriteSize = 2u;
constexpr auto kTankCollisionSpriteSize = 3u;
constexpr auto kSuperBulletCollisionSpriteSize = 3u;
constexpr auto kEagleCollisionSpriteSize = 2u;
constexpr auto kApperanceSpriteSize = 4u;


bool Animation::setInit(const float aScale, const float aLifeTime)
{
  auto sprites = SpriteHolder::GetSprite(mImageType);

  if (sprites && sprites->size() == mSpritesCount)
  {
    mSprites = *sprites;

    for (size_t i = 0; i < mSprites.size(); i++)
    {
      mSprites[i].setScale(aScale, aScale);
    }
    for (size_t i = 0; i < mSprites.size(); i++)
    {
      mSprites[i].setOrigin(mSprites[i].getLocalBounds().height / 2.f,
        mSprites[i].getLocalBounds().width / 2.f);
    }

    mTimeOfLiving = aLifeTime;
    mClockForTimeOfLiving.restart();
    mClockForDraw.restart();
  }
  else
  {
    SPDLOG_WARN("Empty pointer, sprites is empty or sprites size doesn't match");
    return false;
  }

  return true;
}

Animation::Animation(const size_t aSpritesCount, const EImage aType)
  : Entity(aType)
  , mClockForTimeOfLiving()
  , mClockForDraw()
  , mIsLiving(true)
  , mTimeOfLiving(0.f)
  , mSpritesCount(aSpritesCount)
{
}

void Animation::Update(const sf::Vector2f&)
{
  sf::Time time = mClockForTimeOfLiving.getElapsedTime();
  if (time.asSeconds() > mTimeOfLiving)
  {
    mIsLiving = false;
  }
}

bool Animation::IsLiving()
{
  return mIsLiving;
}

BulletCollision::BulletCollision()
  : Animation(kBulletCollisionSpriteSize, EImage::BULLETCOLLISION)
{
}

bool BulletCollision::Init()
{
  return setInit(1.f, 0.05f);
}

void BulletCollision::Draw(sf::RenderWindow& window) const
{
  sf::Time time = mClockForDraw.getElapsedTime();
  if (time.asSeconds() < mTimeOfLiving / static_cast<float>(mSpritesCount))
  {
    window.draw(mSprites[0]);
  }
  else if (time.asSeconds() < mTimeOfLiving)
  {
    window.draw(mSprites[1]);
  }
}

TankCollision::TankCollision()
  : Animation(kTankCollisionSpriteSize, EImage::TANKCOLLISION)
{
}

bool TankCollision::Init()
{
  return setInit(2.f, 0.03f);
}

void TankCollision::Draw(sf::RenderWindow& window) const
{
  sf::Time time = mClockForDraw.getElapsedTime();
  auto spritesCount = static_cast<float>(mSpritesCount);
  if (time.asSeconds() < mTimeOfLiving / spritesCount)
  {
    window.draw(mSprites[0]);
  }
  else if (time.asSeconds()
           < (mTimeOfLiving / spritesCount) + (mTimeOfLiving / spritesCount))
  {
    window.draw(mSprites[1]);
  }
  else if (time.asSeconds() < mTimeOfLiving)
  {
    window.draw(mSprites[2]);
  }
}

SuperBulletCollision::SuperBulletCollision()
  : Animation(kSuperBulletCollisionSpriteSize, EImage::SUPERBULLETCOLLISION)
{
}

bool SuperBulletCollision::Init()
{
  return setInit(2.f, 0.3f);
}

void SuperBulletCollision::Draw(sf::RenderWindow& window) const
{
  sf::Time time = mClockForDraw.getElapsedTime();
  auto spritesCount = static_cast<float>(mSpritesCount);
  if (time.asSeconds() < mTimeOfLiving / spritesCount)
  {
    window.draw(mSprites[0]);
  }
  else if (time.asSeconds()
           < (mTimeOfLiving / spritesCount) + (mTimeOfLiving / spritesCount))
  {
    window.draw(mSprites[1]);
  }
  else if (time.asSeconds() < mTimeOfLiving)
  {
    window.draw(mSprites[2]);
  }
}

EagleCollision::EagleCollision()
  : Animation(kEagleCollisionSpriteSize, EImage::EAGLECOLLISION)
{
}

bool EagleCollision::Init()
{
  return setInit(2.f, 0.3f);
}

void EagleCollision::Draw(sf::RenderWindow& window) const
{
  sf::Time time = mClockForDraw.getElapsedTime();
  if (time.asSeconds() < mTimeOfLiving / static_cast<float>(mSpritesCount))
  {
    window.draw(mSprites[0]);
  }
  else if (time.asSeconds() < mTimeOfLiving)
  {
    window.draw(mSprites[1]);
  }
}

Apperance::Apperance()
  : Animation(kApperanceSpriteSize, EImage::APPERANCE)
{
}

bool Apperance::Init()
{
  return setInit(2.f, 0.7f);
}

void Apperance::Draw(sf::RenderWindow& window) const
{
  sf::Time time = mClockForDraw.getElapsedTime();
  auto spritesCount = static_cast<float>(mSpritesCount);
  if (time.asSeconds() < mTimeOfLiving / spritesCount)
  {
    window.draw(mSprites[0]);
  }
  else if (time.asSeconds() < (mTimeOfLiving / spritesCount) + (mTimeOfLiving / spritesCount))
  {
    window.draw(mSprites[1]);
  }
  else if (time.asSeconds()
           < (mTimeOfLiving / spritesCount) + (mTimeOfLiving / spritesCount) + (mTimeOfLiving / spritesCount))
  {
    window.draw(mSprites[2]);
  }
  else if (time.asSeconds() < mTimeOfLiving)
  {
    window.draw(mSprites[3]);
  }
}
