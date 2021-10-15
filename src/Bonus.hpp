
#ifndef BATTLECITY_BONUS_HPP
#define BATTLECITY_BONUS_HPP

#include "Entity.hpp"
#include "Enums.hpp"

class BaseBonus : public std::enable_shared_from_this<BaseBonus>
{
public:
  BaseBonus(EImage aType, size_t aPackSize);
  virtual ~BaseBonus() = default;
	virtual bool Init() = 0;
  void Draw(sf::RenderWindow& window) const;
  void Update(const sf::Vector2f& vector);
  void SetPosition(const float x, const float y);
  sf::FloatRect GetGlobalBounds() const;
  sf::Vector2f GetPosition() const;
  sf::FloatRect GetLocalBounds() const;
  const sf::Sprite& GetSprite() const;
  size_t GetPackSize() const;
  EImage GetType() const;

  friend bool setInit(const std::shared_ptr<BaseBonus> aBaseBonusPtr, const float aScale);

private:
  sf::Sprite mSprite;
  EImage mType;
	size_t mPackSize;
};

class BonusStar : public BaseBonus
{
public:
  BonusStar();
  bool Init() override;
};

class BonusMissle : public BaseBonus
{
public:
  BonusMissle();
  bool Init() override;
};

class BonusLife : public BaseBonus
{
public:
  BonusLife();
  bool Init() override;
};


#endif// BATTLECITY_BONUS_HPP