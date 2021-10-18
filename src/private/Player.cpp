#include "../Player.hpp"
#include "../Eagle.hpp"

Player::Player(std::multimap<ECategory, std::shared_ptr<Entity>>& ent,
  std::multimap<EImage, std::shared_ptr<Animation>>& a,
  EGamestates& g,
  std::vector<std::shared_ptr<Entity>>& et,
  std::vector<std::shared_ptr<Map>>& map,
  RightPanel& pan,
  std::multimap<EImage, std::shared_ptr<BaseBonus>>& bon)
  : entities(ent)
  , animations(a)
  , gameStage(g)
  , enemyTanks(et)
  , spawnEnemyTanksTime(2)
  , mapSequence(map)
  , panel(pan)
  , enemyTanksOnFieldNumber(4)
  , bonuses(bon)
{
}

void Player::initialPlayer()
{
  playerTank = std::make_shared<PlayerTank>();
  playerTank->Init();
  entities.insert({ECategory::PLAYERTANK, playerTank});
}

void Player::handleEvent(const sf::Event& event, sf::Time)
{
  if (event.type == sf::Event::KeyPressed)
  {
    auto found = mKeyBinding.find(event.key.code);
    if (found != mKeyBinding.end() && !isRealtimeAction(found->second))
    {
      auto itrPlayer = entities.find(ECategory::PLAYERTANK);
      if (itrPlayer != entities.end())
      {
        if (itrPlayer->second->CanIDoFire())
        {
          mFireBinding[found->second].get()->bulletAction(itrPlayer->second);
          panel.SetCurrentMissles(itrPlayer->second->GetSuperClipSize());
        }
      }
    }
  }
}

void Player::handleRealtimeInput(sf::Time TimePerFrame)
{
  auto itrPlayer = entities.find(ECategory::PLAYERTANK);
  if (itrPlayer != entities.end())
  {
    for (auto pair : mKeyBinding)
    {
      if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
      {
        mActionBinding[pair.second].get()->tankAction(TimePerFrame, itrPlayer->second, false);
        if (isIntersectsPlayerTank())
        {
          mActionBinding[pair.second].get()->tankAction(TimePerFrame, itrPlayer->second, true);
        }
        itrPlayer->second->SetIsMoving(true);
        break;
      }
      itrPlayer->second->SetIsMoving(false);
    }
  }
}

void Player::handleBonusEvents(sf::Time)
{
  static sf::Clock clock;
  static sf::Vector2f spawnPos;
  static float bonusSpawnTime = 20.f; // for initial start
  sf::Time time = clock.getElapsedTime();

  if (time.asSeconds() > bonusSpawnTime)
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

    bonusSpawnTime = static_cast<float>(rand() % 30 + 20);
    clock.restart();
  }

  isIntersectsBonus();
}

void Player::handleAnimation(sf::FloatRect rect, EImage tex)
{
  Animation* anim = nullptr;
  if (tex == +EImage::BULLETCOLLISION)
  {
    anim = new BulletCollision();
  }
  else if (tex == +EImage::TANKCOLLISION)
  {
    anim = new TankCollision();
  }
  else if (tex == +EImage::SUPERBULLETCOLLISION)
  {
    anim = new SuperBulletCollision();
  }
  else if (tex == +EImage::EAGLECOLLISION)
  {
    anim = new EagleCollision();
  }

  anim->Init();
  anim->Bang(rect);
  animations.insert(std::make_pair(tex, anim));
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
      mFireBinding[EActions::SUPERFIRE].get()->bulletAction(entity);
    }
    else
    {
      mFireBinding[EActions::FIRE].get()->bulletAction(entity);
    }
  }
}

void Player::BulletControl::bulletAction(std::shared_ptr<Entity> entity)
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
  float playerStepMove = 100.f;

  mActionBinding[EActions::LEFT] = std::make_unique<TankControl>(-playerStepMove, 0.f, EActions::LEFT);
  mActionBinding[EActions::RIGHT] = std::make_unique<TankControl>(+playerStepMove, 0.f, EActions::RIGHT);
  mActionBinding[EActions::UP] = std::make_unique<TankControl>(0.f, -playerStepMove, EActions::UP);
  mActionBinding[EActions::DOWN] = std::make_unique<TankControl>(0.f, +playerStepMove, EActions::DOWN);
  mFireBinding[EActions::FIRE] = std::make_unique<BulletControl>(entities, ECategory::BULLET);
  mFireBinding[EActions::SUPERFIRE] = std::make_unique<BulletControl>(entities, ECategory::SUPERBULLET);
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

bool Player::isRealtimeAction(EActions action)
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

void Player::TankControl::tankAction(sf::Time time, std::shared_ptr<Entity> entity, bool back)
{
  if (!back)
  {
    entity->SetVelocity(velocity * entity->GetSpeed());
    entity->Update((velocity * entity->GetSpeed()) * time.asSeconds());
    entity->Rotate(side);
  }
  else
  {
    entity->UpdateBack((velocity * entity->GetSpeed()) * time.asSeconds());
  }
}

bool Player::myIntersection(sf::FloatRect obj1, sf::FloatRect obj2)
{
  if (obj1.left + obj1.width >= obj2.left && obj1.top + obj1.height >= obj2.top && obj1.left <= obj2.left + obj2.width
      && obj1.top <= obj2.top + obj2.width)
    return true;
  else
    return false;
}

bool Player::isIntersectsBullet()
{
  initializeObjects();
  // Bullet vs Wall_1
  for (auto itrBullet = retBullet.first; itrBullet != retBullet.second; ++itrBullet)
  {
    for (auto itrMap = retWall_1.first; itrMap != retWall_1.second; itrMap++)
    {
      if (myIntersection(itrBullet->second->GetGlobalBounds(), itrMap->second.Rect))
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
      if (myIntersection(itrBullet->second->GetGlobalBounds(), itrMap->second.Rect))
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
      if (myIntersection(itrBullet->second->GetGlobalBounds(), itrMap->second.Rect))
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
      if (myIntersection(itrTank->second->GetGlobalBounds(), itrBullet->second->GetGlobalBounds()))
      {
        handleAnimation(itrBullet->second->GetGlobalBounds(), EImage::BULLETCOLLISION);
        entities.erase(itrBullet);
        itrTank->second->Kill();
        initializeObjects();
        // std::cout << playerTank->second->getHP() << std::endl;
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
  auto lplayerTank = entities.find(ECategory::PLAYERTANK);
  if (lplayerTank != entities.end())
  {
    for (auto itrBullet = retBullet.first; itrBullet != retBullet.second; ++itrBullet)
    {
      if (myIntersection(lplayerTank->second->GetGlobalBounds(), itrBullet->second->GetGlobalBounds()))
      {
        handleAnimation(itrBullet->second->GetGlobalBounds(), EImage::BULLETCOLLISION);
        entities.erase(itrBullet);
        lplayerTank->second->Kill();
        initializeObjects();
        // std::cout << lplayerTank->second->getHP() << std::endl;
        if (!lplayerTank->second->IsAlife())
        {
          handleAnimation(lplayerTank->second->GetGlobalBounds(), EImage::TANKCOLLISION);
          entities.erase(lplayerTank);
          gameStage = EGamestates::GAME_OVER;
          initializeObjects();
        }
        else
        {
          auto lives = lplayerTank->second->GetHP();
          if (lives < 0)
          {
            lives = 0;
          }

          panel.SetCurrentLives(static_cast<std::size_t>(lives));
        }
        // std::cout << "TankCollision!!!" << std::endl;

        return true;
      }
    }
  }

  // Bullet vs Eagle
  auto eagle = entities.find(ECategory::EAGLE);
  for (auto itrBullet = retBullet.first; itrBullet != retBullet.second; ++itrBullet)
  {
    if (myIntersection(eagle->second->GetGlobalBounds(), itrBullet->second->GetGlobalBounds()))
    {
      handleAnimation(itrBullet->second->GetGlobalBounds(), EImage::BULLETCOLLISION);
      handleAnimation(eagle->second->GetGlobalBounds(), EImage::EAGLECOLLISION);
      eagle->second->Kill();
      entities.erase(itrBullet);
      gameStage = EGamestates::GAME_OVER;
      // std::cout << "TankCollision!!!" << std::endl;
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
      if (myIntersection(itrSuperBullet->second->GetGlobalBounds(), itrMap->second.Rect))
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
      if (myIntersection(itrSuperBullet->second->GetGlobalBounds(), itrMap->second.Rect))
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
      if (myIntersection(itrSuperBullet->second->GetGlobalBounds(), itrMap->second.Rect))
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
      if (myIntersection(itrTank->second->GetGlobalBounds(), itrSuperBullet->second->GetGlobalBounds()))
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
  auto lplayerTank = entities.find(ECategory::PLAYERTANK);
  if (lplayerTank != entities.end())
  {
    for (auto itrSuperBullet = retSuperBullet.first; itrSuperBullet != retSuperBullet.second; ++itrSuperBullet)
    {
      if (myIntersection(lplayerTank->second->GetGlobalBounds(), itrSuperBullet->second->GetGlobalBounds()))
      {
        handleAnimation(itrSuperBullet->second->GetGlobalBounds(), EImage::SUPERBULLETCOLLISION);
        handleAnimation(lplayerTank->second->GetGlobalBounds(), EImage::TANKCOLLISION);
        entities.erase(itrSuperBullet);
        entities.erase(lplayerTank);
        panel.SetCurrentLives(0);
        gameStage = EGamestates::GAME_OVER;
        initializeObjects();
        // std::cout << lplayerTank->second->getHP() << std::endl;
        return true;
      }
    }
  }

  // SuperBullet vs Eagle
  auto eagle = entities.find(ECategory::EAGLE);
  for (auto itrSuperBullet = retSuperBullet.first; itrSuperBullet != retSuperBullet.second; ++itrSuperBullet)
  {
    if (myIntersection(eagle->second->GetGlobalBounds(), itrSuperBullet->second->GetGlobalBounds()))
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
      if (myIntersection(itrEnemyTank->second->GetGlobalBounds(), itrMap->second.Rect))
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
      if (myIntersection(itrEnemyTank->second->GetGlobalBounds(), itrMap->second.Rect))
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
      if (myIntersection(itrEnemyTank->second->GetGlobalBounds(), itrMap->second.Rect))
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
      if (myIntersection(itrEnemyTank->second->GetGlobalBounds(), itrMap->second.Rect))
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
    if (myIntersection(eagle->second->GetGlobalBounds(), itrEnemyTank->second->GetGlobalBounds()))
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
  auto retPlayerTank = entities.find(ECategory::PLAYERTANK);

  // PlayerTank vs StarBonus
  for (auto itrBonus = bonuses.begin(); itrBonus != bonuses.end(); itrBonus++)
  {
    if (myIntersection(retPlayerTank->second->GetGlobalBounds(), itrBonus->second->GetGlobalBounds()))
    {
      auto bonus = itrBonus->second;
      if (bonus->GetType() == +EImage::BONUSSTAR)
      {
        retPlayerTank->second->SetBulletSpeed(retPlayerTank->second->GetBulletSpeed() + static_cast<float>(bonus->GetPackSize()));
        bonuses.erase(itrBonus);
      }
      else if (bonus->GetType() == +EImage::BONUSMISSLE)
      {
        retPlayerTank->second->SuperClipLoad(bonus->GetPackSize());
        bonuses.erase(itrBonus);
        panel.SetCurrentMissles(retPlayerTank->second->GetSuperClipSize());
      }
      else if (bonus->GetType() == +EImage::BONUSLIFE)
      {
        retPlayerTank->second->SetHP(retPlayerTank->second->GetHP() + static_cast<int>(bonus->GetPackSize()));
        bonuses.erase(itrBonus);
        auto lives = retPlayerTank->second->GetHP();
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
  auto retPlayerTank = entities.find(ECategory::PLAYERTANK);
  initializeObjects();

  for (auto itrMap = retWall_1.first; itrMap != retWall_1.second; itrMap++)
  {
    if (myIntersection(retPlayerTank->second->GetGlobalBounds(), itrMap->second.Rect))
    {
      // std::cout << "Collision!!!" << std::endl;
      return true;
    }
  }

  // PlayerTank vs Wall_2
  for (auto itrMap = retWall_2.first; itrMap != retWall_2.second; itrMap++)
  {
    if (myIntersection(retPlayerTank->second->GetGlobalBounds(), itrMap->second.Rect))
    {
      // std::cout << "Collision!!!" << std::endl;
      return true;
    }
  }

  // PlayerTank vs MainWall
  for (auto itrMap = retMainWall.first; itrMap != retMainWall.second; itrMap++)
  {
    if (myIntersection(retPlayerTank->second->GetGlobalBounds(), itrMap->second.Rect))
    {
      // std::cout << "Collision!!!" << std::endl;
      return true;
    }
  }

  // PlayerTank vs WaterWall
  for (auto itrMap = retWaterWall.first; itrMap != retWaterWall.second; itrMap++)
  {
    if (myIntersection(retPlayerTank->second->GetGlobalBounds(), itrMap->second.Rect))
    {
      // std::cout << "Collision!!!" << std::endl;
      return true;
    }
  }

  return false;
}

void Player::isIntersectsOthers()
{
  initializeObjects();
  // EnemyTank vs PlayerTank
  auto retPlayerTank = entities.find(ECategory::PLAYERTANK);
  if (retPlayerTank != entities.end())
  {
    for (auto itrEnemyTank = retEnemy.first; itrEnemyTank != retEnemy.second; ++itrEnemyTank)
    {
      if (myIntersection(retPlayerTank->second->GetGlobalBounds(), itrEnemyTank->second->GetGlobalBounds()))
      {
        handleAnimation(itrEnemyTank->second->GetGlobalBounds(), EImage::TANKCOLLISION);
        handleAnimation(retPlayerTank->second->GetGlobalBounds(), EImage::TANKCOLLISION);
        entities.erase(itrEnemyTank);
        entities.erase(retPlayerTank);
        panel.SetCurrentLives(0);
        gameStage = EGamestates::GAME_OVER;
        initializeObjects();
        break;
      }
    }
  }
}

bool Player::Init()
{
  mKeyBinding[sf::Keyboard::Left] = EActions::LEFT;
  mKeyBinding[sf::Keyboard::Right] = EActions::RIGHT;
  mKeyBinding[sf::Keyboard::Up] = EActions::UP;
  mKeyBinding[sf::Keyboard::Down] = EActions::DOWN;
  mKeyBinding[sf::Keyboard::Space] = EActions::FIRE;
  mKeyBinding[sf::Keyboard::LControl] = EActions::SUPERFIRE;
  mKeyBinding[sf::Keyboard::P] = EActions::PAUSE;

  auto eagle = std::make_shared<Eagle>();
  if(!eagle->Init())
  {
    SPDLOG_ERROR("Something wrong with Eagle");
    return false;
  }
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

  initialPlayer();
  initializeActions();
  initializeObjects();

  return true;
}
