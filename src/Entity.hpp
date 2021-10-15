#ifndef BATTLECITY_ENTITY_HPP
#define BATTLECITY_ENTITY_HPP

#include <SFML/Graphics.hpp>
#include "Enums.hpp"

class BulletBase;

class Entity
{
public:
  virtual ~Entity() = default;
  virtual void Draw(sf::RenderWindow &) = 0;
  virtual void Update(const sf::Vector2f&) {}
  virtual void UpdateBack(const sf::Vector2f&) {}
  virtual const std::vector<sf::Sprite>& GetSprite() const = 0;
  virtual sf::FloatRect GetGlobalBounds() const = 0;
  virtual sf::FloatRect GetLocalBounds() const = 0;
  virtual void Rotate(EActions) {}
  virtual void SetPosition(const sf::Vector2f&) {}
  virtual bool Init() = 0;
  virtual void SetSpeed(float aSpeed);
  virtual bool CanIDoFire() const;
  virtual size_t GetSuperClipSize() const;
  virtual EImage GetType() const;
  virtual std::shared_ptr<BulletBase> DoFire(ECategory);
  virtual void SetBulletSpeed(const float) {}
  virtual float GetBulletSpeed() const;
  virtual void SuperClipLoad(const size_t) {}
  virtual void SetInitialPosition() {}
  bool IsAlife() const;
  void Kill();
  void SetVelocity(const sf::Vector2f& aVelocity);
  const sf::Vector2f& GetVelocity() const;
  float GetSpeed() const;
  void SetIsMoving(bool aMoving);
  bool IsMoving() const;
  int GetHP() const;
  void SetHP(int aHP);

protected:
  bool mMoving;
  sf::Vector2f mVelocity;
  float mSpeed;
  int mHP;
};

#endif// BATTLECITY_ENTITY_HPP