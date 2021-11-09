#ifndef BATTLECITY_BONUSHANDLER_HPP
#define BATTLECITY_BONUSHANDLER_HPP

#include "Bonus.hpp"
#include "Enums.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

class BonusHandler
{
public:
  using BonusCIterator = std::unordered_multimap<EImage, std::shared_ptr<BaseBonus>>::const_iterator;

  void Draw(sf::RenderWindow&) const;
  void Update();
  bool CheckIntersection(const sf::FloatRect& obj, BonusCIterator& itOut) const;
  void EraseBonus(BonusCIterator);

private:
  std::unordered_multimap<EImage, std::shared_ptr<BaseBonus>> mBonuses;
};

#endif // BATTLECITY_BONUSHANDLER_HPP