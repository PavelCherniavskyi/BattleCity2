#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <queue>

#include "Bullet.h"
#include "Entity.h"

class Tank : public Entity {
 public:
  Tank(Category::Type, Textures::ID);
  virtual ~Tank();
  virtual void draw(sf::RenderWindow& window);
  virtual void update(const sf::Vector2f vector);
  virtual void updateBack(const sf::Vector2f vector);
  virtual void setPosition(float x, float y);
  virtual void rotate(actions action);
  size_t getSpritesCount() { return spritesCount; }
  virtual sf::Sprite* getSprite() { return spr; }
  virtual sf::FloatRect getGlobalBounds();
  virtual sf::FloatRect getLocalBounds();
  void setBulletSpeed(float b) { bulletSpeed = b; }
  float getBulletSpeed() { return bulletSpeed; }
  size_t superClipSize() { return superBulletClip.size(); }
  void superClipPop() { superBulletClip.pop(); }
  void superClipLoad(size_t);
  BulletBase* doFire(Category::Type, Textures::ID);
  void setBulltFrequency(float f) { bulletFrequency = f; }
  float getBulletFrequency() { return bulletFrequency; }
  bool canIdoFire();
  bool canEnemyDoFire();
  void setInitialPosition();
  Textures::ID getType() { return type; }

 private:
  void textInitial();

 private:
  std::queue<std::unique_ptr<SuperBullet>> superBulletClip;
  float bulletSpeed;
  float bulletFrequency;
  const size_t spritesCount;
  Category::Type category;
  Textures::ID type;
  sf::Sprite* spr;
  sf::Sprite numbers[10];
};
