#pragma once


#include "Entity.h"
#include "Category.h"
#include "ResourceHolder.h"


class Animation : public Resourses
{
public:
	Animation() : resourses(getRes()) {}
	virtual										~Animation();
	virtual void								draw(sf::RenderWindow&) = 0;
	virtual void								update();
	virtual void								bang(sf::FloatRect) = 0;
	virtual bool								isAlife(){ return isLiving; }

protected:

	ResourceHolder&								resourses;
	static sf::Clock							clockForTimeOfLiving;
	static sf::Clock							clockForDraw;
	sf::Sprite*									spr;
	bool										isLiving;
	float										timeOfLiving;

};

class BulletCollision : public Animation
{
public:
	BulletCollision();
	virtual void								draw(sf::RenderWindow&);
	virtual void								bang(sf::FloatRect);
private:
	const size_t								spritesCount;

};

class TankCollision : public Animation
{
public:
	TankCollision();
	virtual void								draw(sf::RenderWindow&);
	virtual void								bang(sf::FloatRect);
private:
	const size_t								spritesCount;
};

class SuperBulletCollision : public Animation
{
public:
	SuperBulletCollision();
	virtual void								draw(sf::RenderWindow&);
	virtual void								bang(sf::FloatRect);
private:
	const size_t								spritesCount;
};

class EagleCollision : public Animation
{
public:
	EagleCollision();
	virtual void								draw(sf::RenderWindow&);
	virtual void								bang(sf::FloatRect);
private:
	const size_t								spritesCount;
};

class Apperance : public Animation
{
public:
	Apperance();
	virtual void								draw(sf::RenderWindow&);
	virtual void								bang(sf::FloatRect);
private:
	const size_t								spritesCount;
};