#include "../Bonus.hpp"
#include "../ResourceHolders/SpriteHolder.hpp"

constexpr size_t kBonusSpriteSize = 1u;
constexpr size_t kBonusMisslePackSize = 5u;
constexpr size_t kBonusLifePackSize = 1u;
constexpr size_t kBonusStarPackSize = 1u;

bool BaseBonus::setInit(BaseBonus& aBaseBonus, const float aScale)
{
  if (auto spritesPtr = SpriteHolder::GetSprite(aBaseBonus.mImageType)
      ; spritesPtr->size() == kBonusSpriteSize)
  {
    aBaseBonus.mSprites = *spritesPtr;
    aBaseBonus.mSprites[0].setScale(aScale, aScale);
  }
  else
  {
    SPDLOG_WARN("Empty pointer or sprite size doesn't match");
    return false;
  }

  return true;
}

BaseBonus::BaseBonus(EImage aType, size_t aPackSize)
  : Entity(aType)
  , mPackSize(aPackSize)
{
}

void BaseBonus::Draw(sf::RenderWindow& window) const
{
  window.draw(mSprites[0]);
}

void BaseBonus::Update(const sf::Vector2f& vector)
{
  mSprites[0].move(vector);
}

size_t BaseBonus::GetPackSize() const
{
  return mPackSize;
}

BonusStar::BonusStar()
  : BaseBonus(EImage::BONUSSTAR, kBonusStarPackSize)
{
}

bool BonusStar::Init()
{
	return setInit(*this, 2.f);
}

BonusMissle::BonusMissle()
  : BaseBonus(EImage::BONUSMISSLE, kBonusMisslePackSize)
{
}

bool BonusMissle::Init()
{
  return setInit(*this, 2.f);
}

BonusLife::BonusLife()
  : BaseBonus(EImage::BONUSLIFE, kBonusLifePackSize)
{
}

bool BonusLife::Init()
{
  return setInit(*this, 2.f);
}
