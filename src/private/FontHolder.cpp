#include "../FontHolder.hpp"
#include "config.h"

FontHolder::FontHolder()
   : mResourceHolder()
{
}

std::shared_ptr<sf::Font> FontHolder::getFont(EFont aFont) const
{
  return mResourceHolder.get(aFont);
}

void FontHolder::init()
{
  mResourceHolder.load(EFont::SANSATION, fmt::format("{}/media/Sansation.ttf", CMAKE_SOURCE_DIR));
}