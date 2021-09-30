#pragma once

#include "Entity.h"


struct Node{
	Node(sf::FloatRect r, sf::Sprite* s) : rect(r), sprite(s){}
	sf::FloatRect rect;
	sf::Sprite*	sprite;
};

class Map : public Resourses
{
public:

														Map() :resourses(Resourses::getRes()){}
	virtual												~Map();
	virtual void										draw(sf::RenderWindow & window);
	virtual std::multimap<Textures::ID, Node>&			getMap() { return lvl; }
	
protected:
	std::multimap<Textures::ID, Node>					lvl;
	ResourceHolder&										resourses;


};

class Map0 : public Map
{
public:
	Map0();
private:
	static char											lvl_0_Map[HEIGHT_MAP][WIDTH_MAP];
};

class Map1 : public Map
{
public:
														Map1();
private:
	static char											lvl_1_Map[HEIGHT_MAP][WIDTH_MAP];
};

class Map2 : public Map
{
public:
														Map2();
private:
	static char											lvl_2_Map[HEIGHT_MAP][WIDTH_MAP];
};

class Map3 : public Map
{
public:
														Map3();
private:
	static char											lvl_3_Map[HEIGHT_MAP][WIDTH_MAP];
};

class Map4 : public Map
{
public:
	Map4();
private:
	static char											lvl_4_Map[HEIGHT_MAP][WIDTH_MAP];
};