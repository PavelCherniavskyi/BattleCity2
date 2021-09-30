#include "../Animation.h"

sf::Clock Animation::clockForTimeOfLiving;
sf::Clock Animation::clockForDraw;

Animation::~Animation()
{
	//std::cout << "Animation deleted!" << std::endl;
	delete []spr;
}

void Animation::update()
{
	sf::Time time = clockForTimeOfLiving.getElapsedTime();
	//std::cout << time.asSeconds() << std::endl;
	if (time.asSeconds() > timeOfLiving){
		//std::cout << "isLiving = false" << std::endl;
		isLiving = false;
	}
}

BulletCollision::BulletCollision() : spritesCount(2)
{
	spr = new sf::Sprite[spritesCount];
	for (size_t i = 0; i < spritesCount; i++)
		spr[i].setTexture(resourses.getTexturePtr(Textures::BulletCollision)[i]);

	for (size_t i = 0; i < spritesCount; i++)
		spr[i].setScale(1.f, 1.f);
	for (size_t i = 0; i < spritesCount; i++)
		spr[i].setOrigin(spr[i].getLocalBounds().height / 2.f, spr[i].getLocalBounds().width / 2.f);

	timeOfLiving = 0.05f;
	isLiving = true;
	//std::cout << "BulletCollision created!" << std::endl;
	clockForTimeOfLiving.restart();
	clockForDraw.restart();
}

void BulletCollision::draw(sf::RenderWindow& window)
{
	sf::Time time = clockForDraw.getElapsedTime();
	if (time.asSeconds() < timeOfLiving / static_cast<float>(spritesCount)){
		//std::cout << "1" << std::endl;
		window.draw(spr[0]);
	}
	else if (time.asSeconds() < timeOfLiving){
		//std::cout << "2" << std::endl;
		window.draw(spr[1]);
	}
}

void BulletCollision::bang(sf::FloatRect rect)
{
	for (size_t i = 0; i < spritesCount; i++){
		spr[i].setPosition(rect.left + rect.width / 2, rect.top + rect.height / 2);
	}
	//std::cout << rect.left << ' ' << rect.top << std::endl;
	//std::cout << rect.width << ' ' << rect.height << std::endl;
}

TankCollision::TankCollision() : spritesCount(3)
{
	spr = new sf::Sprite[spritesCount];
	for (size_t i = 0; i < spritesCount; i++)
		spr[i].setTexture(resourses.getTexturePtr(Textures::TankCollision)[i]);

	for (size_t i = 0; i < spritesCount; i++)
		spr[i].setScale(2.f, 2.f);
	for (size_t i = 0; i < spritesCount; i++)
		spr[i].setOrigin(spr[i].getLocalBounds().height / 2.f, spr[i].getLocalBounds().width / 2.f);

	timeOfLiving = 0.3f;
	isLiving = true;
	//std::cout << "TankCollision created!" << std::endl;
	clockForTimeOfLiving.restart();
	clockForDraw.restart();
}

void TankCollision::draw(sf::RenderWindow& window)
{
	sf::Time time = clockForDraw.getElapsedTime();
	if (time.asSeconds() < timeOfLiving / static_cast<float>(spritesCount)){
		//std::cout << "1" << std::endl;
		window.draw(spr[0]);
	}
	else if (time.asSeconds() < (timeOfLiving / static_cast<float>(spritesCount)) + (timeOfLiving / static_cast<float>(spritesCount))){
		//std::cout << "2" << std::endl;
		window.draw(spr[1]);
	}
	else if (time.asSeconds() < timeOfLiving){
		//std::cout << "3" << std::endl;
		window.draw(spr[2]);
	}
}

void TankCollision::bang(sf::FloatRect rect)
{
	for (size_t i = 0; i < spritesCount; i++){
		spr[i].setPosition(rect.left + rect.width / 2, rect.top + rect.height / 2);
	}
	//std::cout << rect.left << ' ' << rect.top << std::endl;
	//std::cout << rect.width << ' ' << rect.height << std::endl;
}

SuperBulletCollision::SuperBulletCollision() : spritesCount(3)
{
	spr = new sf::Sprite[spritesCount];
	for (size_t i = 0; i < spritesCount; i++)
		spr[i].setTexture(resourses.getTexturePtr(Textures::SuperBulletCollision)[i]);

	for (size_t i = 0; i < spritesCount; i++)
		spr[i].setScale(2.f, 2.f);
	for (size_t i = 0; i < spritesCount; i++)
		spr[i].setOrigin(spr[i].getLocalBounds().height / 2.f, spr[i].getLocalBounds().width / 2.f);

	timeOfLiving = 0.3f;
	isLiving = true;
	//std::cout << "SuperBulletCollision created!" << std::endl;
	clockForTimeOfLiving.restart();
	clockForDraw.restart();
}

void SuperBulletCollision::draw(sf::RenderWindow& window)
{
	sf::Time time = clockForDraw.getElapsedTime();
	if (time.asSeconds() < timeOfLiving / static_cast<float>(spritesCount)){
		//std::cout << "1" << std::endl;
		window.draw(spr[0]);
	}
	else if (time.asSeconds() < (timeOfLiving / static_cast<float>(spritesCount)) + (timeOfLiving / static_cast<float>(spritesCount))){
		//std::cout << "2" << std::endl;
		window.draw(spr[1]);
	}
	else if (time.asSeconds() < timeOfLiving){
		//std::cout << "3" << std::endl;
		window.draw(spr[2]);
	}
}

void SuperBulletCollision::bang(sf::FloatRect rect)
{
	for (size_t i = 0; i < spritesCount; i++){
		spr[i].setPosition(rect.left + rect.width / 2, rect.top + rect.height / 2);
	}
	//std::cout << rect.left << ' ' << rect.top << std::endl;
	//std::cout << rect.width << ' ' << rect.height << std::endl;
}

EagleCollision::EagleCollision() : spritesCount(2)
{
	spr = new sf::Sprite[spritesCount];
	for (size_t i = 0; i < spritesCount; i++)
		spr[i].setTexture(resourses.getTexturePtr(Textures::EagleCollision)[i]);

	for (size_t i = 0; i < spritesCount; i++)
		spr[i].setScale(2.f, 2.f);
	for (size_t i = 0; i < spritesCount; i++)
		spr[i].setOrigin(spr[i].getLocalBounds().height / 2.f, spr[i].getLocalBounds().width / 2.f);

	timeOfLiving = 0.3f;
	isLiving = true;
	//std::cout << "Eagle collision created!" << std::endl;
	clockForTimeOfLiving.restart();
	clockForDraw.restart();
}

void EagleCollision::draw(sf::RenderWindow& window)
{
	sf::Time time = clockForDraw.getElapsedTime();
	if (time.asSeconds() < timeOfLiving / 2){
		//std::cout << "1" << std::endl;
		window.draw(spr[0]);
	}
	else if (time.asSeconds() < timeOfLiving){
		//std::cout << "2" << std::endl;
		window.draw(spr[1]);
	}

}

void EagleCollision::bang(sf::FloatRect rect)
{
	for (size_t i = 0; i < spritesCount; i++){
		spr[i].setPosition(rect.left + rect.width / 2, rect.top + rect.height / 2);
	}
	//std::cout << rect.left << ' ' << rect.top << std::endl;
	//std::cout << rect.width << ' ' << rect.height << std::endl;
}

Apperance::Apperance() : spritesCount(4)
{
	spr = new sf::Sprite[spritesCount];
	for (size_t i = 0; i < spritesCount; i++)
		spr[i].setTexture(resourses.getTexturePtr(Textures::Apperance)[i]);

	for (size_t i = 0; i < spritesCount; i++)
		spr[i].setScale(2.f, 2.f);
	for (size_t i = 0; i < spritesCount; i++)
		spr[i].setOrigin(spr[i].getLocalBounds().height / 2.f, spr[i].getLocalBounds().width / 2.f);

	timeOfLiving = 0.7f;
	isLiving = true;
	//std::cout << "Apperance" << std::endl;
	clockForTimeOfLiving.restart();
	clockForDraw.restart();
}

void Apperance::draw(sf::RenderWindow& window)
{
	sf::Time time = clockForDraw.getElapsedTime();
	if (time.asSeconds() < timeOfLiving / static_cast<float>(spritesCount)){
		//std::cout << "1" << std::endl;
		window.draw(spr[0]);
	}
	else if (time.asSeconds() < (timeOfLiving / static_cast<float>(spritesCount)) + (timeOfLiving / static_cast<float>(spritesCount))){
		//std::cout << "2" << std::endl;
		window.draw(spr[1]);
	}
	else if (time.asSeconds() < (timeOfLiving / static_cast<float>(spritesCount)) + (timeOfLiving / static_cast<float>(spritesCount)) + (timeOfLiving / static_cast<float>(spritesCount))){
		//std::cout << "2" << std::endl;
		window.draw(spr[2]);
	}
	else if (time.asSeconds() < timeOfLiving){
		//std::cout << "3" << std::endl;
		window.draw(spr[3]);
	}

}

void Apperance::bang(sf::FloatRect rect)
{
	for (size_t i = 0; i < spritesCount; i++){
		spr[i].setPosition(rect.left + rect.width / 2, rect.top + rect.height / 2);
	}
	//std::cout << rect.left << ' ' << rect.top << std::endl;
	//std::cout << rect.width << ' ' << rect.height << std::endl;
}
