#ifndef BATTLECITY_BONUSHANDLER_HPP
#define BATTLECITY_BONUSHANDLER_HPP

#include "Bonus.hpp"
#include "Enums.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

class BonusHandler
{
public:
  void Draw(sf::RenderWindow&);
  

private:
  std::unordered_multimap<EImage, std::shared_ptr<BaseBonus>> mBonuses;
};

#endif// BATTLECITY_BONUSHANDLER_HPP