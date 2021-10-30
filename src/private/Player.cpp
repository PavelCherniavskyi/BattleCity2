#include "../Player.hpp"
#include "../Eagle.hpp"
#include "../Utils/Utils.hpp"

constexpr auto kPlayerStepMove = 100.f;
constexpr auto kBonusSpawnTime = 20.f;

Player::Player(std::unordered_multimap<ECategory, std::shared_ptr<Entity>>& ent,
  std::unordered_multimap<EImage, std::shared_ptr<Animation>>& a,
  EGamestates& g,
  std::vector<std::shared_ptr<Entity>>& et,
  std::vector<std::shared_ptr<Map>>& map,
  RightPanel& pan,
  std::unordered_multimap<EImage, std::shared_ptr<BaseBonus>>& bon)
  : entities(ent)
  , animations(a)
  , gameStage(g)
  , enemyTanks(et)
  , spawnEnemyTanksTime(2)
  , mapSequence(map)
  , panel(pan)
  , enemyTanksOnFieldNumber(4)
  , bonuses(bon)
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

void Player::handleBonusEvents(sf::Time)
{
  static sf::Clock clock;
  static sf::Vector2f spawnPos;
  sf::Time time = clock.getElapsedTime();

  if (time.asSeconds() > kBonusSpawnTime)
  {
    int bonusSwitch;
    std::shared_ptr<BaseBonus> bonus = nullptr;
    float x = static_cast<float>(rand() % (kWidthScreen - 75) + 25);
    float y = static_cast<float>(rand() % (kHeightScreen - 75) + 25);
    bonusSwitch = rand() % 3 + 1;

    switch (bonusSwitch)
    {
    case 1:
      bonus = std::make_shared<BonusStar>();
      bonus->Init();
      bonus->SetPosition(x, y);
      bonuses.insert({EImage::BONUSSTAR, bonus});
      break;
    case 2:
      bonus = std::make_shared<BonusMissle>();
      bonus->Init();
      bonus->SetPosition(x, y);
      bonuses.insert({EImage::BONUSMISSLE, bonus});
      break;
    case 3:
      bonus = std::make_shared<BonusLife>();
      bonus->SetPosition(x, y);
      bonus->Init();
      bonuses.insert({EImage::BONUSLIFE, bonus});
      break;
    default:
      break;
    }
    clock.restart();
  }

  isIntersectsBonus();
}

void Player::handleAnimation(sf::FloatRect rect, EImage tex)
{
  std::shared_ptr<Animation> anim = nullptr;
  if (tex == +EImage::BULLETCOLLISION)
  {
    anim = std::make_shared<BulletCollision>();
  }
  else if (tex == +EImage::TANKCOLLISION)
  {
    anim = std::make_shared<TankCollision>();
  }
  else if (tex == +EImage::SUPERBULLETCOLLISION)
  {
    anim = std::make_shared<SuperBulletCollision>();
  }
  else if (tex == +EImage::EAGLECOLLISION)
  {
    anim = std::make_shared<EagleCollision>();
  }

  anim->Init();
  anim->Bang(rect);
  animations.insert({tex, std::move(anim)});
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
    if (!enemyTanks.empty())
    {
      if (entities.count(ECategory::ENEMYTANK) < enemyTanksOnFieldNumber)
      {
        if (handleEnemyApperanceEffect())
        {
          entities.insert({ECategory::ENEMYTANK, enemyTanks.back()});
          panel.PopIcon();
          enemyTanks.pop_back();
          clock.restart();
          spawn = spawnEnemyTanksTime;
        }
      }
    }
  }
}

bool Player::handleEnemyApperanceEffect()
{
  static bool doOnce = true;
  static Apperance* anim = nullptr;
  if (doOnce)
  {
    // std::cout << "handleEnemyApperanceEffect1" << std::endl;
    sf::FloatRect rect = enemyTanks.back()->GetGlobalBounds();
    anim = new Apperance;
    anim->Init();
    anim->Bang(rect);
    animations.insert(std::make_pair(EImage::APPERANCE, anim));
    doOnce = false;
  }
  else
  {
    // std::cout << "handleEnemyApperanceEffect2" << std::endl;
    if (!anim->IsAlife())
    {
      doOnce = true;
      return true;
    }

    // std::cout << "handleEnemyApperanceEffect3" << std::endl;
  }

  return false;
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
        handleAnimation(itrBullet->second->GetGlobalBounds(), EImage::BULLETCOLLISION);
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
        handleAnimation(itrBullet->second->GetGlobalBounds(), EImage::BULLETCOLLISION);
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
        handleAnimation(itrBullet->second->GetGlobalBounds(), EImage::BULLETCOLLISION);
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
        handleAnimation(itrBullet->second->GetGlobalBounds(), EImage::BULLETCOLLISION);
        entities.erase(itrBullet);
        itrTank->second->Kill();
        initializeObjects();
        if (!itrTank->second->IsAlife())
        {
          handleAnimation(itrTank->second->GetGlobalBounds(), EImage::TANKCOLLISION);
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
      handleAnimation(itrBullet->second->GetGlobalBounds(), EImage::BULLETCOLLISION);
      entities.erase(itrBullet);
      mPlayerTank->Kill();
      initializeObjects();
      // std::cout << lplayerTank->second->getHP() << std::endl;
      if (!mPlayerTank->IsAlife())
      {
        handleAnimation(mPlayerTank->GetGlobalBounds(), EImage::TANKCOLLISION);
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
      handleAnimation(itrBullet->second->GetGlobalBounds(), EImage::BULLETCOLLISION);
      handleAnimation(eagle->second->GetGlobalBounds(), EImage::EAGLECOLLISION);
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
        handleAnimation(itrSuperBullet->second->GetGlobalBounds(), EImage::SUPERBULLETCOLLISION);
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
        handleAnimation(itrSuperBullet->second->GetGlobalBounds(), EImage::SUPERBULLETCOLLISION);
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
        handleAnimation(itrSuperBullet->second->GetGlobalBounds(), EImage::SUPERBULLETCOLLISION);
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
        handleAnimation(itrSuperBullet->second->GetGlobalBounds(), EImage::SUPERBULLETCOLLISION);
        handleAnimation(itrTank->second->GetGlobalBounds(), EImage::TANKCOLLISION);
        entities.erase(itrSuperBullet);
        entities.erase(itrTank);
        // std::cout << "TankCollision!!!" << std::endl;
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
      handleAnimation(itrSuperBullet->second->GetGlobalBounds(), EImage::SUPERBULLETCOLLISION);
      handleAnimation(mPlayerTank->GetGlobalBounds(), EImage::TANKCOLLISION);
      entities.erase(itrSuperBullet);
      entities.erase(ECategory::PLAYERTANK);
      panel.SetCurrentLives(0);
      gameStage = EGamestates::GAME_OVER;
      initializeObjects();
      // std::cout << lplayerTank->second->getHP() << std::endl;
      return true;
    }
  }

  // SuperBullet vs Eagle
  auto eagle = entities.find(ECategory::EAGLE);
  for (auto itrSuperBullet = retSuperBullet.first; itrSuperBullet != retSuperBullet.second; ++itrSuperBullet)
  {
    if (Utils::Intersection(eagle->second->GetGlobalBounds(), itrSuperBullet->second->GetGlobalBounds()))
    {
      handleAnimation(itrSuperBullet->second->GetGlobalBounds(), EImage::SUPERBULLETCOLLISION);
      handleAnimation(eagle->second->GetGlobalBounds(), EImage::EAGLECOLLISION);
      eagle->second->Kill();
      entities.erase(itrSuperBullet);
      gameStage = EGamestates::GAME_OVER;
      // std::cout << "TankCollision!!!" << std::endl;
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
      handleAnimation(eagle->second->GetGlobalBounds(), EImage::EAGLECOLLISION);
      eagle->second->Kill();
      gameStage = EGamestates::GAME_OVER;
      return true;
    }
  }

  return false;
}

bool Player::isIntersectsBonus()
{
  initializeObjects();

  // PlayerTank vs StarBonus
  for (auto itrBonus = bonuses.begin(); itrBonus != bonuses.end(); itrBonus++)
  {
    if (Utils::Intersection(mPlayerTank->GetGlobalBounds(), itrBonus->second->GetGlobalBounds()))
    {
      auto bonus = itrBonus->second;
      if (bonus->GetType() == +EImage::BONUSSTAR)
      {
        mPlayerTank->SetBulletSpeed(mPlayerTank->GetBulletSpeed() + static_cast<float>(bonus->GetPackSize()));
        bonuses.erase(itrBonus);
      }
      else if (bonus->GetType() == +EImage::BONUSMISSLE)
      {
        mPlayerTank->SuperClipLoad(bonus->GetPackSize());
        bonuses.erase(itrBonus);
        panel.SetCurrentMissles(mPlayerTank->GetSuperClipSize());
      }
      else if (bonus->GetType() == +EImage::BONUSLIFE)
      {
        mPlayerTank->SetHP(mPlayerTank->GetHP() + static_cast<int>(bonus->GetPackSize()));
        bonuses.erase(itrBonus);
        auto lives = mPlayerTank->GetHP();
        if (lives < 0)
        {
          lives = 0;
        }

        panel.SetCurrentLives(static_cast<std::size_t>(lives));
      }

      // panel.setCurrentMissles(tank->GetSuperClipSize());
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
      handleAnimation(itrEnemyTank->second->GetGlobalBounds(), EImage::TANKCOLLISION);
      handleAnimation(mPlayerTank->GetGlobalBounds(), EImage::TANKCOLLISION);
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
