#include "../Bullet.hpp"
#include "../ResourceHolders/SpriteHolder.hpp"

constexpr auto kBulletSpriteSize = 1u;

BulletBase::BulletBase(ECategory type)
  : Entity(Utils::Category2ImageConverter(type))
  , mBulletType(type)
{
}

bool BulletBase::setInit(BulletBase& aBullet, const float aScale)
{
  auto sprites = SpriteHolder::GetSprite(mImageType);
  if (!sprites || sprites->size() != kBulletSpriteSize)
  {
    SPDLOG_ERROR("Sprite is incorrect");
    return false;
  }

  aBullet.mSprites = *sprites;
  aBullet.mSprites[0].setScale(aScale, aScale);
  auto height = static_cast<float>(aBullet.mSprites[0].getTextureRect().height);
  auto width = static_cast<float>(aBullet.mSprites[0].getTextureRect().width);
  aBullet.mSprites[0].setOrigin(height / 2.f, width / 2.f);

  return true;
}

ECategory BulletBase::GetBulletType() const
{
  return mBulletType;
}

SimpleBullet::SimpleBullet() 
  : BulletBase(ECategory::BULLET)
{
}

bool SimpleBullet::Init()
{
  return setInit(*this, 2.f);
}

SuperBullet::SuperBullet() 
  : BulletBase(ECategory::SUPERBULLET)
{
}

bool SuperBullet::Init()
{
  return setInit(*this, 3.f);
}

void BulletBase::Draw(sf::RenderWindow& window) const
{
  window.draw(mSprites[0]);
}

void BulletBase::Update(const sf::Vector2f& aOffset)
{
  mSprites[0].move(aOffset);
}
