#include "../Bonus.hpp"
#include "../ResourceHolders/SpriteHolder.hpp"

constexpr size_t kBonusSpriteSize = 1u;
constexpr size_t kBonusMisslePackSize = 5u;
constexpr size_t kBonusLifePackSize = 1u;
constexpr size_t kBonusStarPackSize = 1u;

bool setInit(const std::shared_ptr<BaseBonus> aBaseBonusPtr, const float aScale)
{
  std::shared_ptr<std::vector<sf::Sprite>> spritesPtr;

  if (aBaseBonusPtr && (spritesPtr = SpriteHolder::GetSprite(aBaseBonusPtr->mType))
      && spritesPtr->size() == kBonusSpriteSize)
  {
    aBaseBonusPtr->mSprite = spritesPtr->at(0);
    aBaseBonusPtr->mSprite.setScale(aScale, aScale);
  }
  else
  {
    SPDLOG_WARN("Empty pointer or sprite size doesn't match");
    return false;
  }

  return true;
}

BaseBonus::BaseBonus(EImage aType, size_t aPackSize)
  : mSprite()
  , mType(aType)
  , mPackSize(aPackSize)
{
}

void BaseBonus::Draw(sf::RenderWindow& window) const
{
  window.draw(mSprite);
}

void BaseBonus::Update(const sf::Vector2f& vector)
{
  mSprite.move(vector);
}

sf::FloatRect BaseBonus::GetGlobalBounds() const
{
  return mSprite.getGlobalBounds();
}

sf::FloatRect BaseBonus::GetLocalBounds() const
{
  return mSprite.getLocalBounds();
}

void BaseBonus::SetPosition(const float x, const float y)
{
  mSprite.setPosition(x, y);
}

const sf::Sprite& BaseBonus::GetSprite() const
{
  return mSprite;
}
size_t BaseBonus::GetPackSize() const
{
  return mPackSize;
}

EImage BaseBonus::GetType() const
{
  return mType;
}

BonusStar::BonusStar()
  : BaseBonus(EImage::BONUSSTAR, kBonusStarPackSize)
{
}

bool BonusStar::Init()
{
	return setInit(shared_from_this(), 2.f);
}

BonusMissle::BonusMissle()
  : BaseBonus(EImage::BONUSMISSLE, kBonusMisslePackSize)
{
}

bool BonusMissle::Init()
{
  return setInit(shared_from_this(), 2.f);
}

BonusLife::BonusLife()
  : BaseBonus(EImage::BONUSLIFE, kBonusLifePackSize)
{
}

bool BonusLife::Init()
{
  return setInit(shared_from_this(), 2.f);
}
