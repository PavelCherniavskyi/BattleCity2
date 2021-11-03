#include "../Player.hpp"
#include "../Eagle.hpp"
#include "../Utils/Utils.hpp"

constexpr auto kPlayerStepMove = 100.f;

Player::Player(std::unordered_multimap<ECategory, std::shared_ptr<Entity>>& ent,
  AnimationHandler& a,
  EGamestates& g,
  std::vector<std::shared_ptr<Entity>>& et,
  std::vector<std::shared_ptr<Map>>& map,
  RightPanel& pan,
  BonusHandler& bon)
  : entities(ent)
  , mAnimationHandler(a)
  , gameStage(g)
  , mEnemyTanksQueue(et)
  , mapSequence(map)
  , panel(pan)
  , mBonusHandler(bon)
  , mPlayerTank(std::make_shared<PlayerTank>())
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
        mMouseActionBinding[found->second].get()->MouseAction(mPlayerTank);
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
      mKeyboardMovingBinding[pair.second].get()->KeyboardAction(TimePerFrame, mPlayerTank, false);
      if (isIntersectsPlayerTank())
      {
        mKeyboardMovingBinding[pair.second].get()->KeyboardAction(TimePerFrame, mPlayerTank, true);
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
  if(mBonusHandler.CheckIntersection(mPlayerTank->GetGlobalBounds(), bonusIt))
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
      mPlayerTank->SetHP(mPlayerTank->GetHP() + static_cast<int>(bonusIt->second->GetPackSize()));
      panel.SetCurrentLives(static_cast<std::size_t>(mPlayerTank->GetHP()));
    }
    mBonusHandler.EraseBonus(bonusIt);
  }
}

std::shared_ptr<PlayerTank> Player::getPlayerTank()
{
  return mPlayerTank;
}

void Player::MouseControl::MouseAction(std::shared_ptr<Entity> entity)
{
  std::shared_ptr<BulletBase> bullet(nullptr);
  if (type == +ECategory::BULLET)
  {
    bullet = entity->DoFire(ECategory::BULLET);
  }
  else if (type == +ECategory::SUPERBULLET)
  {
    bullet = entity->DoFire(ECategory::SUPERBULLET);
  }

  if(!bullet)
  {
    return;
  }
  entities.insert({ECategory::BULLET, std::move(bullet)});
}

void Player::initializeActions()
{
  mKeyboardMovingBinding[EActions::LEFT] = std::make_unique<KeyboardControl>(-kPlayerStepMove, 0.f, EActions::LEFT);
  mKeyboardMovingBinding[EActions::RIGHT] = std::make_unique<KeyboardControl>(+kPlayerStepMove, 0.f, EActions::RIGHT);
  mKeyboardMovingBinding[EActions::UP] = std::make_unique<KeyboardControl>(0.f, -kPlayerStepMove, EActions::UP);
  mKeyboardMovingBinding[EActions::DOWN] = std::make_unique<KeyboardControl>(0.f, +kPlayerStepMove, EActions::DOWN);
  mMouseActionBinding[EActions::FIRE] = std::make_unique<MouseControl>(entities, ECategory::BULLET);
  mMouseActionBinding[EActions::SUPERFIRE] = std::make_unique<MouseControl>(entities, ECategory::SUPERBULLET);
}

void Player::initializeObjects()
{
  retBullet = entities.equal_range(ECategory::BULLET);
  retSuperBullet = entities.equal_range(ECategory::SUPERBULLET);
  retEagle = entities.equal_range(ECategory::EAGLE);
  retWall_1 = mapSequence.back()->GetMap().equal_range(EImage::WALL_1);
  retWall_2 = mapSequence.back()->GetMap().equal_range(EImage::WALL_2);
  retMainWall = mapSequence.back()->GetMap().equal_range(EImage::MAINWALL);
  retWaterWall = mapSequence.back()->GetMap().equal_range(EImage::WATERWALL);
}

void Player::KeyboardControl::KeyboardAction(sf::Time time, std::shared_ptr<PlayerTank> aPlayer, bool aMoveBack)
{
  if (aMoveBack)
  {
    aPlayer->MoveBack((Velocity * aPlayer->GetSpeed()) * time.asSeconds());
  }
  else
  {
    aPlayer->SetVelocity(Velocity * aPlayer->GetSpeed());
    aPlayer->Update((Velocity * aPlayer->GetSpeed()) * time.asSeconds());
    aPlayer->Rotate(Side);
  }
}

bool Player::isIntersectsPlayerTank()
{
  // PlayerTank vs Wall_1
  initializeObjects();

  const auto& globalBounds = mPlayerTank->GetGlobalBounds();
  for (auto itrMap = retWall_1.first; itrMap != retWall_1.second; itrMap++)
  {
    if (Utils::Intersection(globalBounds, itrMap->second.Rect))
    {
      return true;
    }
  }

  // PlayerTank vs Wall_2
  for (auto itrMap = retWall_2.first; itrMap != retWall_2.second; itrMap++)
  {
    if (Utils::Intersection(globalBounds, itrMap->second.Rect))
    {
      return true;
    }
  }

  // PlayerTank vs MainWall
  for (auto itrMap = retMainWall.first; itrMap != retMainWall.second; itrMap++)
  {
    if (Utils::Intersection(globalBounds, itrMap->second.Rect))
    {
      return true;
    }
  }

  // PlayerTank vs WaterWall
  for (auto itrMap = retWaterWall.first; itrMap != retWaterWall.second; itrMap++)
  {
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

  auto eagle = std::make_shared<Eagle>();
  if(!eagle->Init() || !mPlayerTank->Init())
  {
    SPDLOG_ERROR("Something wrong with Eagle or Player tank");
    return false;
  }
  entities.insert({ECategory::PLAYERTANK, mPlayerTank});
  entities.insert({ECategory::EAGLE, std::move(eagle)});

  mapSequence.emplace_back(std::make_shared<Map4>());
  mapSequence.emplace_back(std::make_shared<Map3>());
  mapSequence.emplace_back(std::make_shared<Map2>());
  mapSequence.emplace_back(std::make_shared<Map1>());

  for(auto& map : mapSequence)
  {
    if(!map->Init())
    {
      SPDLOG_ERROR("Something wrong with map");
      return false;
    }
  }

  initializeActions();
  initializeObjects();

  return true;
}
