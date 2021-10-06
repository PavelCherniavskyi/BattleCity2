#include "../Player.h"

Player::Player(std::multimap<Category::Type, Entity*>& ent,
               std::multimap<Textures::ID, Animation*>& a, Gamestates& g,
               std::queue<Entity*>& et, std::queue<Map*>& map, RightPanel& pan,
               std::multimap<Textures::ID, BaseBonus*>& bon)
    : entities(ent),
      animations(a),
      gameStage(g),
      enemyTanks(et),
      spawnEnemyTanksTime(2),
      mapSequence(map),
      panel(pan),
      enemyTanksOnFieldNumber(4),
      bonuses(bon) {}

void Player::initialPlayer() {
  playerTank = new Tank(Category::PlayerTank, Textures::T_10);
  entities.insert(std::make_pair(Category::PlayerTank, playerTank));
}

void Player::handleEvent(const sf::Event& event, sf::Time) {
  if (event.type == sf::Event::KeyPressed) {
    auto found = mKeyBinding.find(event.key.code);
    if (found != mKeyBinding.end() && !isRealtimeAction(found->second)) {
      auto itrPlayer = entities.find(Category::PlayerTank);
      if (itrPlayer != entities.end()) {
        Tank* tank = dynamic_cast<Tank*>(itrPlayer->second);
        if (tank->canIdoFire()) {
          mFireBinding[found->second].get()->bulletAction(tank);
          panel.setCurrentMissles(tank->superClipSize());
        }

        // std::cout << "mActionBinding_FIRE - DONE" << std::endl;
      }
    }
  }
}

void Player::handleRealtimeInput(sf::Time TimePerFrame) {
  auto itrPlayer = entities.find(Category::PlayerTank);
  if (itrPlayer != entities.end()) {
    for (auto pair : mKeyBinding) {
      if (sf::Keyboard::isKeyPressed(pair.first) &&
          isRealtimeAction(pair.second)) {
        mActionBinding[pair.second].get()->tankAction(TimePerFrame,
                                                      itrPlayer->second, false);
        if (isIntersectsPlayerTank()) {
          mActionBinding[pair.second].get()->tankAction(
              TimePerFrame, itrPlayer->second, true);
        }
        itrPlayer->second->setIsMoving(true);
        break;
      }
      itrPlayer->second->setIsMoving(false);
    }
  }
}

void Player::handleBonusEvents(sf::Time) {
  static sf::Clock clock;
  static sf::Vector2f spawnPos;
  static float bonusSpawnTime = 20.f;  // for initial start
  int bonusSwitch;
  sf::Time time = clock.getElapsedTime();
  BaseBonus* bonus = nullptr;

  if (time.asSeconds() > bonusSpawnTime) {
    spawnPos.x = static_cast<float>(rand() % (kWidthScreen - 75) + 25);
    spawnPos.y = static_cast<float>(rand() % (kHeightScreen - 75) + 25);
    bonusSwitch = rand() % 3 + 1;

    switch (bonusSwitch) {
      case 1:
        bonus = new BonusStar(Textures::BonusStar);
        bonus->setPosition(spawnPos.x, spawnPos.y);
        bonuses.insert(std::make_pair(Textures::BonusStar, bonus));
        break;
      case 2:
        bonus = new BonusMissle(Textures::BonusMissle);
        bonus->setPosition(spawnPos.x, spawnPos.y);
        bonuses.insert(std::make_pair(Textures::BonusMissle, bonus));
        break;
      case 3:
        bonus = new BonusLife(Textures::BonusLife);
        bonus->setPosition(spawnPos.x, spawnPos.y);
        bonuses.insert(std::make_pair(Textures::BonusLife, bonus));
        break;
      default:
        break;
    }

    bonusSpawnTime = static_cast<float>(rand() % 30 + 20);
    clock.restart();

    // std::cout << spawnPos.x << ' ' << spawnPos.y << std::endl;
  }
  isIntersectsBonus();
}

void Player::handleAnimation(sf::FloatRect rect, Textures::ID tex) {
  Animation* anim = nullptr;
  if (tex == Textures::BulletCollision) {
    // std::cout << "Textures::BulletCollision" << std::endl;
    anim = new BulletCollision();
  } else if (tex == Textures::TankCollision) {
    // std::cout << "Textures::TankCollision" << std::endl;
    anim = new TankCollision();
  } else if (tex == Textures::SuperBulletCollision) {
    // std::cout << "Textures::SuperBulletCollision" << std::endl;
    anim = new SuperBulletCollision();
  } else if (tex == Textures::EagleCollision) {
    // std::cout << "Textures::EagleCollision" << std::endl;
    anim = new EagleCollision();
  }

  anim->bang(rect);
  animations.insert(std::make_pair(tex, anim));
}

void Player::handleEnemyTanks(Entity* entity) {
  int choice(std::rand() % 4);
  if (choice == 0) {
    entity->setVelocity(0.f, +100.f * entity->getSpeed());
    entity->rotate(Entity::Down);
  } else if (choice == 1) {
    entity->setVelocity(0.f, -100.f * entity->getSpeed());
    entity->rotate(Entity::Up);
  } else if (choice == 2) {
    entity->setVelocity(+100 * entity->getSpeed(), 0.f);
    entity->rotate(Entity::Right);
  } else if (choice == 3) {
    entity->setVelocity(-100 * entity->getSpeed(), 0.f);
    entity->rotate(Entity::Left);
  }
}

void Player::handleEnemySpawn(sf::Time) {
  static sf::Clock clock;
  static float spawn = 0.f;  // for initial start
  sf::Time time = clock.getElapsedTime();

  if (time.asSeconds() > spawn) {
    if (!enemyTanks.empty()) {
      if (entities.count(Category::EnemyTank) < enemyTanksOnFieldNumber) {
        if (handleEnemyApperanceEffect()) {
          entities.insert(
              std::make_pair(Category::EnemyTank, enemyTanks.front()));
          panel.popIcon();
          enemyTanks.pop();
          clock.restart();
          spawn = spawnEnemyTanksTime;
        }
      }
    }
  }
}

bool Player::handleEnemyApperanceEffect() {
  static bool doOnce = true;
  static Apperance* anim = nullptr;
  if (doOnce) {
    // std::cout << "handleEnemyApperanceEffect1" << std::endl;
    sf::FloatRect rect = enemyTanks.front()->getGlobalBounds();
    anim = new Apperance;
    anim->bang(rect);
    animations.insert(std::make_pair(Textures::Apperance, anim));
    doOnce = false;
  } else {
    // std::cout << "handleEnemyApperanceEffect2" << std::endl;
    if (!anim->isAlife()) {
      doOnce = true;
      return true;
    }

    // std::cout << "handleEnemyApperanceEffect3" << std::endl;
  }

  return false;
}

void Player::handleEnemyFire(sf::Time, Entity* entity) {
  Tank* tank = dynamic_cast<Tank*>(entity);
  if (tank->canEnemyDoFire()) {
    if (tank->getType() == Textures::Enemy_40) {
      mFireBinding[Player::SuperFire].get()->bulletAction(tank);
      // std::cout << "SuperFire" << std::endl;
    } else {
      mFireBinding[Player::Fire].get()->bulletAction(tank);
    }
  }
}

void Player::BulletControl::bulletAction(Entity* entity) {
  Tank* tank = dynamic_cast<Tank*>(entity);
  if (type == Category::Bullet) {
    entities.insert(std::make_pair(
        Category::Bullet, tank->doFire(Category::Bullet, Textures::None)));
  } else if (type == Category::SuperBullet) {
    // std::cout << "bulletAction1" << std::endl;
    if (tank->getType() == Textures::T_10) {
      if (tank->superClipSize() != 0) {
        entities.insert(std::make_pair(
            Category::SuperBullet,
            tank->doFire(Category::SuperBullet, Textures::T_10)));
      }
    } else if (tank->getType() == Textures::Enemy_40) {
      // std::cout << "bulletAction2" << std::endl;
      entities.insert(std::make_pair(
          Category::SuperBullet,
          tank->doFire(Category::SuperBullet, Textures::Enemy_40)));
    }
  }
}

void Player::initializeActions() {
  float playerStepMove = 100.f;

  mActionBinding[MoveLeft] = std::unique_ptr<TankControl>(
      new TankControl(-playerStepMove, 0.f, Entity::Left));
  mActionBinding[MoveRight] = std::unique_ptr<TankControl>(
      new TankControl(+playerStepMove, 0.f, Entity::Right));
  mActionBinding[MoveUp] = std::unique_ptr<TankControl>(
      new TankControl(0.f, -playerStepMove, Entity::Up));
  mActionBinding[MoveDown] = std::unique_ptr<TankControl>(
      new TankControl(0.f, +playerStepMove, Entity::Down));
  mFireBinding[Fire] = std::unique_ptr<BulletControl>(
      new BulletControl(entities, Category::Bullet));
  mFireBinding[SuperFire] = std::unique_ptr<BulletControl>(
      new BulletControl(entities, Category::SuperBullet));
}

void Player::initializeObjects() {
  retBullet = entities.equal_range(Category::Bullet);
  retSuperBullet = entities.equal_range(Category::SuperBullet);
  retEnemy = entities.equal_range(Category::EnemyTank);
  retEagle = entities.equal_range(Category::Eagle);
  retWall_1 = mapSequence.front()->getMap().equal_range(Textures::Wall_1);
  retWall_2 = mapSequence.front()->getMap().equal_range(Textures::Wall_2);
  retMainWall = mapSequence.front()->getMap().equal_range(Textures::MainWall);
  retWaterWall = mapSequence.front()->getMap().equal_range(Textures::WaterWall);
}

bool Player::isRealtimeAction(Action action) {
  switch (action) {
    case MoveLeft:
    case MoveRight:
    case MoveDown:
    case MoveUp:
      return true;

    default:
      return false;
  }
}

void Player::TankControl::tankAction(sf::Time time, Entity* entity, bool back) {
  // std::cout << velocity.x << ' ' << velocity.y << std::endl;
  if (!back) {
    entity->setVelocity(velocity * entity->getSpeed());
    entity->update((velocity * entity->getSpeed()) * time.asSeconds());
    entity->rotate(side);
  } else {
    // std::cout << "Collision!!!" << std::endl;
    entity->updateBack((velocity * entity->getSpeed()) * time.asSeconds());
  }
}

bool Player::myIntersection(sf::FloatRect obj1, sf::FloatRect obj2) {
  if (obj1.left + obj1.width >= obj2.left &&
      obj1.top + obj1.height >= obj2.top &&
      obj1.left <= obj2.left + obj2.width && obj1.top <= obj2.top + obj2.width)
    return true;
  else
    return false;
}

bool Player::isIntersectsBullet() {
  initializeObjects();
  // Bullet vs Wall_1
  for (auto itrBullet = retBullet.first; itrBullet != retBullet.second;
       ++itrBullet) {
    for (auto itrMap = retWall_1.first; itrMap != retWall_1.second; itrMap++) {
      if (myIntersection(itrBullet->second->getGlobalBounds(),
                         itrMap->second.rect)) {
        handleAnimation(itrBullet->second->getGlobalBounds(),
                        Textures::BulletCollision);
        itrBullet->second->~Entity();
        mapSequence.front()->getMap().erase(itrMap);
        entities.erase(itrBullet);
        initializeObjects();
        return true;
      }
    }
  }
  // Bullet vs Wall_2
  for (auto itrBullet = retBullet.first; itrBullet != retBullet.second;
       ++itrBullet) {
    for (auto itrMap = retWall_2.first; itrMap != retWall_2.second; itrMap++) {
      if (myIntersection(itrBullet->second->getGlobalBounds(),
                         itrMap->second.rect)) {
        handleAnimation(itrBullet->second->getGlobalBounds(),
                        Textures::BulletCollision);
        itrBullet->second->~Entity();
        entities.erase(itrBullet);
        // std::cout << "WallCrashed!!!" << std::endl;
        initializeObjects();
        return true;
      }
    }
  }
  // Bullet vs MainWall
  for (auto itrBullet = retBullet.first; itrBullet != retBullet.second;
       ++itrBullet) {
    for (auto itrMap = retMainWall.first; itrMap != retMainWall.second;
         itrMap++) {
      if (myIntersection(itrBullet->second->getGlobalBounds(),
                         itrMap->second.rect)) {
        handleAnimation(itrBullet->second->getGlobalBounds(),
                        Textures::BulletCollision);
        itrBullet->second->~Entity();
        entities.erase(itrBullet);
        initializeObjects();
        return true;
      }
    }
  }

  // Bullet vs EnemyTank
  for (auto itrTank = retEnemy.first; itrTank != retEnemy.second; ++itrTank) {
    for (auto itrBullet = retBullet.first; itrBullet != retBullet.second;
         ++itrBullet) {
      if (myIntersection(itrTank->second->getGlobalBounds(),
                         itrBullet->second->getGlobalBounds())) {
        handleAnimation(itrBullet->second->getGlobalBounds(),
                        Textures::BulletCollision);
        itrBullet->second->~Entity();
        entities.erase(itrBullet);
        itrTank->second->kill();
        initializeObjects();
        // std::cout << playerTank->second->getHP() << std::endl;
        if (!itrTank->second->isAlife()) {
          handleAnimation(itrTank->second->getGlobalBounds(),
                          Textures::TankCollision);
          itrTank->second->~Entity();
          entities.erase(itrTank);
          initializeObjects();
        }
        return true;
      }
    }
  }

  // Bullet vs PlayerTank
  auto lplayerTank = entities.find(Category::PlayerTank);
  if (lplayerTank != entities.end()) {
    for (auto itrBullet = retBullet.first; itrBullet != retBullet.second;
         ++itrBullet) {
      if (myIntersection(lplayerTank->second->getGlobalBounds(),
                         itrBullet->second->getGlobalBounds())) {
        handleAnimation(itrBullet->second->getGlobalBounds(),
                        Textures::BulletCollision);
        itrBullet->second->~Entity();
        entities.erase(itrBullet);
        lplayerTank->second->kill();
        initializeObjects();
        // std::cout << lplayerTank->second->getHP() << std::endl;
        if (!lplayerTank->second->isAlife()) {
          handleAnimation(lplayerTank->second->getGlobalBounds(),
                          Textures::TankCollision);
          lplayerTank->second->~Entity();
          entities.erase(lplayerTank);
          gameStage = Gamestates::gameOver;
          initializeObjects();
        } else {
          auto lives = lplayerTank->second->getHP();
          if (lives < 0) {
            lives = 0;
          }

          panel.setCurrentLives(static_cast<std::size_t>(lives));
        }
        // std::cout << "TankCollision!!!" << std::endl;

        return true;
      }
    }
  }

  // Bullet vs Eagle
  auto eagle = entities.find(Category::Eagle);
  for (auto itrBullet = retBullet.first; itrBullet != retBullet.second;
       ++itrBullet) {
    if (myIntersection(eagle->second->getGlobalBounds(),
                       itrBullet->second->getGlobalBounds())) {
      handleAnimation(itrBullet->second->getGlobalBounds(),
                      Textures::BulletCollision);
      handleAnimation(eagle->second->getGlobalBounds(),
                      Textures::EagleCollision);
      eagle->second->kill();
      itrBullet->second->~Entity();
      entities.erase(itrBullet);
      gameStage = Gamestates::gameOver;
      // std::cout << "TankCollision!!!" << std::endl;
      initializeObjects();
      return true;
    }
  }

  return false;
}

bool Player::isIntersectsSuperBullet() {
  initializeObjects();
  // SuperBullet vs Wall_1
  for (auto itrSuperBullet = retSuperBullet.first;
       itrSuperBullet != retSuperBullet.second; ++itrSuperBullet) {
    for (auto itrMap = retWall_1.first; itrMap != retWall_1.second; itrMap++) {
      if (myIntersection(itrSuperBullet->second->getGlobalBounds(),
                         itrMap->second.rect)) {
        handleAnimation(itrSuperBullet->second->getGlobalBounds(),
                        Textures::SuperBulletCollision);
        itrSuperBullet->second->~Entity();
        mapSequence.front()->getMap().erase(itrMap);
        entities.erase(itrSuperBullet);
        // std::cout << "WallCrashed!!!" << std::endl;
        initializeObjects();
        return true;
      }
    }
  }
  // SuperBullet vs Wall_2
  for (auto itrSuperBullet = retSuperBullet.first;
       itrSuperBullet != retSuperBullet.second; ++itrSuperBullet) {
    for (auto itrMap = retWall_2.first; itrMap != retWall_2.second; itrMap++) {
      if (myIntersection(itrSuperBullet->second->getGlobalBounds(),
                         itrMap->second.rect)) {
        handleAnimation(itrSuperBullet->second->getGlobalBounds(),
                        Textures::SuperBulletCollision);
        itrSuperBullet->second->~Entity();
        mapSequence.front()->getMap().erase(itrMap);
        entities.erase(itrSuperBullet);
        // std::cout << "WallCrashed!!!" << std::endl;
        initializeObjects();
        return true;
      }
    }
  }
  // SuperBullet vs MainWall
  for (auto itrSuperBullet = retSuperBullet.first;
       itrSuperBullet != retSuperBullet.second; ++itrSuperBullet) {
    for (auto itrMap = retMainWall.first; itrMap != retMainWall.second;
         itrMap++) {
      if (myIntersection(itrSuperBullet->second->getGlobalBounds(),
                         itrMap->second.rect)) {
        handleAnimation(itrSuperBullet->second->getGlobalBounds(),
                        Textures::SuperBulletCollision);
        itrSuperBullet->second->~Entity();
        entities.erase(itrSuperBullet);
        // std::cout << "WallStop!!!" << std::endl;
        initializeObjects();
        return true;
      }
    }
  }

  // SuperBullet vs EnemyTank
  for (auto itrTank = retEnemy.first; itrTank != retEnemy.second; ++itrTank) {
    for (auto itrSuperBullet = retSuperBullet.first;
         itrSuperBullet != retSuperBullet.second; ++itrSuperBullet) {
      if (myIntersection(itrTank->second->getGlobalBounds(),
                         itrSuperBullet->second->getGlobalBounds())) {
        handleAnimation(itrSuperBullet->second->getGlobalBounds(),
                        Textures::SuperBulletCollision);
        handleAnimation(itrTank->second->getGlobalBounds(),
                        Textures::TankCollision);
        itrSuperBullet->second->~Entity();
        itrTank->second->~Entity();
        entities.erase(itrSuperBullet);
        entities.erase(itrTank);
        // std::cout << "TankCollision!!!" << std::endl;
        initializeObjects();
        return true;
      }
    }
  }

  // SuperBullet vs PlayerTank
  auto lplayerTank = entities.find(Category::PlayerTank);
  if (lplayerTank != entities.end()) {
    for (auto itrSuperBullet = retSuperBullet.first;
         itrSuperBullet != retSuperBullet.second; ++itrSuperBullet) {
      if (myIntersection(lplayerTank->second->getGlobalBounds(),
                         itrSuperBullet->second->getGlobalBounds())) {
        handleAnimation(itrSuperBullet->second->getGlobalBounds(),
                        Textures::SuperBulletCollision);
        handleAnimation(lplayerTank->second->getGlobalBounds(),
                        Textures::TankCollision);
        itrSuperBullet->second->~Entity();
        lplayerTank->second->~Entity();
        entities.erase(itrSuperBullet);
        entities.erase(lplayerTank);
        panel.setCurrentLives(0);
        gameStage = Gamestates::gameOver;
        initializeObjects();
        // std::cout << lplayerTank->second->getHP() << std::endl;
        return true;
      }
    }
  }

  // SuperBullet vs Eagle
  auto eagle = entities.find(Category::Eagle);
  for (auto itrSuperBullet = retSuperBullet.first;
       itrSuperBullet != retSuperBullet.second; ++itrSuperBullet) {
    if (myIntersection(eagle->second->getGlobalBounds(),
                       itrSuperBullet->second->getGlobalBounds())) {
      handleAnimation(itrSuperBullet->second->getGlobalBounds(),
                      Textures::SuperBulletCollision);
      handleAnimation(eagle->second->getGlobalBounds(),
                      Textures::EagleCollision);
      eagle->second->kill();
      itrSuperBullet->second->~Entity();
      entities.erase(itrSuperBullet);
      gameStage = Gamestates::gameOver;
      // std::cout << "TankCollision!!!" << std::endl;
      initializeObjects();
      return true;
    }
  }

  return false;
}

bool Player::isIntersectsEnemy() {
  initializeObjects();

  // EnemyTank vs Wall_1
  for (auto itrEnemyTank = retEnemy.first; itrEnemyTank != retEnemy.second;
       ++itrEnemyTank) {
    for (auto itrMap = retWall_1.first; itrMap != retWall_1.second; itrMap++) {
      if (myIntersection(itrEnemyTank->second->getGlobalBounds(),
                         itrMap->second.rect)) { // Collision!
        return true;
      }
    }
  }

  // EnemyTank vs Wall_2
  for (auto itrEnemyTank = retEnemy.first; itrEnemyTank != retEnemy.second;
       ++itrEnemyTank) {
    for (auto itrMap = retWall_2.first; itrMap != retWall_2.second; itrMap++) {
      if (myIntersection(itrEnemyTank->second->getGlobalBounds(),
                         itrMap->second.rect)) { // Collision!
        return true;
      }
    }
  }

  // EnemyTank vs MainWall
  for (auto itrEnemyTank = retEnemy.first; itrEnemyTank != retEnemy.second;
       ++itrEnemyTank) {
    for (auto itrMap = retMainWall.first; itrMap != retMainWall.second;
         itrMap++) {
      if (myIntersection(itrEnemyTank->second->getGlobalBounds(),
                         itrMap->second.rect)) {  // Collision!
        return true;
      }
    }
  }

  // EnemyTank vs WaterWall
  for (auto itrEnemyTank = retEnemy.first; itrEnemyTank != retEnemy.second;
       ++itrEnemyTank) {
    for (auto itrMap = retWaterWall.first; itrMap != retWaterWall.second;
         itrMap++) {
      if (myIntersection(itrEnemyTank->second->getGlobalBounds(),
                         itrMap->second.rect)) {
        // std::cout << "Collision!!!" << std::endl;
        return true;
      }
    }
  }

  // EnemyTank vs Eagle
  auto eagle = entities.find(Category::Eagle);
  for (auto itrEnemyTank = retEnemy.first; itrEnemyTank != retEnemy.second;
       ++itrEnemyTank) {
    if (myIntersection(eagle->second->getGlobalBounds(),
                       itrEnemyTank->second->getGlobalBounds())) {
      handleAnimation(eagle->second->getGlobalBounds(),
                      Textures::EagleCollision);
      eagle->second->kill();
      gameStage = Gamestates::gameOver;
      return true;
    }
  }

  return false;
}

bool Player::isIntersectsBonus() {
  initializeObjects();
  auto retPlayerTank = entities.find(Category::PlayerTank);

  // PlayerTank vs StarBonus
  for (auto itrBonus = bonuses.begin(); itrBonus != bonuses.end(); itrBonus++) {
    if (myIntersection(retPlayerTank->second->getGlobalBounds(),
                       itrBonus->second->getGlobalBounds())) {
      Tank* tank = dynamic_cast<Tank*>(retPlayerTank->second);
      BaseBonus* bonus = itrBonus->second;
      if (bonus->getType() == Textures::BonusStar) {
        tank->setBulletSpeed(tank->getBulletSpeed() +
                             static_cast<float>(bonus->getPack()));
        bonus->~BaseBonus();
        bonuses.erase(itrBonus);
      } else if (bonus->getType() == Textures::BonusMissle) {
        SPDLOG_INFO("Player::isIntersectsBonus getPack size: {}",
                    bonus->getPack());
        tank->superClipLoad(bonus->getPack());
        bonus->~BaseBonus();
        bonuses.erase(itrBonus);
        panel.setCurrentMissles(tank->superClipSize());
      } else if (bonus->getType() == Textures::BonusLife) {
        retPlayerTank->second->setHP(retPlayerTank->second->getHP() +
                                     static_cast<int>(bonus->getPack()));
        bonus->~BaseBonus();
        bonuses.erase(itrBonus);
        auto lives = retPlayerTank->second->getHP();
        if (lives < 0) {
          lives = 0;
        }

        panel.setCurrentLives(static_cast<std::size_t>(lives));
      }

      // panel.setCurrentMissles(tank->superClipSize());
      return true;
    }
  }

  return false;
}

bool Player::isIntersectsPlayerTank() {
  // PlayerTank vs Wall_1
  auto retPlayerTank = entities.find(Category::PlayerTank);
  initializeObjects();

  for (auto itrMap = retWall_1.first; itrMap != retWall_1.second; itrMap++) {
    if (myIntersection(retPlayerTank->second->getGlobalBounds(),
                       itrMap->second.rect)) {
      // std::cout << "Collision!!!" << std::endl;
      return true;
    }
  }

  // PlayerTank vs Wall_2
  for (auto itrMap = retWall_2.first; itrMap != retWall_2.second; itrMap++) {
    if (myIntersection(retPlayerTank->second->getGlobalBounds(),
                       itrMap->second.rect)) {
      // std::cout << "Collision!!!" << std::endl;
      return true;
    }
  }

  // PlayerTank vs MainWall
  for (auto itrMap = retMainWall.first; itrMap != retMainWall.second;
       itrMap++) {
    if (myIntersection(retPlayerTank->second->getGlobalBounds(),
                       itrMap->second.rect)) {
      // std::cout << "Collision!!!" << std::endl;
      return true;
    }
  }

  // PlayerTank vs WaterWall
  for (auto itrMap = retWaterWall.first; itrMap != retWaterWall.second;
       itrMap++) {
    if (myIntersection(retPlayerTank->second->getGlobalBounds(),
                       itrMap->second.rect)) {
      // std::cout << "Collision!!!" << std::endl;
      return true;
    }
  }

  return false;
}

void Player::isIntersectsOthers() {
  initializeObjects();
  // EnemyTank vs PlayerTank
  auto retPlayerTank = entities.find(Category::PlayerTank);
  if (retPlayerTank != entities.end()) {
    for (auto itrEnemyTank = retEnemy.first; itrEnemyTank != retEnemy.second;
         ++itrEnemyTank) {
      if (myIntersection(retPlayerTank->second->getGlobalBounds(),
                         itrEnemyTank->second->getGlobalBounds())) {
        // std::cout << "isIntersectsOthers1" << std::endl;
        handleAnimation(itrEnemyTank->second->getGlobalBounds(),
                        Textures::TankCollision);
        handleAnimation(retPlayerTank->second->getGlobalBounds(),
                        Textures::TankCollision);
        // std::cout << "isIntersectsOthers2" << std::endl;
        itrEnemyTank->second->~Entity();
        retPlayerTank->second->~Entity();
        // std::cout << "isIntersectsOthers3" << std::endl;
        entities.erase(itrEnemyTank);
        entities.erase(retPlayerTank);
        // std::cout << "isIntersectsOthers4" << std::endl;
        panel.setCurrentLives(0);
        gameStage = Gamestates::gameOver;
        initializeObjects();
        break;
      }
    }
  }
}

void Player::Init() {
  mKeyBinding[sf::Keyboard::Left] = MoveLeft;
  mKeyBinding[sf::Keyboard::Right] = MoveRight;
  mKeyBinding[sf::Keyboard::Up] = MoveUp;
  mKeyBinding[sf::Keyboard::Down] = MoveDown;
  mKeyBinding[sf::Keyboard::Space] = Fire;
  mKeyBinding[sf::Keyboard::LControl] = SuperFire;
  mKeyBinding[sf::Keyboard::P] = Pause;

  Eagle* eagle = new Eagle();

  mapSequence.push(new Map1());
  mapSequence.push(new Map2());
  mapSequence.push(new Map3());
  mapSequence.push(new Map4());
  entities.insert(std::make_pair(Category::Eagle, eagle));

  initialPlayer();
  initializeActions();
  initializeObjects();
}
