#include "../Tank.h"



Tank::Tank(Category::Type cat, Textures::ID aType) : spritesCount(2), category(cat), type(aType)
{
	spr = new sf::Sprite[spritesCount];

	

	if (category == Category::PlayerTank){
		for (size_t i = 0; i < spritesCount; i++)
			spr[i].setTexture(resourses.getTexturePtr(Textures::T_10)[i]);
		for (size_t i = 0; i < 5; i++){//initial superClip size
			superBulletClip.push(std::unique_ptr<SuperBullet>(new SuperBullet()));
		}
		for (size_t i = 0; i < spritesCount; i++)
			spr[i].setScale(1.9f, 1.9f);

		setInitialPosition();
		setVelocity(0.f, -100.f);
		setBulltFrequency(0.4f);
		setSpeed(1.f);
		bulletSpeed = 2;
		HP = 2;
	}
	if (category == Category::EnemyTank){
		sf::Vector2f spawnPos[3] = { { 32, 32 }, { (WIDTH_SCREEN + WIDTH_RIGHT_PANEL - 14 ) / 2, 32 }, { WIDTH_SCREEN - 23, 33 } };
		int spawn(rand() % 3);

		

		if (type == Textures::Enemy_10){
			for (size_t i = 0; i < spritesCount; i++)
				spr[i].setTexture(resourses.getTexturePtr(Textures::Enemy_10)[i]);
			setSpeed(1.f);
			bulletSpeed = 2;
			HP = 0;
			setBulltFrequency(1.f);
			for (size_t i = 0; i < spritesCount; i++)
				spr[i].setScale(1.9f, 1.9f);
		}
		else if (type == Textures::Enemy_20){
			for (size_t i = 0; i < spritesCount; i++)
				spr[i].setTexture(resourses.getTexturePtr(Textures::Enemy_20)[i]);
			setSpeed(1.7f);
			bulletSpeed = 2;
			HP = 0;
			setBulltFrequency(1.f);
			for (size_t i = 0; i < spritesCount; i++)
				spr[i].setScale(1.8f, 1.8f);
		}
		else if (type == Textures::Enemy_30){
			for (size_t i = 0; i < spritesCount; i++)
				spr[i].setTexture(resourses.getTexturePtr(Textures::Enemy_30)[i]);
			setSpeed(1.2f);
			bulletSpeed = 4;
			setBulltFrequency(1.f);
			HP = 1;
			for (size_t i = 0; i < spritesCount; i++)
				spr[i].setScale(1.7f, 1.7f);
		}
		else if (type == Textures::Enemy_40){
			for (size_t i = 0; i < spritesCount; i++)
				spr[i].setTexture(resourses.getTexturePtr(Textures::Enemy_40)[i]);
			setSpeed(1.f);
			bulletSpeed = 2;
			setBulltFrequency(1.f);
			HP = 3;
			for (size_t i = 0; i < spritesCount; i++)
				spr[i].setScale(1.8f, 1.8f);
		}



		for (size_t i = 0; i < spritesCount; i++){
			spr[i].setPosition(spawnPos[spawn]);
		}
		setIsMoving(true);
		setVelocity(0.f, +100.f * getSpeed());
		rotate(Entity::Down);
	}

	
	for (size_t i = 0; i < spritesCount; i++)
		spr[i].setOrigin(spr[i].getLocalBounds().height / 2.f, spr[i].getLocalBounds().width / 2.f);
	
	
	
	canIdoFire(); // for initial clock start
	textInitial();
	spdlog::info("superBulletClip size: {}", superBulletClip.size());
}

void Tank::setInitialPosition()
{
	for (size_t i = 0; i < spritesCount; i++){
		spr[i].setPosition(175.f, 418.f);
	}
}

void Tank::textInitial()
{
	numbers[0].setTexture(resourses.getTexturePtr(Textures::Digit_0)[0]);
	numbers[1].setTexture(resourses.getTexturePtr(Textures::Digit_1)[0]);
	numbers[2].setTexture(resourses.getTexturePtr(Textures::Digit_2)[0]);
	numbers[3].setTexture(resourses.getTexturePtr(Textures::Digit_3)[0]);
	numbers[4].setTexture(resourses.getTexturePtr(Textures::Digit_4)[0]);
	numbers[5].setTexture(resourses.getTexturePtr(Textures::Digit_5)[0]);
	numbers[6].setTexture(resourses.getTexturePtr(Textures::Digit_6)[0]);
	numbers[7].setTexture(resourses.getTexturePtr(Textures::Digit_7)[0]);
	numbers[8].setTexture(resourses.getTexturePtr(Textures::Digit_8)[0]);
	numbers[9].setTexture(resourses.getTexturePtr(Textures::Digit_9)[0]);

	for (size_t i = 0; i < 10; i++){
		numbers[i].setScale(1, 1);
	}
}

bool Tank::canIdoFire()
{
	static sf::Clock clock;
	sf::Time time = clock.getElapsedTime();

	if (time.asSeconds() > bulletFrequency){
		clock.restart();
		return true;
	}
	return false;
}

bool Tank::canEnemyDoFire()
{
	static sf::Clock clock;
	sf::Time time = clock.getElapsedTime();
	static float fireRange = 1; // initial value
	
	if (time.asSeconds() > fireRange){
		fireRange = static_cast<float>(rand() % 20 + 10);
		fireRange /= 10;
		clock.restart();
		return true;
	}
	return false;
}

BulletBase* Tank::doFire(Category::Type aCategory, Textures::ID aType )
{
	BulletBase * bullet = nullptr;
	
	if (aType == Textures::T_10){
		if (aCategory == Category::SuperBullet){
			bullet = superBulletClip.front().release();
			superBulletClip.pop();
		}
		else if (aCategory == Category::Bullet){
			bullet = new SimpleBullet();
		}
	}
	else if (aType == Textures::Enemy_40){
		bullet = new SuperBullet();
	}
	else{
		bullet = new SimpleBullet();
	}

	
	
	
	bullet->setSpeed(bulletSpeed);

	sf::Vector2f entVelocity = getVelocity() / getSpeed();
	//std::cout << getVelocity().x << ' ' << getVelocity().y << std::endl;
	
	bullet->setVelocity(entVelocity * bulletSpeed);
	if (entVelocity.y < 0){ // Up
		bullet->setPosition(getGlobalBounds().left + getGlobalBounds().width / 2 + 1,
			getGlobalBounds().top - 10);
		bullet->rotate(Entity::Up);
	}
	if (entVelocity.y > 0){ // Down
		bullet->setPosition(getGlobalBounds().left + getGlobalBounds().width / 2 - 1,
			getGlobalBounds().top + getGlobalBounds().height + 10);
		bullet->rotate(Entity::Down);
	}
	if (entVelocity.x < 0){ // Left
		bullet->setPosition(getGlobalBounds().left - 10,
			getGlobalBounds().top + getGlobalBounds().height / 2 - 1);
		bullet->rotate(Entity::Left);
	}
	if (entVelocity.x > 0){ // Right
		bullet->setPosition(getGlobalBounds().left + getGlobalBounds().width + 10,
			getGlobalBounds().top + getGlobalBounds().height / 2 + 1);
		bullet->rotate(Entity::Right);
	}

	/*std::cout << bullet->getSpeed() << std::endl;
	std::cout << getSpeed() << std::endl;*/
	
	return bullet;
}

void Tank::draw(sf::RenderWindow & window)
{
	if (isMoving()){
		static sf::Clock clock;
		sf::Time time = clock.getElapsedTime();
		if (time.asSeconds() < 0.05f){
			window.draw(spr[0]);
		}
		else if (time.asSeconds() < 0.1f)
			window.draw(spr[1]);
		else
			clock.restart();
	}
	else 
		window.draw(spr[0]);
	if (category == Category::EnemyTank){
		numbers[getHP()].setPosition(getGlobalBounds().left + getGlobalBounds().width, getGlobalBounds().top + getGlobalBounds().height);
		window.draw(numbers[getHP()]);
	}
		
}

void Tank::update(const sf::Vector2f vector)
{
	//std::cout << vector.x << ' ' << vector.y << std::endl;
	for (size_t i = 0; i < spritesCount; i++){
		spr[i].move(vector);
	}
}

void Tank::updateBack(sf::Vector2f vector)
{
	//std::cout << "updateBack" << std::endl;
	
	if (vector.y < 0){ // Up
		vector.y = +(-vector.y);
		//std::cout << "Up" << std::endl;
	}
	else if (vector.y > 0){ // Down
		vector.y = -vector.y;
		//std::cout << "Down" << std::endl;
	}
	else if(vector.x < 0){ // Left
		vector.x = +(-vector.x);
		//std::cout << "Left" << std::endl;
	}
	else if(vector.x > 0){ // Right
		vector.x = -vector.x;
		//std::cout << "Right" << std::endl;
	}
	//std::cout << vector.x << ' ' << vector.y << std::endl;
	for (size_t i = 0; i < spritesCount; i++){
		spr[i].move(vector);
	}
}

void Tank::rotate(actions action)
{
	switch (action)
	{
		
	case Tank::Left:
		for (size_t i = 0; i < spritesCount; i++)
			spr[i].setRotation(270.f);
		break;
	case Tank::Right:
		for (size_t i = 0; i < spritesCount; i++)
			spr[i].setRotation(90.f);
		break;
	case Tank::Up:
		for (size_t i = 0; i < spritesCount; i++)
			spr[i].setRotation(0.f);
		break;
	case Tank::Down:
		for (size_t i = 0; i < spritesCount; i++)
			spr[i].setRotation(180.f);
		break;
	case Tank::Fire:
		break;
	}
	
}

void Tank::superClipLoad(size_t count)
{
	for (size_t i = 0; i < count; i++){
		superBulletClip.push(std::unique_ptr<SuperBullet>(new SuperBullet()));
	}
}

sf::FloatRect	Tank::getGlobalBounds()
{
	return spr[0].getGlobalBounds();
}

sf::FloatRect	Tank::getLocalBounds()
{
	return spr[0].getLocalBounds();
}

void Tank::setPosition(float x, float y)
{
	spr[0].setPosition(x, y);
	spr[1].setPosition(x, y);
}

Tank::~Tank()
{
	delete[]spr;
	//std::cout << "Tank deleted!" << std::endl;
}
