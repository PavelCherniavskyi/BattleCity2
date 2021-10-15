#include "../Bullet.hpp"
#include "../ResourceHolders/SpriteHolder.hpp"

constexpr auto kBulletSpriteSize = 1u;

bool setInit(std::shared_ptr<BulletBase> aBullet, const float aScale)
{
  auto sprites = SpriteHolder::GetSprite(EImage::BULLET);
  if (!sprites || sprites->size() != kBulletSpriteSize || !aBullet)
  {
    SPDLOG_ERROR("Sprite is incorrect");
    return false;
  }

  aBullet->mSprite = *sprites;
  aBullet->mSprite[0].setScale(aScale, aScale);
  auto height = static_cast<float>(aBullet->mSprite[0].getTextureRect().height);
  auto width = static_cast<float>(aBullet->mSprite[0].getTextureRect().width);
  aBullet->mSprite[0].setOrigin(height / 2.f, width / 2.f);

  return true;
}

bool SimpleBullet::Init()
{
  return setInit(shared_from_this(), 2.f);
}

bool SuperBullet::Init()
{
  return setInit(shared_from_this(), 3.f);
}

void BulletBase::Draw(sf::RenderWindow& window)
{
  window.draw(mSprite[0]);
}

void BulletBase::Update(const sf::Vector2f& aOffset)
{
  mSprite[0].move(aOffset);
}

void BulletBase::Rotate(EActions aAction)
{
  switch (aAction)
  {
  case +EActions::LEFT:
    mSprite[0].setRotation(270.f);
    break;
  case +EActions::RIGHT:
    mSprite[0].setRotation(90.f);
    break;
  case +EActions::UP:
    mSprite[0].setRotation(0.f);
    break;
  case +EActions::DOWN:
    mSprite[0].setRotation(180.f);
    break;
  case +EActions::FIRE:
    break;
  default:
    break;
  }
}

sf::FloatRect BulletBase::GetGlobalBounds() const
{
  return mSprite[0].getGlobalBounds();
}

sf::FloatRect BulletBase::GetLocalBounds() const
{
  return mSprite[0].getLocalBounds();
}

void BulletBase::SetPosition(const sf::Vector2f& aPosition)
{
  mSprite[0].setPosition(aPosition);
}

const std::vector<sf::Sprite>& BulletBase::GetSprite() const
{
  return mSprite;
}
