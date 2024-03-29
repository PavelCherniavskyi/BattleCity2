#include "../RightPanel.hpp"
#include "../Definitions.hpp"
#include "../ResourceHolders/SpriteHolder.hpp"

constexpr auto kNumbersSize = 10u;
constexpr auto kNumbersScale = 2.f;
constexpr auto kPanelWindosDigitSize = 2u;
constexpr auto kTanksQuantity = 20u;

RightPanel::RightPanel()
  : mPanel()
  , mTankIcons()
  , mNumbers(kNumbersSize)
  , mLife(kPanelWindosDigitSize)
  , mMissle(kPanelWindosDigitSize)
  , mLevel(kPanelWindosDigitSize)
  , mCurrentLives(0)
  , mCurrentLvl(0)
  , mCurrentMissles(0)
{
}

bool RightPanel::Init()
{
  auto rightPanelSprite = SpriteHolder::GetSprite(EImage::RIGHTPANEL);
  if (!rightPanelSprite || rightPanelSprite->empty())
  {
    SPDLOG_ERROR("Sprite is not loaded or empty");
    return false;
  }

  mPanel = rightPanelSprite->at(0);
  mPanel.setScale(2, 2);
  mPanel.setPosition(440, 0);

  auto numbers = SpriteHolder::GetSprite(EImage::DIGITS);
  if (!numbers || numbers->size() != kNumbersSize)
  {
    SPDLOG_ERROR("Sprites is not correct");
    return false;
  }
  mNumbers = *numbers;

  for (size_t i = 0; i < kNumbersSize; ++i)
  {
    mNumbers[i].setScale(kNumbersScale, kNumbersScale);
  }

  ResetIcons();

  setNumbers(EPanelWindow::LIVES, mCurrentLives);
  setNumbers(EPanelWindow::LEVEL, mCurrentLvl);
  setNumbers(EPanelWindow::MISSLES, mCurrentMissles);

  return true;
}

void RightPanel::Draw(sf::RenderWindow& window) const
{
  window.draw(mPanel);

  for (const auto& tank : mTankIcons)
  {
    window.draw(tank);
  }

  for (size_t i = 0; i < kPanelWindosDigitSize; i++)
  {
    window.draw(mLife[i]);
    window.draw(mLevel[i]);
    window.draw(mMissle[i]);
  }
}

void RightPanel::PopIcon()
{
  if (!mTankIcons.empty())
  {
    mTankIcons.erase(mTankIcons.begin());
  }
}

void RightPanel::ResetIcons()
{
  auto sprite = SpriteHolder::GetSprite(EImage::TANKICON);
  if (!sprite || sprite->empty())
  {
    SPDLOG_ERROR("Sprite is empty");
    return;
  }

  bool set = true;
  float X1 = (kWidthScreen + kWidthRightPanel) - 47;
  float X2 = (kWidthScreen + kWidthRightPanel) - 27;
  float Y = kHeightScreen - 420;

  mTankIcons.clear();

  for (size_t i = 0; i < kTanksQuantity; i++)
  {
    sf::Sprite tank = sprite->at(0);
    tank.setScale(2, 2);
    if (set)
    {
      tank.setPosition(X1, Y);
      set = false;
    }
    else
    {
      tank.setPosition(X2, Y);
      set = true;
      Y += 16;
    }
    mTankIcons.emplace_back(std::move(tank));
  }
}

void RightPanel::SetCurrentLives(size_t aLives)
{
  setNumbers(EPanelWindow::LIVES, aLives);
}

void RightPanel::SetCurrentMissles(size_t aMissles)
{
  setNumbers(EPanelWindow::MISSLES, aMissles);
}

void RightPanel::IncrementCurrentLvl()
{
  setNumbers(EPanelWindow::LEVEL, mCurrentLvl += 1);
}

void RightPanel::SetCurrentLevel(size_t aLevel)
{
  mCurrentLvl = aLevel;
  setNumbers(EPanelWindow::LEVEL, mCurrentLvl);
}

size_t RightPanel::GetCurrentLvl() const
{
  return mCurrentLvl;
}

void RightPanel::setNumbersHelper(sf::Sprite& aFirstNumber,
  sf::Sprite& aSecondNumber,
  const sf::Vector2f& aFirstCoords,
  const sf::Vector2f& aSecondCoords,
  std::vector<sf::Sprite>& aTarget)
{
  if (aTarget.size() < kPanelWindosDigitSize)
  {
    SPDLOG_ERROR("Sprite size is incorrect");
    return;
  }

  aFirstNumber.setPosition(aFirstCoords);
  aSecondNumber.setPosition(aSecondCoords);
  aTarget[0] = aFirstNumber;
  aTarget[1] = aSecondNumber;
}

void RightPanel::setNumbers(EPanelWindow aWindow, size_t aNumber)
{
  if (mNumbers.size() < kNumbersSize)
  {
    SPDLOG_ERROR("Sprite size is incorrect");
    return;
  }
  size_t first = aNumber / 10;
  size_t second = aNumber % 10;

  switch (aWindow)
  {
  case +EPanelWindow::LIVES:
    setNumbersHelper(mNumbers[first], mNumbers[second], { 454, 288 }, { 470, 288 }, mLife);
    break;
  case +EPanelWindow::LEVEL:
    setNumbersHelper(mNumbers[first], mNumbers[second], { 444, 400 }, { 460, 400 }, mLevel);
    break;
  case +EPanelWindow::MISSLES:
    setNumbersHelper(mNumbers[first], mNumbers[second], { 454, 336 }, { 470, 336 }, mMissle);
    break;
  default:
    break;
  }
}
