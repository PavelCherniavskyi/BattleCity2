#pragma once

#include "Entity.h"

class BulletBase: public Entity
{
public:
	BulletBase() {}
	virtual										~BulletBase() = 0;
	virtual void								draw(sf::RenderWindow & window);
	virtual void								update(const sf::Vector2f vector);
	virtual sf::FloatRect						getGlobalBounds();
	virtual void								setPosition(float x, float y);
	sf::Vector2f								getPosition();
	virtual void								rotate(actions action);
	virtual sf::FloatRect						getLocalBounds();
	virtual sf::Sprite*							getSprite(){ return spr; }
protected:
	sf::Sprite*									spr;
};

class SimpleBullet : public BulletBase
{
public:
	SimpleBullet();
	virtual										~SimpleBullet();


private:
	
};

class SuperBullet : public BulletBase
{
public:
	SuperBullet();
	virtual										~SuperBullet();
	
};