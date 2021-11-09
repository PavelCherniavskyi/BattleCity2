#ifndef BATTLECITY_EAGLE_HPP
#define BATTLECITY_EAGLE_HPP

#include "Entity.hpp"

class Eagle : public Entity
{
public:
  Eagle();
  bool Init() override;
  void Draw(sf::RenderWindow &window) const override;
  void Update(const sf::Vector2f&) override {};
};


#endif// BATTLECITY_EAGLE_HPP