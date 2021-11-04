#include "../Commands.hpp"

void KeyboardCommand::Action(const sf::Time& aTime, std::shared_ptr<PlayerTank> aPlayer, bool aMoveBack)
{
  if (aMoveBack)
  {
    aPlayer->MoveBack((Velocity * aPlayer->GetSpeed()) * aTime.asSeconds());
  }
  else
  {
    aPlayer->SetVelocity(Velocity * aPlayer->GetSpeed());
    aPlayer->Update((Velocity * aPlayer->GetSpeed()) * aTime.asSeconds());
    aPlayer->Rotate(Side);
  }
}

void MouseCommand::Action(std::shared_ptr<PlayerTank> aPlayerTank)
{
  std::shared_ptr<BulletBase> bullet(nullptr);
  if (type == +ECategory::BULLET)
  {
    bullet = aPlayerTank->DoFire(ECategory::BULLET);
  }
  else if (type == +ECategory::SUPERBULLET)
  {
    bullet = aPlayerTank->DoFire(ECategory::SUPERBULLET);
  }

  if(!bullet)
  {
    return;
  }
  
  mNewBulletCallback(bullet);
}