#ifndef BATTLECITY_RIGHTPANEL_HPP
#define BATTLECITY_RIGHTPANEL_HPP

#include "Entity.hpp"
#include "Enums.hpp"

class RightPanel
{
public:
  RightPanel(size_t aLives, size_t aLevel, size_t aMissles);
  void Draw(sf::RenderWindow& aWindow);
  void PopIcon();
  void ResetIcons();
  void SetCurrentLives(size_t aLives);
  void SetCurrentMissles(size_t aMissles);
  void IncrementCurrentLvl();
  size_t GetCurrentLvl() const;
  bool Init();

private:
  void setNumbers(EPanelWindow aWindow, size_t aNumber);
  void setNumbersHelper(sf::Sprite& aFirstNumber,
    sf::Sprite& aSecondNumber,
    const sf::Vector2f& aFirstCoords,
    const sf::Vector2f& aSecondCoords,
    std::vector<sf::Sprite>& aTarget);

private:
  sf::Sprite mPanel;
  std::vector<sf::Sprite> mTankIcons;
  std::vector<sf::Sprite> mNumbers;
  std::vector<sf::Sprite> mLife;
  std::vector<sf::Sprite> mMissle;
  std::vector<sf::Sprite> mLevel;
  size_t mCurrentLives;
  size_t mCurrentLvl;
  size_t mCurrentMissles;
};


#endif// BATTLECITY_RIGHTPANEL_HPP