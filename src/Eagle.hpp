#ifndef BATTLECITY_EAGLE_HPP
#define BATTLECITY_EAGLE_HPP

#include "Entity.hpp"

class Eagle : public Entity
{
public:
  Eagle();
  bool Init() override;
  void Draw(sf::RenderWindow &window) override;
  sf::FloatRect GetGlobalBounds() const override;
  sf::FloatRect GetLocalBounds() const override;
  const std::vector<sf::Sprite>& GetSprite() const override;

private:
  std::vector<sf::Sprite> mSprites;
  int mHP;
};


#endif// BATTLECITY_EAGLE_HPP