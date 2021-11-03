#include "../AnimationHandler.hpp"
#include "../Definitions.hpp"
#include "../Utils/Utils.hpp"

#include <algorithm>


void AnimationHandler::Draw(sf::RenderWindow& window)
{
  for (const auto& itrAnim : mAnimations)
  {
    itrAnim.second->Draw(window);
  }
}

void AnimationHandler::Update()
{
  for (auto itrAnim = mAnimations.begin(); itrAnim != mAnimations.end(); itrAnim++)
  {
    if (itrAnim->second->IsAlife())
    {
      itrAnim->second->update();
    }
    else
    {
      mAnimations.erase(itrAnim);
      break;
    }
  }

  for (auto ap = mAppearances.begin(); ap != mAppearances.end(); ap++)
  {
    if(ap->first.expired())
    {
      ap->second();
      mAppearances.erase(ap);
      mAppearanceCallback();
      break;
    }
  }
}

bool AnimationHandler::AppearanceIsPending()
{
  return !mAppearances.empty();
}

void AnimationHandler::SetAppearanceFinishCallback(const std::function<void()> aCallback)
{
  mAppearanceCallback = aCallback;
}

void AnimationHandler::CreateAnimation(const sf::FloatRect& rect, EImage type, const std::function<void()> popEnemyTank)
{
  std::shared_ptr<Animation> anim;

  switch (type)
  {
  case +EImage::BULLETCOLLISION:
    anim = std::make_shared<BulletCollision>();
    break;
  case +EImage::TANKCOLLISION:
    anim = std::make_shared<TankCollision>();
    break;
  case +EImage::SUPERBULLETCOLLISION:
    anim = std::make_shared<SuperBulletCollision>();
    break;
  case +EImage::EAGLECOLLISION:
    anim = std::make_shared<EagleCollision>();
    break;
  case +EImage::APPERANCE:
    anim = std::make_shared<Apperance>();
    mAppearances.emplace_back(anim, popEnemyTank);
    break;
  default:
    SPDLOG_ERROR("Wrong animation type");
    return;
  }

  anim->Init();
  anim->SetPosition(rect);
  mAnimations.emplace(type, std::move(anim));
}