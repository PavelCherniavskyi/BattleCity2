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
  return mHP >= 0;
}

void Entity::Kill()
{
  mHP -= 1;
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

bool Entity::CanIDoFire() const
{
  return false;
}

size_t Entity::GetSuperClipSize() const
{
  return 0u;
}

EImage Entity::GetType() const
{
  return EImage::UNKNOWN;
}

std::shared_ptr<BulletBase> Entity::DoFire(ECategory)
{
  return nullptr;
}

float Entity::GetBulletSpeed() const
{
  return 0.f;
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

int Entity::GetHP() const
{
  return mHP;
}

void Entity::SetHP(int aHP)
{
  mHP = aHP;
}