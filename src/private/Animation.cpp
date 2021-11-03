#include "../Animation.hpp"
#include "../ResourceHolders/SpriteHolder.hpp"

constexpr int kMaxSpriteNumber = 4;

bool Animation::setInit(Animation& aAnimation, const float aScale, const float aLifeTime)
{
  auto sprites = SpriteHolder::GetSprite(aAnimation.mType);

  if (sprites && sprites->size() == aAnimation.mSpritesCount)
  {
    aAnimation.mSprites = *sprites;

    for (size_t i = 0; i < aAnimation.mSprites.size(); i++)
    {
      aAnimation.mSprites[i].setScale(aScale, aScale);
    }
    for (size_t i = 0; i < aAnimation.mSprites.size(); i++)
    {
      aAnimation.mSprites[i].setOrigin(aAnimation.mSprites[i].getLocalBounds().height / 2.f,
        aAnimation.mSprites[i].getLocalBounds().width / 2.f);
    }

    aAnimation.mTimeOfLiving = aLifeTime;
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

sf::Vector2f Animation::GetPosition() const
{
  if(!mSprites.empty())
  {
    return mSprites[0].getPosition();
  }
  return sf::Vector2f();
}

BulletCollision::BulletCollision()
  : Animation(2u, EImage::BULLETCOLLISION)
{
}

bool BulletCollision::Init()
{
  return setInit(*this, 1.f, 0.05f);
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

void BulletCollision::SetPosition(const sf::FloatRect& rect)
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
  return setInit(*this, 2.f, 0.03f);
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

void TankCollision::SetPosition(const sf::FloatRect& rect)
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
  return setInit(*this, 2.f, 0.3f);
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

void SuperBulletCollision::SetPosition(const sf::FloatRect& rect)
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
  return setInit(*this, 2.f, 0.3f);
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

void EagleCollision::SetPosition(const sf::FloatRect& rect)
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
  return setInit(*this, 2.f, 0.7f);
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

void Apperance::SetPosition(const sf::FloatRect& rect)
{
  for (size_t i = 0; i < mSpritesCount; i++)
  {
    mSprites[i].setPosition(rect.left + rect.width / 2, rect.top + rect.height / 2);
  }
}
