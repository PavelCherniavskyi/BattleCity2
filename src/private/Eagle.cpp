#include "../Eagle.hpp"
#include "../ResourceHolders/SpriteHolder.hpp"

constexpr std::size_t kEagleSpriteSize = 2u;

Eagle::Eagle()
  : mSprites{}
{
  mHP = 0;
}

bool Eagle::Init()
{
  auto sprite = SpriteHolder::GetSprite(EImage::EAGLE);
  if (!sprite || mSprites.size() != kEagleSpriteSize)
  {
    
    SPDLOG_WARN("Sprite size doesn't match");
    return false;
  }

  mSprites = *sprite;

  for (size_t i = 0; i < mSprites.size(); i++)
  {
    mSprites[i].setPosition(224.f, 418.f);
  }
  for (size_t i = 0; i < mSprites.size(); i++)
  {
    mSprites[i].setScale(1.7f, 1.7f);
  }

  for (size_t i = 0; i < mSprites.size(); i++)
  {
    mSprites[i].setOrigin(mSprites[i].getLocalBounds().height / 2.f, mSprites[i].getLocalBounds().width / 2.f);
  }

  return true;
}

void Eagle::Draw(sf::RenderWindow& window)
{
  if (IsAlife())
  {
    window.draw(mSprites[0]);
  }
  else
  {
    window.draw(mSprites[1]);
  }
}

sf::FloatRect Eagle::GetGlobalBounds() const
{
  return mSprites[0].getGlobalBounds();
}

sf::FloatRect Eagle::GetLocalBounds() const
{
  return mSprites[0].getLocalBounds();
}

const std::vector<sf::Sprite>& Eagle::GetSprite() const
{
  return mSprites;
}

