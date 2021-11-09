#ifndef BATTLECITY_COMMANDS_HPP
#define BATTLECITY_COMMANDS_HPP

#include "Enums.hpp"
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <functional>
#include "Bullet.hpp"
#include "Tank.hpp"

struct KeyboardCommand
{
  KeyboardCommand(float aX, float aY, EActions aSide)
    : Velocity(aX, aY)
    , Side(aSide)
  {
  }

  void Action(const sf::Time&, std::shared_ptr<PlayerTank>, bool);

  sf::Vector2f Velocity;
  EActions Side;
};

struct MouseCommand
{
  MouseCommand(std::function<void(std::shared_ptr<BulletBase>)> aCallback, ECategory aCategory)
    : type(aCategory)
    , mCallback(aCallback)
  {
  }

  void Action(std::shared_ptr<PlayerTank>);

  ECategory type;
  std::function<void(std::shared_ptr<BulletBase>)> mCallback;
};

#endif // BATTLECITY_COMMANDS_HPP