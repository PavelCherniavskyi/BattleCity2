#ifndef BATTLECITY_UTILS_UTILS_HPP
#define BATTLECITY_UTILS_UTILS_HPP

#include "../Enums.hpp"
#include <type_traits>
#include <SFML/Graphics/Rect.hpp>

namespace Utils{

inline bool IsMovingAction(EActions action)
{
  switch (action)
  {
  case +EActions::LEFT:
  case +EActions::RIGHT:
  case +EActions::UP:
  case +EActions::DOWN: 
    return true;

  default:
    return false;
  }
}

inline EMapObjects Image2MapObjectConverter(EImage image)
{
  switch (image)
  {
  case +EImage::WALL:       return EMapObjects::WALL;
  case +EImage::STEELWALL:  return EMapObjects::STEELWALL;
  case +EImage::WATERWALL:  return EMapObjects::WATERWALL;
  case +EImage::GREENWALL:  return EMapObjects::GREENWALL;
  case +EImage::MAINWALL:   return EMapObjects::MAINWALL;

  default:
    return EMapObjects::MAINWALL;
  }
}

inline EImage MapObject2ImageConverter(EMapObjects mapObject)
{
  switch (mapObject)
  {
  case +EMapObjects::WALL:       return EImage::WALL;
  case +EMapObjects::STEELWALL:  return EImage::STEELWALL;
  case +EMapObjects::WATERWALL:  return EImage::WATERWALL;
  case +EMapObjects::GREENWALL:  return EImage::GREENWALL;
  case +EMapObjects::MAINWALL:   return EImage::MAINWALL;

  default:
    return EImage::MAINWALL;
  }
}

inline EImage Category2ImageConverter(ECategory category)
{
  switch (category)
  {
  case +ECategory::EAGLE:       return EImage::EAGLE;
  case +ECategory::PLAYERTANK:  return EImage::T_10;
  case +ECategory::BULLET:  return EImage::BULLET;
  case +ECategory::SUPERBULLET:  return EImage::BULLET;

  default:
    return EImage::UNKNOWN;
  }
}

inline bool Intersection(const sf::FloatRect& obj1, const sf::FloatRect& obj2)
{
  // if (obj1.left + obj1.width >= obj2.left 
  //     && obj1.top + obj1.height >= obj2.top 
  //     && obj1.left <= obj2.left + obj2.width
  //     && obj1.top <= obj2.top + obj2.width)
  if(obj1.intersects(obj2))
  {
    return true;
  }
  else
  {
    return false;
  }
}

template <typename T, typename _ = void>
struct is_vector { 
    static const bool value = false;
};

template <typename T>
struct is_vector< T, typename std::enable_if_t<std::is_same_v<T, std::vector<typename T::value_type>>>>
{
    static const bool value = true;
};

inline ELevels Num2Elevels(size_t aLevel)
{
  switch (aLevel)
  {
  case 0: return ELevels::_0;
  case 1: return ELevels::_1;
  case 2: return ELevels::_2;
  case 3: return ELevels::_3;
  case 4: return ELevels::_4;
  default: return ELevels::_0;
  }
}


}
#endif// BATTLECITY_UTILS_UTILS_HPP