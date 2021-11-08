#include "../Entity.hpp"

Entity::Entity()
  : mMoving(false)
  , mVelocity{0,0}
  , mSpeed(0.f)
  , mHP(0)
{
}
Entity::Entity(const Entity& rhs)
{
  mMoving = rhs.mMoving;
  mVelocity = rhs.mVelocity;
  mSpeed = rhs.mSpeed;
  mHP = rhs.mHP;
}

bool Entity::IsAlife() const
{
  return mHP > 0;
}

size_t Entity::MakeDamage()
{
  if(mHP > 0)
  {
    mHP--;
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

void Entity::SetSpeed(float aSpeed)
{
  mSpeed = aSpeed;
}

EImage Entity::GetImageType() const
{
  return EImage::UNKNOWN;
}

float Entity::GetSpeed() const
{
  return mSpeed;
}

void Entity::SetIsMoving(bool aMoving)
{
  mMoving = aMoving;
}

bool Entity::IsMoving() const
{
  return mMoving;
}

size_t Entity::GetHP() const
{
  return mHP;
}

void Entity::SetHP(size_t aHP)
{
  mHP = aHP;
}