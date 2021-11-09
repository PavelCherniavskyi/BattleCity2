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
  Entity(EImage aType);
  Entity(const Entity&);
  virtual ~Entity() = default;
  virtual void Draw(sf::RenderWindow&) const = 0;
  virtual void Update(const sf::Vector2f& = sf::Vector2f()) = 0;
  virtual bool Init() = 0;
  
  bool IsAlife() const;
  size_t MakeDamage(size_t);
  void Rotate(EActions);

  sf::FloatRect GetGlobalBounds() const;
  sf::FloatRect GetLocalBounds() const;
  const std::vector<sf::Sprite>& GetSprite() const;
  EImage GetImageType() const;

  void SetPosition(const sf::Vector2f&);
  void SetPosition(const sf::FloatRect&);
  sf::Vector2f GetPosition() const;
  
  void SetHP(size_t aHP);
  size_t GetHP() const;

  void SetVelocity(const sf::Vector2f& aVelocity);
  const sf::Vector2f& GetVelocity() const;

  void SetSpeed(float aSpeed);
  float GetSpeed() const;

  void SetIsMoving(bool aMoving);
  bool IsMoving() const;

protected:
  bool mIsMoving;
  sf::Vector2f mVelocity;
  float mSpeed;
  size_t mHP;
  std::vector<sf::Sprite> mSprites;
  EImage mImageType;
};

#endif// BATTLECITY_ENTITY_HPP