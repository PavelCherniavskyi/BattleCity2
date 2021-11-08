#ifndef BATTLECITY_ENTITY_HPP
#define BATTLECITY_ENTITY_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "Enums.hpp"

class BulletBase;

class Entity
{
public:
  Entity();
  Entity(const Entity&);
  virtual ~Entity() = default;
  virtual void Draw(sf::RenderWindow &) = 0;
  virtual void Update(const sf::Vector2f&) {}
  virtual const std::vector<sf::Sprite>& GetSprite() const = 0;
  virtual sf::FloatRect GetGlobalBounds() const = 0;
  virtual sf::FloatRect GetLocalBounds() const = 0;
  virtual void Rotate(EActions) {}
  virtual void SetPosition(const sf::Vector2f&) {}
  virtual bool Init() = 0;
  virtual EImage GetImageType() const;
  virtual void SetInitialPosition() {}
  virtual bool IsAlife() const;
  virtual size_t MakeDamage();

  size_t GetHP() const;
  void SetHP(size_t aHP);
  void SetVelocity(const sf::Vector2f& aVelocity);
  const sf::Vector2f& GetVelocity() const;
  void SetSpeed(float aSpeed);
  float GetSpeed() const;
  void SetIsMoving(bool aMoving);
  bool IsMoving() const;

protected:
  bool mMoving;
  sf::Vector2f mVelocity;
  float mSpeed;
  size_t mHP;
};

#endif// BATTLECITY_ENTITY_HPP