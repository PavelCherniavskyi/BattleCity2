#include "../Tank.hpp"
#include "../Definitions.hpp"
#include "../ResourceHolders/SpriteHolder.hpp"

constexpr auto kNumbersCount = 10u;
constexpr auto kNumbersScale = 1.f;

constexpr auto kTankSpritesCount = 2u;

constexpr auto kPlayerTankSuperBulletClipSize = 5u;
const auto kPlayerTankVelosity = sf::Vector2f(0.f, -100.f);
constexpr auto kPlayerTankBulletFrequency = 0.4f;
constexpr auto kPlayerBulletSpeed = 2.f;
constexpr auto kPlayerTankHP = 3u;
constexpr auto kPlayerTankSpeed = 1.f;

const std::vector<sf::Vector2f> kEnemySpawnPos{ 
  { 32, 32 },
  { (kWidthScreen + 11) / 2, 32 },
  { kWidthScreen - 23, 33 }
};

constexpr auto kEnemy_10TankSpeed = 1.f;
constexpr auto kEnemy_10BulletSpeed = 2.f;
constexpr auto kEnemy_10TankHP = 1;
constexpr auto kEnemy_10TankBulletFrequency = 1.f;
constexpr auto kEnemy_10TankScale = 1.9f;

constexpr auto kEnemy_20TankSpeed = 1.7f;
constexpr auto kEnemy_20BulletSpeed = 2.f;
constexpr auto kEnemy_20TankHP = 1;
constexpr auto kEnemy_20TankBulletFrequency = 1.f;
constexpr auto kEnemy_20TankScale = 1.8f;

constexpr auto kEnemy_30TankSpeed = 1.2f;
constexpr auto kEnemy_30BulletSpeed = 4.f;
constexpr auto kEnemy_30TankHP = 2;
constexpr auto kEnemy_30TankBulletFrequency = 1.f;
constexpr auto kEnemy_30TankScale = 1.7f;

constexpr auto kEnemy_40TankSpeed = 1.f;
constexpr auto kEnemy_40BulletSpeed = 2.f;
constexpr auto kEnemy_40TankHP = 3;
constexpr auto kEnemy_40TankBulletFrequency = 1.f;
constexpr auto kEnemy_40TankScale = 1.8f;

std::shared_ptr<BulletBase> setupBullet(std::shared_ptr<BulletBase> aBullet, BaseTank& aBaseTank)
{
  if (!aBullet)
  {
    SPDLOG_ERROR("Bullet is null");
    return nullptr;
  }

  aBullet->SetSpeed(aBaseTank.GetBulletSpeed());
  sf::Vector2f entVelocity = aBaseTank.GetVelocity() / aBaseTank.GetSpeed();
  aBullet->SetVelocity(entVelocity * aBaseTank.GetBulletSpeed());

  if (entVelocity.y < 0)
  { // Up
    aBullet->SetPosition({ aBaseTank.GetGlobalBounds().left + aBaseTank.GetGlobalBounds().width / 2 + 1,
      aBaseTank.GetGlobalBounds().top - 10 });
    aBullet->Rotate(EActions::UP);
  }
  else if (entVelocity.y > 0)
  { // Down
    aBullet->SetPosition({ aBaseTank.GetGlobalBounds().left + aBaseTank.GetGlobalBounds().width / 2 - 1,
      aBaseTank.GetGlobalBounds().top + aBaseTank.GetGlobalBounds().height + 10 });
    aBullet->Rotate(EActions::DOWN);
  }
  else if (entVelocity.x < 0)
  { // Left
    aBullet->SetPosition({ aBaseTank.GetGlobalBounds().left - 10,
      aBaseTank.GetGlobalBounds().top + aBaseTank.GetGlobalBounds().height / 2 - 1 });
    aBullet->Rotate(EActions::LEFT);
  }
  else if (entVelocity.x > 0)
  { // Right
    aBullet->SetPosition({ aBaseTank.GetGlobalBounds().left + aBaseTank.GetGlobalBounds().width + 10,
      aBaseTank.GetGlobalBounds().top + aBaseTank.GetGlobalBounds().height / 2 + 1 });
    aBullet->Rotate(EActions::RIGHT);
  }

  return aBullet;
}

BaseTank::BaseTank(ECategory aCategory, EImage aType)
  : Entity(aType)
  , mCategory(aCategory)
{
}

BaseTank::BaseTank(const BaseTank& rhs)
  : Entity(rhs)
{
  mSuperBulletClip = rhs.mSuperBulletClip;
  mBulletSpeed = rhs.mBulletSpeed;
  mBulletFrequency = rhs.mBulletFrequency;
  mCategory = rhs.mCategory;
}

bool PlayerTank::CanIDoFire() const
{
  static sf::Clock clock;
  if (clock.getElapsedTime().asSeconds() > mBulletFrequency)
  {
    clock.restart();
    return true;
  }
  return false;
}

void BaseTank::SetBulletSpeed(const float aSpeed)
{
  mBulletSpeed = aSpeed;
}

float BaseTank::GetBulletSpeed() const
{
  return mBulletSpeed;
}

std::shared_ptr<BulletBase> BaseTank::DoFire(ECategory aCategory)
{
  std::shared_ptr<BulletBase> bullet(nullptr);

  if (aCategory == +ECategory::BULLET)
  {
    bullet = std::make_unique<SimpleBullet>();
    if(!bullet->Init())
    {
      return nullptr;
    }
  }
  else if(aCategory == +ECategory::SUPERBULLET && !mSuperBulletClip.empty())
  {
    bullet = std::move(mSuperBulletClip.front());
    mSuperBulletClip.pop();
  }

  if(bullet)
  {
    return setupBullet(std::move(bullet), *this);
  }

  return nullptr;
}

void BaseTank::SetBulletFrequency(const float aBulletFrequency)
{
  mBulletFrequency = aBulletFrequency;
}

float BaseTank::GetBulletFrequency() const
{
  return mBulletFrequency;
}

ECategory BaseTank::GetCategory() const
{
  return mCategory;
}

void BaseTank::Draw(sf::RenderWindow& aWindow) const
{
  if (IsMoving())
  {
    static sf::Clock clock;
    sf::Time time = clock.getElapsedTime();
    if (time.asSeconds() < 0.05f)
    {
      aWindow.draw(mSprites[0]);
    }
    else if (time.asSeconds() < 0.1f)
    {
      aWindow.draw(mSprites[1]);
    }
    else
    {
      clock.restart();
    }
  }
  else
  {
    aWindow.draw(mSprites[0]);
  }
}

void BaseTank::Update(const sf::Vector2f& aPos)
{
  for (size_t i = 0; i < kTankSpritesCount; i++)
  {
    mSprites[i].move(aPos);
  }
}

void BaseTank::MoveBack(const sf::Vector2f& aPos)
{
  sf::Vector2f pos = aPos;
  if (aPos.y < 0)
  { // Up
    pos.y = +(-aPos.y);
  }
  else if (aPos.y > 0)
  { // Down
    pos.y = -aPos.y;
  }
  else if (aPos.x < 0)
  { // Left
    pos.x = +(-aPos.x);
  }
  else if (aPos.x > 0)
  { // Right
    pos.x = -aPos.x;
  }
  for (size_t i = 0; i < kTankSpritesCount; i++)
  {
    mSprites[i].move(pos);
  }
}

void BaseTank::SuperClipLoad(const size_t mCount)
{
  for (size_t i = 0; i < mCount; i++)
  {
    auto bullet = std::make_unique<SuperBullet>();
    bullet->Init();
    mSuperBulletClip.push(std::move(bullet));
  }
}

size_t BaseTank::GetSuperClipSize() const
{
  return mSuperBulletClip.size();
}

PlayerTank::PlayerTank()
  : BaseTank(ECategory::PLAYERTANK, EImage::T_10)
{
}

PlayerTank::PlayerTank(const PlayerTank& rhs)
  : BaseTank(rhs)
{
}

bool PlayerTank::Init()
{
  auto sprites = SpriteHolder::GetSprite(mImageType);
  if (!sprites || sprites->size() != kTankSpritesCount)
  {
    SPDLOG_ERROR("Sprites is not correct");
    return false;
  }

  mSprites = *sprites;

  for (size_t i = 0; i < kPlayerTankSuperBulletClipSize; i++)
  {
    auto bullet = std::make_unique<SuperBullet>();
    if(!bullet->Init())
    {
      SPDLOG_ERROR("Something wrong with SuperBullet");
      return false;
    }
    mSuperBulletClip.push(std::move(bullet));
  }
  for (size_t i = 0; i < kTankSpritesCount; i++)
  {
    mSprites[i].setScale(1.9f, 1.9f);
    mSprites[i].setOrigin(mSprites[i].getLocalBounds().height / 2.f, mSprites[i].getLocalBounds().width / 2.f);

  }

  SetInitialPosition();
  SetVelocity(kPlayerTankVelosity);
  SetBulletFrequency(kPlayerTankBulletFrequency);
  SetSpeed(kPlayerTankSpeed);
  CanIDoFire();
  SetBulletSpeed(kPlayerBulletSpeed);
  SetHP(kPlayerTankHP);

  return true;
}

void PlayerTank::SetInitialPosition()
{
  for (size_t i = 0; i < kTankSpritesCount; i++)
  {
    mSprites[i].setPosition(175.f, 418.f);
  }
}

EnemyBaseTank::EnemyBaseTank(ECategory aCategory, EImage aType)
  : BaseTank(aCategory, aType)
  , mNumbers()
  , mRotationTime()
{
}

bool EnemyBaseTank::CanIDoFire() const
{
  static sf::Clock clock;
  static float fireRange = 1;

  if (clock.getElapsedTime().asSeconds() > fireRange)
  {
    fireRange = static_cast<float>(rand() % 20 + 10);
    fireRange /= 10;
    clock.restart();
    return true;
  }
  return false;
}

void EnemyBaseTank::SetCallbackToRotate(std::function<void(EnemyBaseTank&)> callback)
{
  mCallbackToRotate = callback;
}

void EnemyBaseTank::UpdateRotationTime()
{
  mRotationTime = sf::seconds(static_cast<float>(rand() % 5 + 1));
}

void EnemyBaseTank::Update(const sf::Vector2f& aPos)
{
  static sf::Clock rotationTimer;
  for (size_t i = 0; i < kTankSpritesCount; i++)
  {
    mSprites[i].move(aPos);
  }

  mNumbers[GetHP()].setPosition(
    { GetGlobalBounds().left + GetGlobalBounds().width, GetGlobalBounds().top + GetGlobalBounds().height });


  if(rotationTimer.getElapsedTime() > mRotationTime)
  {
    mCallbackToRotate(*this);
    rotationTimer.restart();
  }
}

bool EnemyBaseTank::Init()
{
  auto numbers = SpriteHolder::GetSprite(EImage::DIGITS);
  if (!numbers || numbers->size() != kNumbersCount)
  {
    SPDLOG_ERROR("Sprites is not correct");
    return false;
  }
  mNumbers = *numbers;
  UpdateRotationTime();
  SetIsMoving(true);

  for (size_t i = 0; i < 10; i++)
  {
    mNumbers[i].setScale(kNumbersScale, kNumbersScale);
  }
  return true;
}

void EnemyBaseTank::Draw(sf::RenderWindow& aWindow) const
{
  if (IsMoving())
  {
    static sf::Clock clock;
    sf::Time time = clock.getElapsedTime();
    if (time.asSeconds() < 0.05f)
    {
      aWindow.draw(mSprites[0]);
    }
    else if (time.asSeconds() < 0.1f)
    {
      aWindow.draw(mSprites[1]);
    }
    else
    {
      clock.restart();
    }
  }
  else
  {
    aWindow.draw(mSprites[0]);
  }

  // drawing lives
  aWindow.draw(mNumbers[GetHP()]);
}

EnemyTank_10::EnemyTank_10()
  : EnemyBaseTank(ECategory::ENEMYTANK, EImage::ENEMY_10)
{
}

bool EnemyTank_10::Init()
{
  auto sprites = SpriteHolder::GetSprite(mImageType);
  if (!sprites || sprites->size() != kTankSpritesCount)
  {
    SPDLOG_ERROR("Sprites is not correct");
    return false;
  }
  mSprites = *sprites;
  mBulletSpeed = kEnemy_10BulletSpeed;
  SetHP(kEnemy_10TankHP);
  SetBulletFrequency(kEnemy_10TankBulletFrequency);
  SetSpeed(kEnemy_10TankSpeed);
  SetVelocity({0.f, +100.f * GetSpeed()});
  Rotate(EActions::DOWN);
  CanIDoFire(); // for clock initialization

  const size_t spawnIndex = static_cast<size_t>(rand()) % kEnemySpawnPos.size();
  for (size_t i = 0; i < mSprites.size(); i++)
  {
    mSprites[i].setScale(kEnemy_10TankScale, kEnemy_10TankScale);
    mSprites[i].setPosition(kEnemySpawnPos[static_cast<size_t>(spawnIndex)]);
    mSprites[i].setOrigin(mSprites[i].getLocalBounds().height / 2.f, mSprites[i].getLocalBounds().width / 2.f);
  }

  return EnemyBaseTank::Init();
}

EnemyTank_20::EnemyTank_20()
  : EnemyBaseTank(ECategory::ENEMYTANK, EImage::ENEMY_20)
{
}

bool EnemyTank_20::Init()
{
  auto sprites = SpriteHolder::GetSprite(mImageType);
  if (!sprites || sprites->size() != kTankSpritesCount)
  {
    SPDLOG_ERROR("Sprites for type {} is not correct. CurSize: {}, Expected: {}"
      , mImageType._to_string()
      , (sprites ? std::to_string(sprites->size()).c_str() : "sprites is NULL")
      , kTankSpritesCount);
    return false;
  }
  mSprites = *sprites;
  mBulletSpeed = kEnemy_20BulletSpeed;
  SetHP(kEnemy_20TankHP);
  SetBulletFrequency(kEnemy_20TankBulletFrequency);
  SetSpeed(kEnemy_20TankSpeed);
  SetVelocity({0.f, +100.f * GetSpeed()});
  Rotate(EActions::DOWN);
  CanIDoFire(); // for clock initialization

  const size_t spawnIndex = static_cast<size_t>(rand()) % kEnemySpawnPos.size();
  for (size_t i = 0; i < mSprites.size(); i++)
  {
    mSprites[i].setScale(kEnemy_20TankScale, kEnemy_20TankScale);
    mSprites[i].setPosition(kEnemySpawnPos[spawnIndex]);
    mSprites[i].setOrigin(mSprites[i].getLocalBounds().height / 2.f, mSprites[i].getLocalBounds().width / 2.f);
  }

  return EnemyBaseTank::Init();
}

EnemyTank_30::EnemyTank_30()
  : EnemyBaseTank(ECategory::ENEMYTANK, EImage::ENEMY_30)
{
}

bool EnemyTank_30::Init()
{
  auto sprites = SpriteHolder::GetSprite(mImageType);
  if (!sprites || sprites->size() != kTankSpritesCount)
  {
    SPDLOG_ERROR("Sprites is not correct");
    return false;
  }
  mSprites = *sprites;
  mBulletSpeed = kEnemy_30BulletSpeed;
  SetHP(kEnemy_30TankHP);
  SetBulletFrequency(kEnemy_30TankBulletFrequency);
  SetSpeed(kEnemy_30TankSpeed);
  SetVelocity({0.f, +100.f * GetSpeed()});
  Rotate(EActions::DOWN);
  CanIDoFire(); // for clock initialization

  const size_t spawnIndex = static_cast<size_t>(rand()) % kEnemySpawnPos.size();
  for (size_t i = 0; i < mSprites.size(); i++)
  {
    mSprites[i].setScale(kEnemy_30TankScale, kEnemy_30TankScale);
    mSprites[i].setPosition(kEnemySpawnPos[spawnIndex]);
    mSprites[i].setOrigin(mSprites[i].getLocalBounds().height / 2.f, mSprites[i].getLocalBounds().width / 2.f);
  }

  return EnemyBaseTank::Init();
}

EnemyTank_40::EnemyTank_40()
  : EnemyBaseTank(ECategory::ENEMYTANK, EImage::ENEMY_40)
{
}

bool EnemyTank_40::Init()
{
  auto sprites = SpriteHolder::GetSprite(mImageType);
  if (!sprites || sprites->size() != kTankSpritesCount)
  {
    SPDLOG_ERROR("Sprites is not correct");
    return false;
  }
  mSprites = *sprites;
  mBulletSpeed = kEnemy_40BulletSpeed;
  SetHP(kEnemy_40TankHP);
  SetBulletFrequency(kEnemy_40TankBulletFrequency);
  SetSpeed(kEnemy_40TankSpeed);
  SetVelocity({0.f, +100.f * GetSpeed()});
  Rotate(EActions::DOWN);
  CanIDoFire(); // for clock initialization

  const size_t spawnIndex = static_cast<size_t>(rand()) % kEnemySpawnPos.size();
  for (size_t i = 0; i < mSprites.size(); i++)
  {
    mSprites[i].setScale(kEnemy_40TankScale, kEnemy_40TankScale);
    mSprites[i].setPosition(kEnemySpawnPos[spawnIndex]);
    mSprites[i].setOrigin(mSprites[i].getLocalBounds().height / 2.f, mSprites[i].getLocalBounds().width / 2.f);
  }

  return EnemyBaseTank::Init();
}