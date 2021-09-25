#include "../Player.hpp"

Player::Player(const SpriteHolder& aSpriteHolder, ESpriteId aSpriteIconId)
  : mSpriteHolder(aSpriteHolder)
  , mSpriteIconId(aSpriteIconId)
  , mIcon(nullptr)
{
}

void Player::Init()
{
  auto icon = mSpriteHolder.getSprite(mSpriteIconId);
  if(icon)
  {
    mIcon = std::move(icon);
    mIcon->setPosition(100.F, 100.F);
  }
  else
  {
    mIcon.reset(new sf::Sprite());
    spdlog::error("Iconf for player: {}, not found. Set to empty Sprite", mSpriteIconId._to_string());
  }
}

void Player::Move(const sf::Vector2f& aOffset)
{
  mIcon->move(aOffset);
}

void Player::Draw(sf::RenderWindow& aWindow) const
{
  aWindow.draw(*mIcon);
}