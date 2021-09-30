#include "../Map.h"


void Map::draw(sf::RenderWindow & window)
{
	for (auto itr = lvl.begin(); itr != lvl.end(); itr++){
		window.draw(*itr->second.sprite);
	}
}

Map::~Map()
{
	for (auto itr = lvl.begin(); itr != lvl.end(); itr++){
		delete itr->second.sprite;
	}
}

char Map0::lvl_0_Map[HEIGHT_MAP][WIDTH_MAP] = {
	"0000000000000000000000000000",
	"0                          0",
	"0                          0",
	"0                          0",
	"0                          0",
	"0                          0",
	"0                          0",
	"0                          0",
	"0                          0",
	"0                          0",
	"0                          0",
	"0                          0",
	"0                          0",
	"0              111         0",
	"0              111         0",
	"022                      220",
	"0       2222               0",
	"0            22            0",
	"0                          0",
	"0                          0",
	"0      1111           211110",
	"0        1111111111        0",
	"0        11      1111      0",
	"0111111            1111    0",
	"01111       2222     11    0",
	"011         2  2           0",
	"0           2  2           0",
	"0000000000000000000000000000"




};


Map0::Map0()
{
	//std::cout << "Map Creation begin!" << std::endl;
	for (size_t i = 0; i < HEIGHT_MAP; i++)
	for (size_t j = 0; j < WIDTH_MAP; j++){
		if (lvl_0_Map[i][j] == '0'){
			sf::Sprite * sprite = new sf::Sprite(resourses.getTexturePtr(Textures::MainWall)[0]);
			sprite->setPosition(static_cast<float>(j * 16), static_cast<float>(i * 16));
			sf::FloatRect pos;
			pos.left = static_cast<float>(j) * 16.f;
			pos.top = static_cast<float>(i) * 16.f;
			pos.height = 16.f;
			pos.width = 16.f;
			Node node(pos, sprite);
			lvl.insert(std::make_pair(Textures::MainWall, node));

		}
		else if (lvl_0_Map[i][j] == '1'){
			sf::Sprite * sprite = new sf::Sprite(resourses.getTexturePtr(Textures::Wall_1)[0]);
			sprite->setPosition(static_cast<float>(j * 16), static_cast<float>(i * 16));
			sprite->setScale(2, 2);
			sf::FloatRect pos;
			pos.left = static_cast<float>(j) * 16.f;
			pos.top = static_cast<float>(i) * 16.f;
			pos.height = 16.f;
			pos.width = 16.f;
			Node node(pos, sprite);
			lvl.insert(std::make_pair(Textures::Wall_1, node));

		}
		else if (lvl_0_Map[i][j] == '2'){
			sf::Sprite * sprite = new sf::Sprite(resourses.getTexturePtr(Textures::Wall_2)[0]);
			sprite->setPosition(static_cast<float>(j * 16), static_cast<float>(i * 16));
			sprite->setScale(2, 2);
			sf::FloatRect pos;
			pos.left = static_cast<float>(j) * 16.f;;
			pos.top = static_cast<float>(i) * 16.f;;
			pos.height = 16.f;
			pos.width = 16.f;
			Node node(pos, sprite);
			lvl.insert(std::make_pair(Textures::Wall_2, node));

		}
		else if (lvl_0_Map[i][j] == '3'){
			sf::Sprite * sprite = new sf::Sprite(resourses.getTexturePtr(Textures::GreenWall)[0]);
			sprite->setPosition(static_cast<float>(j * 16), static_cast<float>(i * 16));
			sprite->setScale(2, 2);
			sf::FloatRect pos;
			pos.left = static_cast<float>(j) * 16.f;;
			pos.top = static_cast<float>(i) * 16.f;;
			pos.height = 16.f;
			pos.width = 16.f;
			Node node(pos, sprite);
			lvl.insert(std::make_pair(Textures::GreenWall, node));

		}
		else if (lvl_0_Map[i][j] == '4'){
			sf::Sprite * sprite = new sf::Sprite(resourses.getTexturePtr(Textures::WaterWall)[0]);
			sprite->setPosition(static_cast<float>(j * 16), static_cast<float>(i * 16));
			sprite->setScale(2, 2);
			sf::FloatRect pos;
			pos.left = static_cast<float>(j) * 16.f;;
			pos.top = static_cast<float>(i) * 16.f;;
			pos.height = 16.f;
			pos.width = 16.f;
			Node node(pos, sprite);
			lvl.insert(std::make_pair(Textures::WaterWall, node));

		}
	}
}


char Map1::lvl_1_Map[HEIGHT_MAP][WIDTH_MAP] = {
	"0000000000000000000000000000",
	"0                          0",
	"0                          0",
	"0  11  11  11  11  11  11  0",
	"0  11  11  11  11  11  11  0",
	"0  11  11  11  11  11  11  0",
	"0  11  11  11  11  11  11  0",
	"0  11  11  112211  11  11  0",
	"0  11  11  112211  11  11  0",
	"0  11  11  11  11  11  11  0",
	"0  11  11          11  11  0",
	"0  11  11          11  11  0",
	"0          11  11          0",
	"0          11  11          0",
	"011  1111          1111  110",
	"022  1111          1111  220",
	"0          11  11          0",
	"0          111111          0",
	"0  11  11  111111  11  11  0",
	"0  11  11  11  11  11  11  0",
	"0  11  11  11  11  11  11  0",
	"0  11  11  11  11  11  11  0",
	"0  11  11          11  11  0",
	"0  11  11          11  11  0",
	"0  11  11   1111   11  11  0",
	"0           1  1           0",
	"0           1  1           0",
	"0000000000000000000000000000"
	
	
	
	
};


Map1::Map1() 
{
	//std::cout << "Map Creation begin!" << std::endl;
	for (size_t i = 0; i < HEIGHT_MAP; i++)
	for (size_t j = 0; j < WIDTH_MAP; j++){
		if (lvl_1_Map[i][j] == '0'){
			sf::Sprite * sprite = new sf::Sprite(resourses.getTexturePtr(Textures::MainWall)[0]);
			sprite->setPosition(static_cast<float>(j * 16), static_cast<float>(i * 16));
			sf::FloatRect pos;
			pos.left = static_cast<float>(j) * 16.f;
			pos.top = static_cast<float>(i) * 16.f;
			pos.height = 16.f;
			pos.width = 16.f;
			Node node(pos, sprite);
			lvl.insert(std::make_pair(Textures::MainWall, node));
			
		}
		else if (lvl_1_Map[i][j] == '1'){
			sf::Sprite * sprite = new sf::Sprite(resourses.getTexturePtr(Textures::Wall_1)[0]);
			sprite->setPosition(static_cast<float>(j * 16), static_cast<float>(i * 16));
			sprite->setScale(2, 2);
			sf::FloatRect pos;
			pos.left = static_cast<float>(j) * 16.f;
			pos.top = static_cast<float>(i) * 16.f;
			pos.height = 16.f;
			pos.width = 16.f;
			Node node(pos, sprite);
			lvl.insert(std::make_pair(Textures::Wall_1, node));

		}
		else if (lvl_1_Map[i][j] == '2'){
			sf::Sprite * sprite = new sf::Sprite(resourses.getTexturePtr(Textures::Wall_2)[0]);
			sprite->setPosition(static_cast<float>(j * 16), static_cast<float>(i * 16));
			sprite->setScale(2, 2);
			sf::FloatRect pos;
			pos.left = static_cast<float>(j) * 16.f;
			pos.top = static_cast<float>(i) * 16.f;
			pos.height = 16.f;
			pos.width = 16.f;
			Node node(pos, sprite);
			lvl.insert(std::make_pair(Textures::Wall_2, node));

		}
	}
	//std::cout << "Map Created" << std::endl;
}


char Map2::lvl_2_Map[HEIGHT_MAP][WIDTH_MAP] = {
	"0000000000000000000000000000",
	"0      22      22          0",
	"0      22      22          0",
	"0  11  22      11  11  11  0",
	"0  11  22      11  11  11  0",
	"0  11        1111  112211  0",
	"0  11        1111  112211  0",
	"0      11          22      0",
	"0      11          22      0",
	"0      11    22    113311220",
	"033    11    22    113311220",
	"03333      11    22  33    0",
	"03333      11    22  33    0",
	"0  11111133333322    3311  0",
	"0  11111133333322    3311  0",
	"0      223311  11  11  11  0",
	"0      223311  11  11  11  0",
	"02211  22  11  11      11  0",
	"02211  22  11  11      11  0",
	"0  11  11  111111  112211  0",
	"0  11  11  111111  112211  0",
	"0  11  11  111111          0",
	"0  11  11  111111          0",
	"0  11              11  11  0",
	"0  11       1111   11  11  0",
	"0  11  11   1  1   111111  0",
	"0  11  11   1  1   111111  0",
	"0000000000000000000000000000"
};


Map2::Map2()
{
	//std::cout << "Map Creation begin!" << std::endl;
	for (size_t i = 0; i < HEIGHT_MAP; i++)
	for (size_t j = 0; j < WIDTH_MAP; j++){
		if (lvl_2_Map[i][j] == '0'){
			sf::Sprite * sprite = new sf::Sprite(resourses.getTexturePtr(Textures::MainWall)[0]);
			sprite->setPosition(static_cast<float>(j * 16), static_cast<float>(i * 16));
			sf::FloatRect pos;
			pos.left = static_cast<float>(j) * 16.f;
			pos.top = static_cast<float>(i) * 16.f;
			pos.height = 16.f;
			pos.width = 16.f;
			Node node(pos, sprite);
			lvl.insert(std::make_pair(Textures::MainWall, node));

		}
		else if (lvl_2_Map[i][j] == '1'){
			sf::Sprite * sprite = new sf::Sprite(resourses.getTexturePtr(Textures::Wall_1)[0]);
			sprite->setPosition(static_cast<float>(j * 16), static_cast<float>(i * 16));
			sprite->setScale(2, 2);
			sf::FloatRect pos;
			pos.left = static_cast<float>(j) * 16.f;
			pos.top = static_cast<float>(i) * 16.f;
			pos.height = 16.f;
			pos.width = 16.f;
			Node node(pos, sprite);
			lvl.insert(std::make_pair(Textures::Wall_1, node));

		}
		else if (lvl_2_Map[i][j] == '2'){
			sf::Sprite * sprite = new sf::Sprite(resourses.getTexturePtr(Textures::Wall_2)[0]);
			sprite->setPosition(static_cast<float>(j * 16), static_cast<float>(i * 16));
			sprite->setScale(2, 2);
			sf::FloatRect pos;
			pos.left = static_cast<float>(j) * 16.f;
			pos.top = static_cast<float>(i) * 16.f;
			pos.height = 16.f;
			pos.width = 16.f;
			Node node(pos, sprite);
			lvl.insert(std::make_pair(Textures::Wall_2, node));

		}
		else if (lvl_2_Map[i][j] == '3'){
			sf::Sprite * sprite = new sf::Sprite(resourses.getTexturePtr(Textures::GreenWall)[0]);
			sprite->setPosition(static_cast<float>(j * 16), static_cast<float>(i * 16));
			sprite->setScale(2, 2);
			sf::FloatRect pos;
			pos.left = static_cast<float>(j) * 16.f;
			pos.top = static_cast<float>(i) * 16.f;
			pos.height = 16.f;
			pos.width = 16.f;
			Node node(pos, sprite);
			lvl.insert(std::make_pair(Textures::GreenWall, node));

		}
	}
	//std::cout << "Map Created" << std::endl;
}

char Map3::lvl_3_Map[HEIGHT_MAP][WIDTH_MAP] = {
	"0000000000000000000000000000",
	"0        11      11        0",
	"0        11      11        0",
	"0  33333311                0",
	"0  33333311          2222220",
	"011333333                  0",
	"011333333                  0",
	"033333333      11  1111111 0",
	"033333333      11  1111111 0",
	"03333333311111111  11   1  0",
	"033333333111111    11   1  0",
	"033333333    11         1  0",
	"033333333    11         1  0",
	"0  33        222222    33  0",
	"0  33        222222    33  0",
	"0                  333333330",
	"0  11  11          333333330",
	"0111  1111  1111111333333330",
	"0111  1111  1      333333330",
	"0          11      333333330",
	"0          11  1111333333330",
	"011    2       1111333333  0",
	"011    2           333333  0",
	"01111  2           333333  0",
	"01111  2    1111   333333  0",
	"0221111     1  1   11      0",
	"0221111     1  1   11      0",
	"0000000000000000000000000000"




};


Map3::Map3()
{
	//std::cout << "Map Creation begin!" << std::endl;
	for (size_t i = 0; i < HEIGHT_MAP; i++)
	for (size_t j = 0; j < WIDTH_MAP; j++){
		if (lvl_3_Map[i][j] == '0'){
			sf::Sprite * sprite = new sf::Sprite(resourses.getTexturePtr(Textures::MainWall)[0]);
			sprite->setPosition(static_cast<float>(j * 16), static_cast<float>(i * 16));
			sf::FloatRect pos;
			pos.left = static_cast<float>(j) * 16.f;
			pos.top = static_cast<float>(i) * 16.f;
			pos.height = 16.f;
			pos.width = 16.f;
			Node node(pos, sprite);
			lvl.insert(std::make_pair(Textures::MainWall, node));

		}
		else if (lvl_3_Map[i][j] == '1'){
			sf::Sprite * sprite = new sf::Sprite(resourses.getTexturePtr(Textures::Wall_1)[0]);
			sprite->setPosition(static_cast<float>(j * 16), static_cast<float>(i * 16));
			sprite->setScale(2, 2);
			sf::FloatRect pos;
			pos.left = static_cast<float>(j) * 16.f;
			pos.top = static_cast<float>(i) * 16.f;
			pos.height = 16.f;
			pos.width = 16.f;
			Node node(pos, sprite);
			lvl.insert(std::make_pair(Textures::Wall_1, node));

		}
		else if (lvl_3_Map[i][j] == '2'){
			sf::Sprite * sprite = new sf::Sprite(resourses.getTexturePtr(Textures::Wall_2)[0]);
			sprite->setPosition(static_cast<float>(j * 16), static_cast<float>(i * 16));
			sprite->setScale(2, 2);
			sf::FloatRect pos;
			pos.left = static_cast<float>(j) * 16.f;
			pos.top = static_cast<float>(i) * 16.f;
			pos.height = 16.f;
			pos.width = 16.f;
			Node node(pos, sprite);
			lvl.insert(std::make_pair(Textures::Wall_2, node));

		}
		else if (lvl_3_Map[i][j] == '3'){
			sf::Sprite * sprite = new sf::Sprite(resourses.getTexturePtr(Textures::GreenWall)[0]);
			sprite->setPosition(static_cast<float>(j * 16), static_cast<float>(i * 16));
			sprite->setScale(2, 2);
			sf::FloatRect pos;
			pos.left = static_cast<float>(j) * 16.f;
			pos.top = static_cast<float>(i) * 16.f;
			pos.height = 16.f;
			pos.width = 16.f;
			Node node(pos, sprite);
			lvl.insert(std::make_pair(Textures::GreenWall, node));

		}
	}
	//std::cout << "Map Created" << std::endl;
}

char Map4::lvl_4_Map[HEIGHT_MAP][WIDTH_MAP] = {
	"0000000000000000000000000000",
	"0        1111              0",
	"0        1111              0",
	"0        11      222222    0",
	"022  11  11          22    0",
	"022  11      11            0",
	"022  11      11            0",
	"011  111111  1111  4444  440",
	"011  111111  1111  4444  440",
	"011      11        44      0",
	"0                  44      0",
	"0        4444  444444  11110",
	"0    11  4444  444444  11110",
	"01111    4411  111         0",
	"01111    4411  111         0",
	"0        44           22   0",
	"0        44           22   0",
	"0444444  44  22  11   2    0",
	"0444444  44  22  11   2    0",
	"0                     211110",
	"0      1111           211110",
	"0        1111111111        0",
	"0        11      1111      0",
	"0111111            1111    0",
	"01111       1111     11    0",
	"011         1  1           0",
	"0           1  1           0",
	"0000000000000000000000000000"




};


Map4::Map4()
{
	//std::cout << "Map Creation begin!" << std::endl;
	for (size_t i = 0; i < HEIGHT_MAP; i++)
	for (size_t j = 0; j < WIDTH_MAP; j++){
		if (lvl_4_Map[i][j] == '0'){
			sf::Sprite * sprite = new sf::Sprite(resourses.getTexturePtr(Textures::MainWall)[0]);
			sprite->setPosition(static_cast<float>(j * 16), static_cast<float>(i * 16));
			sf::FloatRect pos;
			pos.left = static_cast<float>(j) * 16.f;
			pos.top = static_cast<float>(i) * 16.f;
			pos.height = 16.f;
			pos.width = 16.f;
			Node node(pos, sprite);
			lvl.insert(std::make_pair(Textures::MainWall, node));

		}
		else if (lvl_4_Map[i][j] == '1'){
			sf::Sprite * sprite = new sf::Sprite(resourses.getTexturePtr(Textures::Wall_1)[0]);
			sprite->setPosition(static_cast<float>(j * 16), static_cast<float>(i * 16));
			sprite->setScale(2, 2);
			sf::FloatRect pos;
			pos.left = static_cast<float>(j) * 16.f;
			pos.top = static_cast<float>(i) * 16.f;
			pos.height = 16.f;
			pos.width = 16.f;
			Node node(pos, sprite);
			lvl.insert(std::make_pair(Textures::Wall_1, node));

		}
		else if (lvl_4_Map[i][j] == '2'){
			sf::Sprite * sprite = new sf::Sprite(resourses.getTexturePtr(Textures::Wall_2)[0]);
			sprite->setPosition(static_cast<float>(j * 16), static_cast<float>(i * 16));
			sprite->setScale(2, 2);
			sf::FloatRect pos;
			pos.left = static_cast<float>(j) * 16.f;
			pos.top = static_cast<float>(i) * 16.f;
			pos.height = 16.f;
			pos.width = 16.f;
			Node node(pos, sprite);
			lvl.insert(std::make_pair(Textures::Wall_2, node));

		}
		else if (lvl_4_Map[i][j] == '3'){
			sf::Sprite * sprite = new sf::Sprite(resourses.getTexturePtr(Textures::GreenWall)[0]);
			sprite->setPosition(static_cast<float>(j * 16), static_cast<float>(i * 16));
			sprite->setScale(2, 2);
			sf::FloatRect pos;
			pos.left = static_cast<float>(j) * 16.f;
			pos.top = static_cast<float>(i) * 16.f;
			pos.height = 16.f;
			pos.width = 16.f;
			Node node(pos, sprite);
			lvl.insert(std::make_pair(Textures::GreenWall, node));

		}
		else if (lvl_4_Map[i][j] == '4'){
			sf::Sprite * sprite = new sf::Sprite(resourses.getTexturePtr(Textures::WaterWall)[0]);
			sprite->setPosition(static_cast<float>(j * 16), static_cast<float>(i * 16));
			sprite->setScale(2, 2);
			sf::FloatRect pos;
			pos.left = static_cast<float>(j) * 16.f;
			pos.top = static_cast<float>(i) * 16.f;
			pos.height = 16.f;
			pos.width = 16.f;
			Node node(pos, sprite);
			lvl.insert(std::make_pair(Textures::WaterWall, node));

		}
	}
	//std::cout << "Map Created" << std::endl;
}
