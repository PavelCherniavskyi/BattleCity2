#ifndef BATTLECITY_TEXTHOLDER_HPP
#define BATTLECITY_TEXTHOLDER_HPP

#include <SFML/Graphics/Text.hpp>
#include "ResourceHolder.hpp"
#include "FontHolder.hpp"
#include "../Enums.hpp"

class TextHolder
{
public:
  TextHolder();
  void Init();
  std::shared_ptr<sf::Text> GetText(EText aText) const;

private:
  ResourceHolder<EText, sf::Text> mResourceHolder;
  FontHolder mFontHolder;
};


#endif // BATTLECITY_TEXTHOLDER_HPP