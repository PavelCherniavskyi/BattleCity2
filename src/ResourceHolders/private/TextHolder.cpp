#include "../TextHolder.hpp"

#include "config.h"

TextHolder::TextHolder() : mResourceHolder() {}

std::shared_ptr<sf::Text> TextHolder::GetText(EText aText) const 
{
  return mResourceHolder.Get(aText);
}

bool TextHolder::Init()
{
  std::shared_ptr<sf::Font> sansationFont = nullptr;
  if (!mFontHolder.Init() || !(sansationFont = mFontHolder.GetFont(EFont::SANSATION)))
  {
    SPDLOG_ERROR("Font is not found.");
    return false;
  }

  sf::Text mStatisticsText;
  mStatisticsText.setFont(*sansationFont);
  mStatisticsText.setPosition(5.f, 2.f);
  mStatisticsText.setCharacterSize(10);
  mResourceHolder.Insert(EText::FPS, std::move(mStatisticsText));

  sf::Text gameOver("Game Over!!!", *sansationFont, 60);
  gameOver.setStyle(sf::Text::Bold);
  gameOver.setPosition(60.f, 190.f);
  mResourceHolder.Insert(EText::GAME_OVER, std::move(gameOver));

  sf::Text nextLvl("Next Level", *sansationFont, 60);
  nextLvl.setStyle(sf::Text::Bold);
  nextLvl.setPosition(105.f, 190.f);
  mResourceHolder.Insert(EText::NEXT_LVL, std::move(nextLvl));

  sf::Text win("You win!!!", *sansationFont, 60);
  win.setStyle(sf::Text::Bold);
  win.setPosition(120.f, 190.f);
  mResourceHolder.Insert(EText::WIN, std::move(win));

  sf::Text pause("Pause", *sansationFont, 60);
  pause.setStyle(sf::Text::Bold);
  pause.setPosition(130.f, 190.f);
  mResourceHolder.Insert(EText::PAUSE, std::move(pause));

  return true;
}