#include "../RightPanel.h"


RightPanel::RightPanel(size_t liv, size_t quant, size_t levl, size_t mis)
   : resourses(getRes())
   , currentLives(liv)
   , currentLvl(levl)
   , currentMissles(mis)
   , tanksQuantity(quant)

{
	spdlog::info("RightPanel::RightPanel currentMissles:{}", currentMissles);
}

RightPanel::~RightPanel()
{
}

void RightPanel::Init()
{
  panel.setTexture(resourses.getTexturePtr(Textures::RightPanel)[0]);
  panel.setScale(2, 2);
  panel.setPosition(440, 0);

  resetIcons();

  numbers[0].setTexture(resourses.getTexturePtr(Textures::Digit_0)[0]);
  numbers[1].setTexture(resourses.getTexturePtr(Textures::Digit_1)[0]);
  numbers[2].setTexture(resourses.getTexturePtr(Textures::Digit_2)[0]);
  numbers[3].setTexture(resourses.getTexturePtr(Textures::Digit_3)[0]);
  numbers[4].setTexture(resourses.getTexturePtr(Textures::Digit_4)[0]);
  numbers[5].setTexture(resourses.getTexturePtr(Textures::Digit_5)[0]);
  numbers[6].setTexture(resourses.getTexturePtr(Textures::Digit_6)[0]);
  numbers[7].setTexture(resourses.getTexturePtr(Textures::Digit_7)[0]);
  numbers[8].setTexture(resourses.getTexturePtr(Textures::Digit_8)[0]);
  numbers[9].setTexture(resourses.getTexturePtr(Textures::Digit_9)[0]);

  for (size_t i = 0; i < 10; i++)
  {
    numbers[i].setScale(2, 2);
  }

  setNumbers(lives, currentLives);
  setNumbers(lvl, currentLvl);
  setNumbers(missles, currentMissles);
}

void RightPanel::draw(sf::RenderWindow &window)
{
  window.draw(panel);

  for (auto itr = tankIcons.begin(); itr != tankIcons.end(); itr++)
  {
    window.draw(*itr);
  }
  for (size_t i = 0; i < 2; i++)
  {
    window.draw(life[i]);
    window.draw(level[i]);
    window.draw(missle[i]);
  }
}

void RightPanel::popIcon()
{
  if (!tankIcons.empty())
  {
    auto itr = tankIcons.begin();
    tankIcons.erase(itr);
  }
}

void RightPanel::resetIcons()
{
  // std::cout << "resetIcons" << std::endl;
  if (!tankIcons.empty())
  {
    tankIcons.clear();
  }
  bool set = true;
  float X1 = (WIDTH_SCREEN + WIDTH_RIGHT_PANEL) - 20;
  float X2 = (WIDTH_SCREEN + WIDTH_RIGHT_PANEL) - 4;
  float Y = HEIGHT_SCREEN - 400;
  for (size_t i = 0; i < tanksQuantity; i++)
  {
    sf::Sprite spr(resourses.getTexturePtr(Textures::TankIcon)[0]);
    spr.setScale(2, 2);
    if (set)
    {
      spr.setPosition(X1, Y);
      set = false;
    }
    else
    {
      spr.setPosition(X2, Y);
      set = true;
      Y += 16;
    }
    tankIcons.push_back(spr);
  }
}

void RightPanel::setCurrentLives(size_t liv)
{
  setNumbers(windows::lives, liv);
}
void RightPanel::setCurrentMissles(size_t mis)
{
  setNumbers(windows::missles, mis);
}
void RightPanel::setCurrentLvl(size_t l)
{
  setNumbers(windows::lvl, currentLvl += l);
}

void RightPanel::setNumbers(windows win, size_t number)
{

  size_t first = number / 10;
  size_t second = number % 10;

  sf::Sprite spr1(numbers[first]);
  sf::Sprite spr2(numbers[second]);


  switch (win)
  {
  case RightPanel::lives:
    spr1.setPosition(454, 288);
    spr2.setPosition(470, 288);
    life[0] = spr1;
    life[1] = spr2;
    break;
  case RightPanel::lvl:
    spr1.setPosition(444, 400);
    spr2.setPosition(460, 400);
    level[0] = spr1;
    level[1] = spr2;
    break;
  case RightPanel::missles:
    spr1.setPosition(454, 336);
    spr2.setPosition(470, 336);
    missle[0] = spr1;
    missle[1] = spr2;
    break;
  default:
    break;
  }
}
