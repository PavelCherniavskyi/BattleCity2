#ifndef BATTLECITY_PLAYER_HPP
#define BATTLECITY_PLAYER_HPP

#include "Entity.hpp"
#include "Bullet.hpp"
#include "Map.hpp"
#include "Tank.hpp"
#include "BonusHandler.hpp"
#include "AnimationHandler.hpp"
#include "RightPanel.hpp"
#include "Commands.hpp"
#include <SFML/Window/Event.hpp>

class Player
{
public:
  Player(std::vector<std::shared_ptr<Map>>&, RightPanel&, BonusHandler&);
  bool Init();
  void HandleActionEvent(const sf::Event& aEvent, const sf::Time& aTimePerFrame);
  void HandleMovingInput(const sf::Time& aTimePerFrame);
  void HandleBonusEvents();
  bool IsIntersectsWalls();
  std::shared_ptr<PlayerTank> GetPlayerTank();
  void SetNewBulletCallback(std::function<void(std::shared_ptr<BulletBase>)>);

private:
  std::vector<std::shared_ptr<Map>>& mapSequence;
  RightPanel& panel;
  BonusHandler& mBonusHandler;
  std::unordered_map<sf::Keyboard::Key, EActions> mKeyboardBinding;
  std::unordered_map<sf::Mouse::Button, EActions> mMousedBinding;
  std::map<EActions, std::unique_ptr<KeyboardCommand>> mKeyboardActions;
  std::map<EActions, std::unique_ptr<MouseCommand>> mMouseActions;
  std::shared_ptr<PlayerTank> mPlayerTank;
  std::function<void(std::shared_ptr<BulletBase>)> mNewBulletCallback;
};

#endif // BATTLECITY_PLAYER_HPP