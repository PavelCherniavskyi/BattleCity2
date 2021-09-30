#pragma once

#include <SFML/Graphics.hpp>
#include "ResourceHolder.h"

class Resourses
{
public:
  static void Init() { res->Init(); };
  static ResourceHolder &getRes() { return *res; }

private:
  static std::unique_ptr<ResourceHolder> res;
};


class Entity : public Resourses
{
public:
  enum actions
  {
    Left,
    Right,
    Up,
    Down,
    Fire
  };
  Entity()
     : resourses(Resourses::getRes())
  {
  }
  virtual ~Entity() {}
  virtual void draw(sf::RenderWindow &window) = 0;
  virtual void update(const sf::Vector2f) {}
  virtual sf::Sprite *getSprite() = 0;
  virtual void updateBack(const sf::Vector2f) {}
  virtual sf::FloatRect getGlobalBounds() = 0;
  virtual void rotate(actions) {}
  virtual void setPosition(float, float) {}
  virtual sf::FloatRect getLocalBounds() = 0;
  bool isAlife() const { return HP >= 0; }
  void kill() { HP -= 1; }
  void setVelocity(sf::Vector2f aVelocity) { velocity = aVelocity; }
  void setVelocity(float x, float y)
  {
    velocity.x = x;
    velocity.y = y;
  }
  sf::Vector2f getVelocity() { return velocity; }
  virtual void setSpeed(float s) { speed = s; }
  float getSpeed() { return speed; }

  void setIsMoving(bool aMoving) { moving = aMoving; }
  bool isMoving() { return moving; }
  int getHP() { return HP; }
  void setHP(int life) { HP = life; }

protected:
  bool moving;
  sf::Vector2f velocity;
  float speed;
  ResourceHolder &resourses;
  int HP;
};

class Eagle : public Entity
{
public:
  Eagle();
  virtual ~Eagle();
  virtual void draw(sf::RenderWindow &window);
  virtual sf::FloatRect getGlobalBounds();
  virtual sf::FloatRect getLocalBounds();
  virtual sf::Sprite *getSprite() { return spr; }

private:
  sf::Sprite *spr;
  size_t spritesCount;
};
