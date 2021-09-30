#pragma once

#include "Entity.h"


class BaseBonus : public Resourses
{
public:
	BaseBonus() :resourses(Resourses::getRes()){}
	virtual										~BaseBonus() = 0;
	virtual void								draw(sf::RenderWindow & window);
	virtual void								update(const sf::Vector2f vector);
	virtual sf::FloatRect						getGlobalBounds();
	virtual void								setPosition(float x, float y);
	sf::Vector2f								getPosition();
	virtual sf::FloatRect						getLocalBounds();
	virtual sf::Sprite*							getSprite(){ return spr; }
	virtual size_t								getPack() = 0;
	virtual Textures::ID						getType() { return type; }
	
protected:
	sf::Sprite*									spr;
	Textures::ID								type;
	ResourceHolder&								resourses;
};

class BonusStar : public BaseBonus
{
public:
	BonusStar(Textures::ID);
	virtual										~BonusStar();
	virtual size_t								getPack(){ return pack; }
private:
	size_t										pack;
	
};

class BonusMissle : public BaseBonus
{
public:
	BonusMissle(Textures::ID);
	virtual										~BonusMissle();
	virtual size_t								getPack(){ return pack; }
private:
	size_t										pack;
};

class BonusLife : public BaseBonus
{
public:
	BonusLife(Textures::ID);
	virtual										~BonusLife();
	virtual size_t								getPack(){ return pack; }
private:
	size_t										pack;
};
