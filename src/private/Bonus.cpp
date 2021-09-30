#include "../Bonus.h"

BaseBonus::~BaseBonus(){}

BonusStar::BonusStar(Textures::ID t) 
{
	spr = new sf::Sprite(resourses.getTexturePtr(Textures::BonusStar)[0]);

	spr->setScale(2.f, 2.f);
	//spr->setOrigin(spr->getTextureRect().height / 2.f, spr->getTextureRect().width / 2.f);
	type = t;
	pack = 1;

	//std::cout << "BonusStar created!" << std::endl;
}

BonusStar::~BonusStar()
{
	delete spr;
	//std::cout << "BonusStar deleted" << std::endl;
}

BonusMissle::BonusMissle(Textures::ID t)
{
	spr = new sf::Sprite(resourses.getTexturePtr(Textures::BonusMissle)[0]);

	spr->setScale(2.f, 2.f);
	//spr->setOrigin(spr->getTextureRect().height / 2.f, spr->getTextureRect().width / 2.f);
	type = t;
	pack = 5;

	//std::cout << "BonusStar created!" << std::endl;
}

BonusMissle::~BonusMissle()
{
	delete spr;
	//std::cout << "BonusStar deleted" << std::endl;
}

BonusLife::BonusLife(Textures::ID t)
{
	spr = new sf::Sprite(resourses.getTexturePtr(Textures::BonusLife)[0]);

	spr->setScale(2.f, 2.f);
	//spr->setOrigin(spr->getTextureRect().height / 2.f, spr->getTextureRect().width / 2.f);
	type = t;
	pack = 1;

	//std::cout << "BonusStar created!" << std::endl;
}

BonusLife::~BonusLife()
{
	delete spr;
	//std::cout << "BonusStar deleted" << std::endl;
}

void BaseBonus::draw(sf::RenderWindow & window)
{
	window.draw(*spr);
}

void BaseBonus::update(const sf::Vector2f vector)
{
	spr->move(vector);
}

sf::FloatRect BaseBonus::getGlobalBounds()
{
	return spr->getGlobalBounds();
}

sf::FloatRect	BaseBonus::getLocalBounds()
{
	return spr->getLocalBounds();
}

void BaseBonus::setPosition(float x, float y)
{
	spr->setPosition(x, y);
}
