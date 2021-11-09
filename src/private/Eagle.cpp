#include "../Eagle.hpp"
#include "../ResourceHolders/SpriteHolder.hpp"

constexpr std::size_t kEagleSpriteSize = 2u;

Eagle::Eagle() : Entity(EImage::EAGLE)
{
}

bool Eagle::Init()
{
  auto sprite = SpriteHolder::GetSprite(mImageType);
  if (!sprite || sprite->size() != kEagleSpriteSize)
  {
    SPDLOG_ERROR("Sprite: {}. expected: {}, actual: {}", 
      (sprite ? "valid" : "NOT valid"), 
      kEagleSpriteSize, 
      mSprites.size());
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

  SetHP(1);

  return true;
}

void Eagle::Draw(sf::RenderWindow& window) const
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
