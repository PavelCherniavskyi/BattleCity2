#ifndef BATTLECITY_FONTHOLDER_HPP
#define BATTLECITY_FONTHOLDER_HPP

#include <SFML/Graphics/Font.hpp>
#include "ResourceHolder.hpp"
#include "../Enums.hpp"

class FontHolder
{
public:
  FontHolder();
  bool Init();
  std::shared_ptr<sf::Font> GetFont(EFont aFont) const;

private:
  ResourceHolder<EFont, sf::Font> mResourceHolder;
};


#endif // BATTLECITY_FONTHOLDER_HPP