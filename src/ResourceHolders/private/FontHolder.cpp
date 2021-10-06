#include "../FontHolder.hpp"
#include "config.h"

FontHolder::FontHolder()
   : mResourceHolder()
{
}

std::shared_ptr<sf::Font> FontHolder::GetFont(EFont aFont) const
{
  return mResourceHolder.Get(aFont);
}

void FontHolder::Init()
{
  mResourceHolder.LoadFromFile(EFont::SANSATION, fmt::format("{}/media/Sansation.ttf", CMAKE_SOURCE_DIR));
}