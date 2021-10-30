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

inline bool Intersection(const sf::FloatRect& obj1, const sf::FloatRect& obj2)
{
  if (obj1.left + obj1.width >= obj2.left 
      && obj1.top + obj1.height >= obj2.top 
      && obj1.left <= obj2.left + obj2.width
      && obj1.top <= obj2.top + obj2.width)
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


}
#endif// BATTLECITY_UTILS_UTILS_HPP