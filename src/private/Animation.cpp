#include "../Animation.hpp"
#include "../ResourceHolders/SpriteHolder.hpp"

sf::Clock Animation::mClockForTimeOfLiving;
sf::Clock Animation::mClockForDraw;

constexpr int kMaxSpriteNumber = 4;

bool setInit(const std::shared_ptr<Animation> aAnimationPtr, const float aScale, const float aLifeTime)
{
  auto sprites = SpriteHolder::GetSprite(aAnimationPtr->mType);

  if (aAnimationPtr && sprites && sprites->size() == aAnimationPtr->mSpritesCount)
  {
    aAnimationPtr->mSprites = *sprites;

    for (size_t i = 0; i < aAnimationPtr->mSprites.size(); i++)
    {
      aAnimationPtr->mSprites[i].setScale(aScale, aScale);
    }
    for (size_t i = 0; i < aAnimationPtr->mSprites.size(); i++)
    {
      aAnimationPtr->mSprites[i].setOrigin(aAnimationPtr->mSprites[i].getLocalBounds().height / 2.f,
        aAnimationPtr->mSprites[i].getLocalBounds().width / 2.f);
    }

    aAnimationPtr->mTimeOfLiving = aLifeTime;
    Animation::mClockForTimeOfLiving.restart();
    Animation::mClockForDraw.restart();
  }
  else
  {
    SPDLOG_WARN("Empty pointer, sprites is empty or sprites size doesn't match");
    return false;
  }

  return true;
}

Animation::Animation(const size_t aSpritesCount, const EImage aType)
  : mSprites(kMaxSpriteNumber)
  , mIsLiving(true)
  , mTimeOfLiving(0.f)
  , mSpritesCount(aSpritesCount)
  , mType(aType)
{
}

void Animation::update()
{
  sf::Time time = mClockForTimeOfLiving.getElapsedTime();
  if (time.asSeconds() > mTimeOfLiving)
  {
    mIsLiving = false;
  }
}

bool Animation::IsAlife() const
{
  return mIsLiving;
}

BulletCollision::BulletCollision()
  : Animation(2u, EImage::BULLETCOLLISION)
{
}

bool BulletCollision::Init()
{
  return setInit(shared_from_this(), 1.f, 0.05f);
}

void BulletCollision::Draw(sf::RenderWindow& window)
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

void BulletCollision::Bang(const sf::FloatRect& rect)
{
  for (size_t i = 0; i < mSpritesCount; i++)
  {
    mSprites[i].setPosition(rect.left + rect.width / 2, rect.top + rect.height / 2);
  }
}

TankCollision::TankCollision()
  : Animation(3u, EImage::TANKCOLLISION)
{
}

bool TankCollision::Init()
{
  return setInit(shared_from_this(), 2.f, 0.03f);
}

void TankCollision::Draw(sf::RenderWindow& window)
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

void TankCollision::Bang(const sf::FloatRect& rect)
{
  for (size_t i = 0; i < mSpritesCount; i++)
  {
    mSprites[i].setPosition(rect.left + rect.width / 2, rect.top + rect.height / 2);
  }
}

SuperBulletCollision::SuperBulletCollision()
  : Animation(3u, EImage::SUPERBULLETCOLLISION)
{
}

bool SuperBulletCollision::Init()
{
  return setInit(shared_from_this(), 2.f, 0.3f);
}

void SuperBulletCollision::Draw(sf::RenderWindow& window)
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

void SuperBulletCollision::Bang(const sf::FloatRect& rect)
{
  for (size_t i = 0; i < mSpritesCount; i++)
  {
    mSprites[i].setPosition(rect.left + rect.width / 2, rect.top + rect.height / 2);
  }
}

EagleCollision::EagleCollision()
  : Animation(2u, EImage::EAGLECOLLISION)
{
}

bool EagleCollision::Init()
{
  return setInit(shared_from_this(), 2.f, 0.3f);
}

void EagleCollision::Draw(sf::RenderWindow& window)
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

void EagleCollision::Bang(const sf::FloatRect& rect)
{
  for (size_t i = 0; i < mSpritesCount; i++)
  {
    mSprites[i].setPosition(rect.left + rect.width / 2, rect.top + rect.height / 2);
  }
}

Apperance::Apperance()
  : Animation(4u, EImage::APPERANCE)
{
}

bool Apperance::Init()
{
  return setInit(shared_from_this(), 2.f, 0.7f);
}

void Apperance::Draw(sf::RenderWindow& window)
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

void Apperance::Bang(const sf::FloatRect& rect)
{
  for (size_t i = 0; i < mSpritesCount; i++)
  {
    mSprites[i].setPosition(rect.left + rect.width / 2, rect.top + rect.height / 2);
  }
}
