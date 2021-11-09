
#ifndef BATTLECITY_BONUS_HPP
#define BATTLECITY_BONUS_HPP

#include "Entity.hpp"
#include "Enums.hpp"

class BaseBonus : public Entity
{
public:
  BaseBonus(EImage aType, size_t aPackSize);
  virtual ~BaseBonus() = default;
  void Draw(sf::RenderWindow& window) const override;
  void Update(const sf::Vector2f& vector);
  size_t GetPackSize() const;

protected:
  bool setInit(BaseBonus& aBaseBonus, const float aScale);

private:
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