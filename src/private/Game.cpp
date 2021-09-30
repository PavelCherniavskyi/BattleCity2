#include "../Game.h"
#include "../StringHelpers.h"
#include "config.h"

Game::Game(): mWindow(sf::VideoMode(WIDTH_SCREEN + WIDTH_RIGHT_PANEL * 2, HEIGHT_SCREEN), "Battle City", sf::Style::Close)
, TimePerFrame(sf::seconds(1.f / 60.f))
, mFont()
, mStatisticsUpdateTime()
, mStatisticsNumFrames(0)
, player(entities, animations, gameStage, enemyTanks, mapSequence, panel, bonuses)

, gameStage(Player::Gamestates::runing)
, enemyTanksQuantity(20)
, panel(static_cast<std::size_t>(player.getPlayerTank()->getHP()), enemyTanksQuantity, 1u, 0)
{
}

void Game::textInitialization()
{
    mFont.loadFromFile(fmt::format("{}/media/Sansation.ttf", CMAKE_SOURCE_DIR));

	sf::Text mStatisticsText;
	mStatisticsText.setFont(mFont);
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10);
	texts.insert(std::make_pair(TextHolder::mStatistics, mStatisticsText));

	sf::Text gameOver("Game Over!!!", mFont, 60);
	//gameOver.setColor(sf::Color::Red);
	gameOver.setStyle(sf::Text::Bold);
	gameOver.setPosition(60.f, 190.f);
	texts.insert(std::make_pair(TextHolder::gameover, gameOver));

	sf::Text nextLvl("Next Level", mFont, 60);
	//gameOver.setColor(sf::Color::Red);
	nextLvl.setStyle(sf::Text::Bold);
	nextLvl.setPosition(105.f, 190.f);
	texts.insert(std::make_pair(TextHolder::nextlvl, nextLvl));

	sf::Text win("You win!!!", mFont, 60);
	//gameOver.setColor(sf::Color::Red);
	win.setStyle(sf::Text::Bold);
	win.setPosition(120.f, 190.f);
	texts.insert(std::make_pair(TextHolder::win, win));

	sf::Text pause("Pause", mFont, 60);
	//gameOver.setColor(sf::Color::Red);
	pause.setStyle(sf::Text::Bold);
	pause.setPosition(130.f, 190.f);
	texts.insert(std::make_pair(TextHolder::pause, pause));
}

void Game::run()
{
	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			//std::cout << "Check1" << std::endl;
			timeSinceLastUpdate -= TimePerFrame;
			if (gameStage != Player::Gamestates::gameOver && gameStage != Player::Gamestates::win){
				handleInput(TimePerFrame);
				if (isPaused)
					continue;
				update(TimePerFrame);
			}
			
		}
		//std::cout << "Check2" << std::endl;
		TimePerFrame.asSeconds();
		updateStatistics(elapsedTime);
		render();
	}
}

void Game::Init()
{
	Resourses::Init();
	textInitialization();
	tankLoad(1);
	isPaused = false;
	mWindow.setKeyRepeatEnabled(false);
	player.Init();
	panel.setCurrentMissles(player.getPlayerTank()->superClipSize());
	panel.Init();
}

void Game::handleInput(sf::Time aTimePerFrame)
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			mWindow.close();
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P){
			isPaused = !isPaused;
			break;
		}
		
		if(!isPaused)
			player.handleEvent(event, aTimePerFrame);
	}
	player.handleRealtimeInput(aTimePerFrame);
	
	//std::cout << "player.handleRealtimeInput" << std::endl;
}

void Game::update(sf::Time elapsedTime)
{
	Player::rangePtr  retBullet;
	Player::rangePtr  retSuperBullet;
	Player::rangePtr retEnemyTank;


	/*sf::Vector2i pixelPos = sf::Mouse::getPosition(mWindow);
	std::cout << pixelPos.x << ' ' << pixelPos.y << std::endl;*/



	//std::cout << "update0" << std::endl;
	retBullet = entities.equal_range(Category::Bullet);
	//Update for bullets
	for (auto itrBullet = retBullet.first; itrBullet != retBullet.second; ++itrBullet){
		itrBullet->second->update(itrBullet->second->getVelocity() * elapsedTime.asSeconds());
		if (player.isIntersectsBullet()){
			break;
		}
	}
	//std::cout << "update1" << std::endl;
	retSuperBullet = entities.equal_range(Category::SuperBullet);
	//Update for SuperBullets
	for (auto itrSuperBullet = retSuperBullet.first; itrSuperBullet != retSuperBullet.second; ++itrSuperBullet){
		itrSuperBullet->second->update(itrSuperBullet->second->getVelocity() * elapsedTime.asSeconds());
		if (player.isIntersectsSuperBullet()){
			break;
		}
	}
	//std::cout << "update2" << std::endl;
	//Update for Enemy tanks
	retEnemyTank = entities.equal_range(Category::EnemyTank);
	for (auto itrTank = retEnemyTank.first; itrTank != retEnemyTank.second; itrTank++){
		itrTank->second->update(itrTank->second->getVelocity() * elapsedTime.asSeconds());
		//std::cout << "Check222" << std::endl;
		if (player.isIntersectsEnemy()){
			itrTank->second->updateBack(itrTank->second->getVelocity() * elapsedTime.asSeconds());
			player.handleEnemyTanks(itrTank->second);
		}
		player.handleEnemyFire(TimePerFrame, itrTank->second);
	}
	//std::cout << "update3" << std::endl;

	

	//std::cout << "update4" << std::endl;
	
	//Update for Animation
	for (auto itrAnim = animations.begin(); itrAnim != animations.end(); itrAnim++){
		if (itrAnim->second->isAlife()){
			itrAnim->second->update();
		}
		else{
			auto temp = itrAnim;
			itrAnim->second->~Animation();
			animations.erase(temp);
			itrAnim = animations.begin();
			break;
		}
	}

	//std::cout << "update5" << std::endl;
	
	
	player.handleEnemySpawn(TimePerFrame);

	//std::cout << "update6" << std::endl;
	player.handleBonusEvents(TimePerFrame);

	//std::cout << "update7" << std::endl;
	player.isIntersectsOthers();
	
	//std::cout << "update8" << std::endl;
}

void Game::stageRender()
{
	//std::cout << "===========" << std::endl;

	// Check for game states
	static bool forClockRestart = true;
	static sf::Clock clock;
	if (forClockRestart)
		clock.restart();
	auto checkIfEmpty = entities.find(Category::EnemyTank);
	auto checkIfPlayerAlife = entities.find(Category::PlayerTank);
	if (checkIfEmpty == entities.end() && mapSequence.size() > 1
		&& checkIfPlayerAlife != entities.end() && enemyTanks.empty()){
		gameStage = Player::Gamestates::nextLvl;
		forClockRestart = false;
		//std::cout << mapSequence.size() << std::endl;
		//std::cout << "nextLvl" << std::endl;
	}
	else if (checkIfEmpty == entities.end() && mapSequence.size() == 1
		&& checkIfPlayerAlife != entities.end() && enemyTanks.empty()){
		gameStage = Player::Gamestates::win;
		forClockRestart = false;
		//std::cout << "win" << std::endl;
	}

	if (gameStage == Player::Gamestates::runing){
		mWindow.draw(texts[TextHolder::mStatistics]);
	}

	if (isPaused) {
		mWindow.draw(texts[TextHolder::pause]);
	}

	else if (gameStage == Player::Gamestates::gameOver){
		mWindow.draw(texts[TextHolder::gameover]);
		static sf::Clock forGameOver;
		sf::Time time = forGameOver.getElapsedTime();
		if (time.asSeconds() > 2){
			mWindow.close();
		}
	}
	else if (gameStage == Player::Gamestates::nextLvl){
		//std::cout << "nextLvl2" << std::endl;
		mWindow.draw(texts[TextHolder::nextlvl]);
		sf::Time time = clock.getElapsedTime();
		if (time.asSeconds() > 3){
			nextLvlInitialize();
			forClockRestart = true;
		}
	}
	else if (gameStage == Player::Gamestates::win){
		mWindow.draw(texts[TextHolder::win]);
		sf::Time time = clock.getElapsedTime();
		if (time.asSeconds() > 3){
			mWindow.close();
		}
	}

	

}

void	Game::nextLvlInitialize()
{
	mapSequence.front()->~Map();
	mapSequence.pop();
	panel.resetIcons();

	panel.setCurrentLvl(1);
	//std::cout << panel.getCurrentLvl() << std::endl;
	tankLoad(panel.getCurrentLvl());
	auto TankPos = entities.find(Category::PlayerTank);
	Tank * tank = dynamic_cast<Tank*> (TankPos->second);
	tank->setInitialPosition();
	//TankPos->second->rotate(Entity::actions::Up);
	gameStage = Player::Gamestates::runing;
	
	
}

void Game::tankLoad(size_t levl)
{
	switch (levl)
	{
	case 1:
		for (int i = 0; i < 18; i++){
			enemyTanks.push(new Tank(Category::EnemyTank, Textures::Enemy_10));
		}
		for (int i = 0; i < 2; i++){
			enemyTanks.push(new Tank(Category::EnemyTank, Textures::Enemy_20));
		}
		break;
	case 2:
		for (int i = 0; i < 14; i++){
			enemyTanks.push(new Tank(Category::EnemyTank, Textures::Enemy_10));
		}
		for (int i = 0; i < 4; i++){
			enemyTanks.push(new Tank(Category::EnemyTank, Textures::Enemy_20));
		}

		for (int i = 0; i < 2; i++){
			enemyTanks.push(new Tank(Category::EnemyTank, Textures::Enemy_30));
		}
		break;
	case 3:
		for (int i = 0; i < 10; i++){
			enemyTanks.push(new Tank(Category::EnemyTank, Textures::Enemy_10));
		}
		for (int i = 0; i < 6; i++){
			enemyTanks.push(new Tank(Category::EnemyTank, Textures::Enemy_20));
		}

		for (int i = 0; i < 4; i++){
			enemyTanks.push(new Tank(Category::EnemyTank, Textures::Enemy_30));
		}
		break;
	case 4:
		for (int i = 0; i < 2; i++){
			enemyTanks.push(new Tank(Category::EnemyTank, Textures::Enemy_10));
		}
		for (int i = 0; i < 8; i++){
			enemyTanks.push(new Tank(Category::EnemyTank, Textures::Enemy_20));
		}

		for (int i = 0; i < 6; i++){
			enemyTanks.push(new Tank(Category::EnemyTank, Textures::Enemy_30));
		}
		for (int i = 0; i < 4; i++){
			enemyTanks.push(new Tank(Category::EnemyTank, Textures::Enemy_40));
		}
		break;
	default:
		for (int i = 0; i < 20; i++){
			enemyTanks.push(new Tank(Category::EnemyTank, Textures::Enemy_40));
		}
		break;
	}

	player.handleEnemySpawn(TimePerFrame); // initial start for check game states wouldn't get nextLvl because of empty entity field
	
}

void Game::render()
{
	mWindow.clear();
	draw();
	stageRender();
	
	
	mWindow.display();
}

void Game::updateStatistics(sf::Time elapsedTime)
{
	mStatisticsUpdateTime += elapsedTime;
	mStatisticsNumFrames += 1;

	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		texts[TextHolder::mStatistics].setString(
			"Frames / Second = " + toString(mStatisticsNumFrames) + "\n" +
			"Time / Update = " + toString(mStatisticsUpdateTime.asMicroseconds() / static_cast<sf::Int64>(mStatisticsNumFrames)) + "us");

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}

void Game::draw()
{
	

	//Draw for All entities
	for (auto itr = entities.begin(); itr != entities.end(); itr++)
		itr->second->draw(mWindow);

	//Draw for map
	mapSequence.front()->draw(mWindow);

	//Draw for Bonuses
	for (auto itrBonus = bonuses.begin(); itrBonus != bonuses.end(); itrBonus++){
		itrBonus->second->draw(mWindow);
	}

	//Draw for Animation
	for (auto itrAnim = animations.begin(); itrAnim != animations.end(); itrAnim++){
		itrAnim->second->draw(mWindow);
	}

	//Draw for Right Panel
	panel.draw(mWindow);
	
}

Game::~Game()
{
	for (auto itr = entities.begin(); itr != entities.end(); itr++)
		itr->second->~Entity();
}
