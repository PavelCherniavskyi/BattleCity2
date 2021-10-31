#include "../BonusHandler.hpp"
#include "../Definitions.hpp"
#include "../Utils/Utils.hpp"

constexpr auto kBonusSpawnTime = 20.f;

void BonusHandler::Draw(sf::RenderWindow& window)
{
  for (const auto& itrBonus : mBonuses)
  {
    itrBonus.second->Draw(window);
  }
}

void BonusHandler::Update()
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
      mBonuses.emplace(EImage::BONUSSTAR, std::move(bonus));
      break;
    case 2:
      bonus = std::make_shared<BonusMissle>();
      bonus->Init();
      bonus->SetPosition(x, y);
      mBonuses.emplace(EImage::BONUSMISSLE, std::move(bonus));
      break;
    case 3:
      bonus = std::make_shared<BonusLife>();
      bonus->SetPosition(x, y);
      bonus->Init();
      mBonuses.emplace(EImage::BONUSLIFE, std::move(bonus));
      break;
    default:
      break;
    }
    clock.restart();
  }
}

 
bool BonusHandler::CheckIntersection(const sf::FloatRect& obj, BonusCIterator& itOut) const
{
  for(auto it = std::begin(mBonuses); it != std::end(mBonuses); ++it)
  {
    if (Utils::Intersection(obj, it->second->GetGlobalBounds()))
    {
      itOut = it;
      return true;
    }
  }
  return false;
}

void BonusHandler::EraseBonus(BonusCIterator it)
{
  mBonuses.erase(it);
}