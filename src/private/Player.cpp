#include "../Player.hpp"
#include "../Eagle.hpp"
#include "../Utils/Utils.hpp"

constexpr auto kPlayerStepMove = 100.f;

Player::Player(std::vector<std::shared_ptr<Map>>& map, RightPanel& pan, BonusHandler& bon)
  : mapSequence(map)
  , panel(pan)
  , mBonusHandler(bon)
  , mPlayerTank(std::make_shared<PlayerTank>())
  , mNewBulletCallback()
{
}

void Player::HandleActionEvent(const sf::Event& event, sf::Time)
{
  if (event.type == sf::Event::MouseButtonPressed)
  {
    if (auto found = mMousedBinding.find(event.mouseButton.button); found != mMousedBinding.end())
    {
      if (mPlayerTank && mPlayerTank->CanIDoFire())
      {
        mMouseActions[found->second].get()->Action(mPlayerTank);
        panel.SetCurrentMissles(mPlayerTank->GetSuperClipSize());
      }
    }
  }
}

void Player::HandleMovingInput(sf::Time TimePerFrame)
{
  for (auto& pair : mKeyboardBinding)
  {
    if (sf::Keyboard::isKeyPressed(pair.first) && Utils::IsMovingAction(pair.second))
    {
      mKeyboardActions[pair.second].get()->Action(TimePerFrame, mPlayerTank, false);
      if (IsIntersectsWalls())
      {
        mKeyboardActions[pair.second].get()->Action(TimePerFrame, mPlayerTank, true);
      }
      mPlayerTank->SetIsMoving(true);
      break;
    }
    mPlayerTank->SetIsMoving(false);
  }
}

void Player::HandleBonusEvents()
{
  BonusHandler::BonusCIterator bonusIt;
  if (mBonusHandler.CheckIntersection(mPlayerTank->GetGlobalBounds(), bonusIt))
  {
    if (bonusIt->second->GetType() == +EImage::BONUSSTAR)
    {
      mPlayerTank->SetBulletSpeed(mPlayerTank->GetBulletSpeed() + static_cast<float>(bonusIt->second->GetPackSize()));
    }
    else if (bonusIt->second->GetType() == +EImage::BONUSMISSLE)
    {
      mPlayerTank->SuperClipLoad(bonusIt->second->GetPackSize());
      panel.SetCurrentMissles(mPlayerTank->GetSuperClipSize());
    }
    else if (bonusIt->second->GetType() == +EImage::BONUSLIFE)
    {
      mPlayerTank->SetHP(mPlayerTank->GetHP() + bonusIt->second->GetPackSize());
      panel.SetCurrentLives(mPlayerTank->GetHP());
    }
    mBonusHandler.EraseBonus(bonusIt);
  }
}

std::shared_ptr<PlayerTank> Player::GetPlayerTank()
{
  return mPlayerTank;
}


void Player::SetNewBulletCallback(std::function<void(std::shared_ptr<BulletBase>)> aCallback)
{
  mNewBulletCallback = aCallback;
}

bool Player::IsIntersectsWalls()
{
  const auto& globalBounds = mPlayerTank->GetGlobalBounds();
  const auto mapObjects = mapSequence.back()->GetMapObjects();
  for (auto itrMap = mapObjects.begin(); itrMap != mapObjects.end(); itrMap++)
  {
    if(itrMap->second.Type == +EMapObjects::GREENWALL)
    {
      continue;
    }
    if (Utils::Intersection(globalBounds, itrMap->second.Rect))
    {
      return true;
    }
  }

  return false;
}

bool Player::Init()
{
  mKeyboardBinding[sf::Keyboard::A] = EActions::LEFT;
  mKeyboardBinding[sf::Keyboard::D] = EActions::RIGHT;
  mKeyboardBinding[sf::Keyboard::W] = EActions::UP;
  mKeyboardBinding[sf::Keyboard::S] = EActions::DOWN;
  mMousedBinding[sf::Mouse::Left] = EActions::FIRE;
  mMousedBinding[sf::Mouse::Right] = EActions::SUPERFIRE;
  mMousedBinding[sf::Mouse::Middle] = EActions::PAUSE;

  mKeyboardActions[EActions::LEFT] = std::make_unique<KeyboardCommand>(-kPlayerStepMove, 0.f, EActions::LEFT);
  mKeyboardActions[EActions::RIGHT] = std::make_unique<KeyboardCommand>(+kPlayerStepMove, 0.f, EActions::RIGHT);
  mKeyboardActions[EActions::UP] = std::make_unique<KeyboardCommand>(0.f, -kPlayerStepMove, EActions::UP);
  mKeyboardActions[EActions::DOWN] = std::make_unique<KeyboardCommand>(0.f, +kPlayerStepMove, EActions::DOWN);
  mMouseActions[EActions::FIRE] = std::make_unique<MouseCommand>
    ([this](const auto& aBullet) { mNewBulletCallback(aBullet); }, ECategory::BULLET);
  mMouseActions[EActions::SUPERFIRE] = std::make_unique<MouseCommand>
    ([this](const auto& aBullet) { mNewBulletCallback(aBullet); }, ECategory::SUPERBULLET);

  return true;
}
