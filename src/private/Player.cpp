#include "../Player.hpp"
#include "../Eagle.hpp"
#include "../Utils/Utils.hpp"

constexpr auto kPlayerStepMove = 100.f;
constexpr auto KEnemyTanksOnFieldNumber = 4u;

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
  , spawnEnemyTanksTime(2)
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

void Player::handleEnemyTanks(std::shared_ptr<Entity> entity)
{
  int choice(std::rand() % 4);
  if (choice == 0)
  {
    entity->SetVelocity({0.f, +100.f * entity->GetSpeed()});
    entity->Rotate(EActions::DOWN);
  }
  else if (choice == 1)
  {
    entity->SetVelocity({0.f, -100.f * entity->GetSpeed()});
    entity->Rotate(EActions::UP);
  }
  else if (choice == 2)
  {
    entity->SetVelocity({+100 * entity->GetSpeed(), 0.f});
    entity->Rotate(EActions::RIGHT);
  }
  else if (choice == 3)
  {
    entity->SetVelocity({-100 * entity->GetSpeed(), 0.f});
    entity->Rotate(EActions::LEFT);
  }
}

void Player::handleEnemySpawn(sf::Time)
{
  static sf::Clock clock;
  static float spawn = 0.f; // for initial start
  sf::Time time = clock.getElapsedTime();

  if (time.asSeconds() > spawn)
  {
    if (!mEnemyTanksQueue.empty())
    {
      const auto enemyTanksOnField = entities.count(ECategory::ENEMYTANK);
      if (enemyTanksOnField < KEnemyTanksOnFieldNumber && !mAnimationHandler.AppearanceIsPending())
      {
        auto popEnemyTank = [this]() {
          entities.emplace(ECategory::ENEMYTANK, mEnemyTanksQueue.back());
          panel.PopIcon();
          mEnemyTanksQueue.pop_back();
          clock.restart();
          spawn = spawnEnemyTanksTime;
        };

        const auto rect = mEnemyTanksQueue.back()->GetGlobalBounds();
        mAnimationHandler.CreateAnimation(rect, EImage::APPERANCE, popEnemyTank);
      }
    }
  }
}

void Player::handleEnemyFire(sf::Time, std::shared_ptr<Entity> entity)
{
  if (entity->CanIDoFire())
  {
    if (entity->GetType() == +EImage::ENEMY_40)
    {
      mMouseActionBinding[EActions::SUPERFIRE].get()->MouseAction(entity);
    }
    else
    {
      mMouseActionBinding[EActions::FIRE].get()->MouseAction(entity);
    }
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
  retEnemy = entities.equal_range(ECategory::ENEMYTANK);
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

bool Player::isIntersectsBullet()
{
  initializeObjects();
  // Bullet vs Wall_1
  for (auto itrBullet = retBullet.first; itrBullet != retBullet.second; ++itrBullet)
  {
    for (auto itrMap = retWall_1.first; itrMap != retWall_1.second; itrMap++)
    {
      if (Utils::Intersection(itrBullet->second->GetGlobalBounds(), itrMap->second.Rect))
      {
        mAnimationHandler.CreateAnimation(itrBullet->second->GetGlobalBounds(), EImage::BULLETCOLLISION);
        mapSequence.back()->GetMap().erase(itrMap);
        entities.erase(itrBullet);
        initializeObjects();
        return true;
      }
    }
  }
  // Bullet vs Wall_2
  for (auto itrBullet = retBullet.first; itrBullet != retBullet.second; ++itrBullet)
  {
    for (auto itrMap = retWall_2.first; itrMap != retWall_2.second; itrMap++)
    {
      if (Utils::Intersection(itrBullet->second->GetGlobalBounds(), itrMap->second.Rect))
      {
        mAnimationHandler.CreateAnimation(itrBullet->second->GetGlobalBounds(), EImage::BULLETCOLLISION);
        entities.erase(itrBullet);
        initializeObjects();
        return true;
      }
    }
  }
  // Bullet vs MainWall
  for (auto itrBullet = retBullet.first; itrBullet != retBullet.second; ++itrBullet)
  {
    for (auto itrMap = retMainWall.first; itrMap != retMainWall.second; itrMap++)
    {
      if (Utils::Intersection(itrBullet->second->GetGlobalBounds(), itrMap->second.Rect))
      {
        mAnimationHandler.CreateAnimation(itrBullet->second->GetGlobalBounds(), EImage::BULLETCOLLISION);
        entities.erase(itrBullet);
        initializeObjects();
        return true;
      }
    }
  }

  // Bullet vs EnemyTank
  for (auto itrTank = retEnemy.first; itrTank != retEnemy.second; ++itrTank)
  {
    for (auto itrBullet = retBullet.first; itrBullet != retBullet.second; ++itrBullet)
    {
      if (Utils::Intersection(itrTank->second->GetGlobalBounds(), itrBullet->second->GetGlobalBounds()))
      {
        mAnimationHandler.CreateAnimation(itrBullet->second->GetGlobalBounds(), EImage::BULLETCOLLISION);
        entities.erase(itrBullet);
        itrTank->second->Kill();
        initializeObjects();
        if (!itrTank->second->IsAlife())
        {
          mAnimationHandler.CreateAnimation(itrTank->second->GetGlobalBounds(), EImage::TANKCOLLISION);
          entities.erase(itrTank);
          initializeObjects();
        }
        return true;
      }
    }
  }

  // Bullet vs PlayerTank
  for (auto itrBullet = retBullet.first; itrBullet != retBullet.second; ++itrBullet)
  {
    if (Utils::Intersection(mPlayerTank->GetGlobalBounds(), itrBullet->second->GetGlobalBounds()))
    {
      mAnimationHandler.CreateAnimation(itrBullet->second->GetGlobalBounds(), EImage::BULLETCOLLISION);
      entities.erase(itrBullet);
      mPlayerTank->Kill();
      initializeObjects();
      if (!mPlayerTank->IsAlife())
      {
        mAnimationHandler.CreateAnimation(mPlayerTank->GetGlobalBounds(), EImage::TANKCOLLISION);
        entities.erase(ECategory::PLAYERTANK);
        gameStage = EGamestates::GAME_OVER;
        initializeObjects();
      }
      else
      {
        auto lives = mPlayerTank->GetHP();
        if (lives < 0)
        {
          lives = 0;
        }

        panel.SetCurrentLives(static_cast<std::size_t>(lives));
      }

      return true;
    }
  }

  // Bullet vs Eagle
  auto eagle = entities.find(ECategory::EAGLE);
  for (auto itrBullet = retBullet.first; itrBullet != retBullet.second; ++itrBullet)
  {
    if (Utils::Intersection(eagle->second->GetGlobalBounds(), itrBullet->second->GetGlobalBounds()))
    {
      mAnimationHandler.CreateAnimation(itrBullet->second->GetGlobalBounds(), EImage::BULLETCOLLISION);
      mAnimationHandler.CreateAnimation(eagle->second->GetGlobalBounds(), EImage::EAGLECOLLISION);
      eagle->second->Kill();
      entities.erase(itrBullet);
      gameStage = EGamestates::GAME_OVER;
      initializeObjects();
      return true;
    }
  }

  return false;
}

bool Player::isIntersectsSuperBullet()
{
  initializeObjects();
  // SuperBullet vs Wall_1
  for (auto itrSuperBullet = retSuperBullet.first; itrSuperBullet != retSuperBullet.second; ++itrSuperBullet)
  {
    for (auto itrMap = retWall_1.first; itrMap != retWall_1.second; itrMap++)
    {
      if (Utils::Intersection(itrSuperBullet->second->GetGlobalBounds(), itrMap->second.Rect))
      {
        mAnimationHandler.CreateAnimation(itrSuperBullet->second->GetGlobalBounds(), EImage::SUPERBULLETCOLLISION);
        mapSequence.back()->GetMap().erase(itrMap);
        entities.erase(itrSuperBullet);
        initializeObjects();
        return true;
      }
    }
  }
  // SuperBullet vs Wall_2
  for (auto itrSuperBullet = retSuperBullet.first; itrSuperBullet != retSuperBullet.second; ++itrSuperBullet)
  {
    for (auto itrMap = retWall_2.first; itrMap != retWall_2.second; itrMap++)
    {
      if (Utils::Intersection(itrSuperBullet->second->GetGlobalBounds(), itrMap->second.Rect))
      {
        mAnimationHandler.CreateAnimation(itrSuperBullet->second->GetGlobalBounds(), EImage::SUPERBULLETCOLLISION);
        mapSequence.back()->GetMap().erase(itrMap);
        entities.erase(itrSuperBullet);
        initializeObjects();
        return true;
      }
    }
  }
  // SuperBullet vs MainWall
  for (auto itrSuperBullet = retSuperBullet.first; itrSuperBullet != retSuperBullet.second; ++itrSuperBullet)
  {
    for (auto itrMap = retMainWall.first; itrMap != retMainWall.second; itrMap++)
    {
      if (Utils::Intersection(itrSuperBullet->second->GetGlobalBounds(), itrMap->second.Rect))
      {
        mAnimationHandler.CreateAnimation(itrSuperBullet->second->GetGlobalBounds(), EImage::SUPERBULLETCOLLISION);
        entities.erase(itrSuperBullet);
        initializeObjects();
        return true;
      }
    }
  }

  // SuperBullet vs EnemyTank
  for (auto itrTank = retEnemy.first; itrTank != retEnemy.second; ++itrTank)
  {
    for (auto itrSuperBullet = retSuperBullet.first; itrSuperBullet != retSuperBullet.second; ++itrSuperBullet)
    {
      if (Utils::Intersection(itrTank->second->GetGlobalBounds(), itrSuperBullet->second->GetGlobalBounds()))
      {
        mAnimationHandler.CreateAnimation(itrSuperBullet->second->GetGlobalBounds(), EImage::SUPERBULLETCOLLISION);
        mAnimationHandler.CreateAnimation(itrTank->second->GetGlobalBounds(), EImage::TANKCOLLISION);
        entities.erase(itrSuperBullet);
        entities.erase(itrTank);
        initializeObjects();
        return true;
      }
    }
  }

  // SuperBullet vs PlayerTank
  for (auto itrSuperBullet = retSuperBullet.first; itrSuperBullet != retSuperBullet.second; ++itrSuperBullet)
  {
    if (Utils::Intersection(mPlayerTank->GetGlobalBounds(), itrSuperBullet->second->GetGlobalBounds()))
    {
      mAnimationHandler.CreateAnimation(itrSuperBullet->second->GetGlobalBounds(), EImage::SUPERBULLETCOLLISION);
      mAnimationHandler.CreateAnimation(mPlayerTank->GetGlobalBounds(), EImage::TANKCOLLISION);
      entities.erase(itrSuperBullet);
      entities.erase(ECategory::PLAYERTANK);
      panel.SetCurrentLives(0);
      gameStage = EGamestates::GAME_OVER;
      initializeObjects();
      return true;
    }
  }

  // SuperBullet vs Eagle
  auto eagle = entities.find(ECategory::EAGLE);
  for (auto itrSuperBullet = retSuperBullet.first; itrSuperBullet != retSuperBullet.second; ++itrSuperBullet)
  {
    if (Utils::Intersection(eagle->second->GetGlobalBounds(), itrSuperBullet->second->GetGlobalBounds()))
    {
      mAnimationHandler.CreateAnimation(itrSuperBullet->second->GetGlobalBounds(), EImage::SUPERBULLETCOLLISION);
      mAnimationHandler.CreateAnimation(eagle->second->GetGlobalBounds(), EImage::EAGLECOLLISION);
      eagle->second->Kill();
      entities.erase(itrSuperBullet);
      gameStage = EGamestates::GAME_OVER;
      initializeObjects();
      return true;
    }
  }

  return false;
}

bool Player::isIntersectsEnemy()
{
  initializeObjects();

  // EnemyTank vs Wall_1
  for (auto itrEnemyTank = retEnemy.first; itrEnemyTank != retEnemy.second; ++itrEnemyTank)
  {
    for (auto itrMap = retWall_1.first; itrMap != retWall_1.second; itrMap++)
    {
      if (Utils::Intersection(itrEnemyTank->second->GetGlobalBounds(), itrMap->second.Rect))
      { // Collision!
        return true;
      }
    }
  }

  // EnemyTank vs Wall_2
  for (auto itrEnemyTank = retEnemy.first; itrEnemyTank != retEnemy.second; ++itrEnemyTank)
  {
    for (auto itrMap = retWall_2.first; itrMap != retWall_2.second; itrMap++)
    {
      if (Utils::Intersection(itrEnemyTank->second->GetGlobalBounds(), itrMap->second.Rect))
      { // Collision!
        return true;
      }
    }
  }

  // EnemyTank vs MainWall
  for (auto itrEnemyTank = retEnemy.first; itrEnemyTank != retEnemy.second; ++itrEnemyTank)
  {
    for (auto itrMap = retMainWall.first; itrMap != retMainWall.second; itrMap++)
    {
      if (Utils::Intersection(itrEnemyTank->second->GetGlobalBounds(), itrMap->second.Rect))
      { // Collision!
        return true;
      }
    }
  }

  // EnemyTank vs WaterWall
  for (auto itrEnemyTank = retEnemy.first; itrEnemyTank != retEnemy.second; ++itrEnemyTank)
  {
    for (auto itrMap = retWaterWall.first; itrMap != retWaterWall.second; itrMap++)
    {
      if (Utils::Intersection(itrEnemyTank->second->GetGlobalBounds(), itrMap->second.Rect))
      {
        // std::cout << "Collision!!!" << std::endl;
        return true;
      }
    }
  }

  // EnemyTank vs Eagle
  auto eagle = entities.find(ECategory::EAGLE);
  for (auto itrEnemyTank = retEnemy.first; itrEnemyTank != retEnemy.second; ++itrEnemyTank)
  {
    if (Utils::Intersection(eagle->second->GetGlobalBounds(), itrEnemyTank->second->GetGlobalBounds()))
    {
      mAnimationHandler.CreateAnimation(eagle->second->GetGlobalBounds(), EImage::EAGLECOLLISION);
      eagle->second->Kill();
      gameStage = EGamestates::GAME_OVER;
      return true;
    }
  }

  return false;
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

void Player::isIntersectsOthers()
{
  initializeObjects();
  // EnemyTank vs PlayerTank
  for (auto itrEnemyTank = retEnemy.first; itrEnemyTank != retEnemy.second; ++itrEnemyTank)
  {
    if (Utils::Intersection(mPlayerTank->GetGlobalBounds(), itrEnemyTank->second->GetGlobalBounds()))
    {
      mAnimationHandler.CreateAnimation(mPlayerTank->GetGlobalBounds(), EImage::TANKCOLLISION);
      mAnimationHandler.CreateAnimation(itrEnemyTank->second->GetGlobalBounds(), EImage::TANKCOLLISION);
      entities.erase(itrEnemyTank);
      entities.erase(ECategory::PLAYERTANK);
      panel.SetCurrentLives(0);
      gameStage = EGamestates::GAME_OVER;
      initializeObjects();
      break;
    }
  }
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
