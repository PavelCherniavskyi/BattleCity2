#ifndef BATTLECITY_ANIMATIONHANDLER_HPP
#define BATTLECITY_ANIMATIONHANDLER_HPP

#include "Animation.hpp"
#include "Enums.hpp"

#include <functional>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Rect.hpp>

class AnimationHandler
{
public:
  using AnimationCIterator = std::unordered_multimap<EImage, std::shared_ptr<Animation>>::const_iterator;

  void Draw(sf::RenderWindow&);
  void Update();
  void CreateAnimation(const sf::FloatRect&, EImage, const std::function<void()> = nullptr);
  bool AppearanceIsPending();

private:
  std::unordered_multimap<EImage, std::shared_ptr<Animation>> mAnimations;
  std::vector<std::pair<std::weak_ptr<Animation>, std::function<void()>>> mAppearances;
};

#endif // BATTLECITY_ANIMATIONHANDLER_HPP