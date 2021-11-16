#include "../Entity.hpp"

Entity::Entity(EImage aType)
  : mIsMoving(false)
  , mVelocity{ 0, 0 }
  , mSpeed(0.f)
  , mHP(0)
  , mSprites()
  , mImageType(aType)
{
}
Entity::Entity(const Entity& rhs)
{
  mIsMoving = rhs.mIsMoving;
  mVelocity = rhs.mVelocity;
  mSpeed = rhs.mSpeed;
  mHP = rhs.mHP;
}

bool Entity::IsAlife() const
{
  return mHP > 0;
}

sf::FloatRect Entity::GetGlobalBounds() const
{
  if (!mSprites.empty())
  {
    return mSprites[0].getGlobalBounds();
  }
  return sf::FloatRect();
}

sf::FloatRect Entity::GetLocalBounds() const
{
  if (!mSprites.empty())
  {
    return mSprites[0].getLocalBounds();
  }
  return sf::FloatRect();
}

size_t Entity::MakeDamage(size_t aAmount)
{
  if (aAmount <= mHP)
  {
    mHP -= aAmount;
  }
  else
  {
    mHP = 0u;
  }
  return mHP;
}

void Entity::SetVelocity(const sf::Vector2f& aVelocity)
{
  mVelocity = aVelocity;
}

const sf::Vector2f& Entity::GetVelocity() const
{
  return mVelocity;
}

sf::Vector2f Entity::GetPosition() const
{
  if (!mSprites.empty())
  {
    return mSprites[0].getPosition();
  }
  return sf::Vector2f();
}

const std::vector<sf::Sprite>& Entity::GetSprite() const
{
  return mSprites;
}

void Entity::Rotate(EActions side)
{
  switch (side)
  {
  case +EActions::LEFT:
    for (auto& spr : mSprites) spr.setRotation(270.f);
    break;
  case +EActions::RIGHT:
    for (auto& spr : mSprites) spr.setRotation(90.f);
    break;
  case +EActions::UP:
    for (auto& spr : mSprites) spr.setRotation(0.f);
    break;
  case +EActions::DOWN:
    for (auto& spr : mSprites) spr.setRotation(180.f);
    break;
  default:
    break;
  }
}

void Entity::SetPosition(const sf::FloatRect& rect)
{
  for (auto& spr : mSprites)
  {
    spr.setPosition(rect.left + rect.width / 2, rect.top + rect.height / 2);
  }
}

void Entity::SetPosition(const sf::Vector2f& coords)
{
  for (auto& spr : mSprites)
  {
    spr.setPosition(coords);
  }
}

void Entity::SetSpeed(float aSpeed)
{
  mSpeed = aSpeed;
}

EImage Entity::GetImageType() const
{
  return mImageType;
}

float Entity::GetSpeed() const
{
  return mSpeed;
}

void Entity::SetIsMoving(bool aMoving)
{
  mIsMoving = aMoving;
}

bool Entity::IsMoving() const
{
  return mIsMoving;
}

size_t Entity::GetHP() const
{
  return mHP;
}

void Entity::SetHP(size_t aHP)
{
  mHP = aHP;
}