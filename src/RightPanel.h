#pragma once

#include "ResourceHolder.h"
#include "Entity.h"
#include <vector>

class RightPanel : public Resourses
{
public:
  enum windows
  {
    lives,
    lvl,
    missles
  };


  RightPanel(size_t, size_t, size_t, size_t);
  ~RightPanel();
  void draw(sf::RenderWindow &);
  std::vector<sf::Sprite> getTankIcons() { return tankIcons; }
  void popIcon();
  void resetIcons();
  void setCurrentLives(size_t liv);
  void setCurrentMissles(size_t mis);
  void setCurrentLvl(size_t l);
  size_t getCurrentLvl() { return currentLvl; }
  void Init();

private:
  void setNumbers(windows, size_t);

private:
  ResourceHolder &resourses;
  sf::Sprite panel;
  std::vector<sf::Sprite> tankIcons;
  sf::Sprite numbers[10];
  sf::Sprite life[2];
  sf::Sprite missle[2];
  sf::Sprite level[2];
  size_t currentLives;
  size_t currentLvl;
  size_t currentMissles;
  size_t tanksQuantity;
};
