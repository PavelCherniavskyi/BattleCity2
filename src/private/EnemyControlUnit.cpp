#include "../EnemyControlUnit.hpp"
#include <SFML/System/Clock.hpp>
#include "../Game.h"
#include "../Utils/Utils.hpp"


constexpr auto KMaxEnemyTanksOnField = 20;
constexpr auto KSpawnEnemyTanksTime = 2.f;
constexpr auto KTanksOnFieldLimit = 4u;

EnemyControlUnit::EnemyControlUnit(AnimationHandler& animationHandler)
  : mTanksQueue{}
  , mTanksOnField{}
  , mAnimationHandler(animationHandler)
  , mGameInstance(nullptr)
{
}

bool EnemyControlUnit::Init(Game* aGame)
{
  if(!aGame)
  {
    SPDLOG_ERROR("Game is null");
    return false;
  }

  mGameInstance = aGame;
  return true;
}

bool EnemyControlUnit::Intersection(const sf::FloatRect& obj, EnemyTankIter& itOut) const
{
  for(auto it = mTanksOnField.begin(); it != mTanksOnField.end(); ++it)
  {
    if (Utils::Intersection(it->get()->GetGlobalBounds(), obj))
    {
      itOut = it;
      return true;
    }
  }
    
  return false;
}

void EnemyControlUnit::SetNewBulletCallback(std::function<void(std::shared_ptr<BulletBase>)> aCallback)
{
  mNewBulletCallback = aCallback;
}

size_t EnemyControlUnit::GetTanksOnFieldCount() const
{
  return mTanksOnField.size();
}

size_t EnemyControlUnit::GetTanksQueueCount() const
{
  return mTanksQueue.size();
}

template <typename T>
void EnemyControlUnit::loadLevelHelper(size_t aSize)
{
  const int delta = KMaxEnemyTanksOnField - static_cast<int>(mTanksQueue.size());
  if(delta <= 0)
  {
    SPDLOG_WARN("Queue is already full");
    return;
  }
  for (size_t i = 0; i < aSize; i++)
  {
    mTanksQueue.emplace_back(std::make_shared<T>());
  }
}

void EnemyControlUnit::Spawn()
{
  static sf::Clock clock;
  sf::Time time = clock.getElapsedTime();

  if (time.asSeconds() > KSpawnEnemyTanksTime)
  {
    if (!mTanksQueue.empty())
    {
      const auto currentTanksOnField = mTanksOnField.size();
      if (currentTanksOnField < KTanksOnFieldLimit && !mAnimationHandler.AppearanceIsPending())
      {
        auto popEnemyTank = [this]() {
          mTanksOnField.emplace_back(std::move(mTanksQueue.back()));
          mTanksQueue.pop_back();
          clock.restart();
        };

        const auto rect = mTanksQueue.back()->GetGlobalBounds();
        mAnimationHandler.CreateAnimation(rect, EImage::APPERANCE, popEnemyTank);
      }
    }
  }
}

void EnemyControlUnit::DeleteTank(const EnemyControlUnit::EnemyTankIter& iterator)
{
  mTanksOnField.erase(iterator);
}

bool EnemyControlUnit::LoadLevel(size_t aLevel)
{
  switch (aLevel)
  {
  case 1:
    loadLevelHelper<EnemyTank_20>(2);
    loadLevelHelper<EnemyTank_10>(18);
    break;
  case 2:
    loadLevelHelper<EnemyTank_30>(2);
    loadLevelHelper<EnemyTank_10>(4);
    loadLevelHelper<EnemyTank_10>(14);
    break;
  case 3:
    loadLevelHelper<EnemyTank_30>(4);
    loadLevelHelper<EnemyTank_10>(6);
    loadLevelHelper<EnemyTank_10>(10);
    break;
  case 4:
    loadLevelHelper<EnemyTank_40>(4);
    loadLevelHelper<EnemyTank_30>(6);
    loadLevelHelper<EnemyTank_10>(8);
    loadLevelHelper<EnemyTank_10>(2);
    break;
  default:
    SPDLOG_WARN("Level number not found. Setup hardcore!");
    loadLevelHelper<EnemyTank_40>(20);
    break;
  }

  for(auto& tank : mTanksQueue)
  {
    if(!tank->Init())
    {
      SPDLOG_ERROR("Enemy tank init failed");
      return false;
    }
  }

  Spawn(); // initial start for check game states wouldn't get
          // nextLvl because of empty entity field

  return true;
}

void EnemyControlUnit::setDirectionsForTank(std::shared_ptr<EnemyBaseTank> aTank)
{
  const int direction(std::rand() % 4);
  if (direction == 0)
  {
    aTank->SetVelocity({0.f, +100.f * aTank->GetSpeed()});
    aTank->Rotate(EActions::DOWN);
  }
  else if (direction == 1)
  {
    aTank->SetVelocity({0.f, -100.f * aTank->GetSpeed()});
    aTank->Rotate(EActions::UP);
  }
  else if (direction == 2)
  {
    aTank->SetVelocity({+100 * aTank->GetSpeed(), 0.f});
    aTank->Rotate(EActions::RIGHT);
  }
  else if (direction == 3)
  {
    aTank->SetVelocity({-100 * aTank->GetSpeed(), 0.f});
    aTank->Rotate(EActions::LEFT);
  }
}

void EnemyControlUnit::doFire(std::shared_ptr<EnemyBaseTank> aTank)
{
  if (aTank->CanIDoFire())
  {
    auto superClipSize = aTank->GetSuperClipSize();
    std::shared_ptr<BulletBase> bullet;
    if (superClipSize > 0u)
    {
      bullet = aTank->DoFire(ECategory::SUPERBULLET);
    }
    else
    {
      bullet = aTank->DoFire(ECategory::BULLET);
    }

    mNewBulletCallback(bullet);
  }
}

void EnemyControlUnit::Update(sf::Time elapsedTime)
{
  for(auto& tank : mTanksOnField)
  {
    tank->Update(tank->GetVelocity() * elapsedTime.asSeconds());
    if (mGameInstance->isIntersectsEnemy())
    {
      tank->MoveBack(tank->GetVelocity() * elapsedTime.asSeconds());
      setDirectionsForTank(tank);
    }

    doFire(tank);
  }
}

void EnemyControlUnit::Draw(sf::RenderWindow& aWindow) const
{
  for(const auto& tank : mTanksOnField)
  {
    if(tank)
    {
      tank->Draw(aWindow);
    }
  }
}