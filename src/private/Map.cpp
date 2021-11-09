#include "../Map.hpp"
#include "../ResourceHolders/SpriteHolder.hpp"

constexpr auto kWidthAndHeightTile = 16.f;
constexpr auto kScale = 2.f;

const std::vector<std::string> kMap0{
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



const std::vector<std::string> kMap1{
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

const std::vector<std::string> kMap2{
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

const std::vector<std::string> kMap3{
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

const std::vector<std::string> kMap4{
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

const std::vector<std::vector<std::string>> kMaps{kMap0, kMap1, kMap2, kMap3, kMap4};

bool setUpInit(EMapObjects mapObject, const sf::Vector2f& aSpritePos, std::unordered_multimap<EMapObjects, Node>& aLevelMap)
{
	auto sprite = SpriteHolder::GetSprite(Utils::MapObject2ImageConverter(mapObject));
	if(!sprite || sprite->empty())
	{
		SPDLOG_ERROR("Sprite is empty");
		return false;
	}

	if(mapObject != +EMapObjects::MAINWALL)
	{
		sprite->at(0).setScale(kScale, kScale);
	}

	sprite->at(0).setPosition(aSpritePos);
	sf::FloatRect pos{aSpritePos.x, aSpritePos.y, kWidthAndHeightTile, kWidthAndHeightTile};
	Node node(pos, sprite->at(0), mapObject);
	aLevelMap.insert(std::make_pair(mapObject, node));

	return true;
}

Map::Map(size_t aMapIndex)
	: mLevelMap()
	, mMapIndex(aMapIndex)
{
}

void Map::Draw(sf::RenderWindow& window) const
{
	for(const auto& pair: mLevelMap)
	{
		window.draw(pair.second.Sprite);
	}
}

void Map::DestroyObject(const std::unordered_multimap<EMapObjects, Node>::const_iterator& it)
{
	mLevelMap.erase(it);
}

const std::unordered_multimap<EMapObjects, Node>& Map::GetMapObjects() const
{
	return mLevelMap;
}

bool Map::Init()
{
	//check correct map size
	for(const auto& map : kMaps)
	{
		if(map.size() != kHeightMap)
		{
			SPDLOG_ERROR("Map height is not correct; actual: {}, expected: {}", map.size(), kHeightMap);
			return false;
		}
		for(const auto& widthMap: map)
		{
			if(widthMap.size() != kWidthMap)
			{
				SPDLOG_ERROR("Map width is not correct; actual: {}, expected: {}", widthMap.size(), kWidthMap);
				return false;
			}
		}
	}

  auto currentMap = kMaps[mMapIndex];

  for (size_t i = 0; i < kHeightMap; i++)
  {
    for (size_t j = 0; j < kWidthMap; j++)
    {
      sf::Vector2f spritePos = { static_cast<float>(j) * kWidthAndHeightTile,
        static_cast<float>(i) * kWidthAndHeightTile };
      if (currentMap[i][j] == '0')
      {
        if (!setUpInit(EMapObjects::MAINWALL, spritePos, mLevelMap))
        {
          return false;
        }
      }
      else if (currentMap[i][j] == '1')
      {
        if (!setUpInit(EMapObjects::WALL, spritePos, mLevelMap))
        {
					return false;
				}
      }
      else if (currentMap[i][j] == '2'){
				if(!setUpInit(EMapObjects::STEELWALL, spritePos, mLevelMap))
				{
					return false;
				}
			}
			else if (currentMap[i][j] == '3'){
				if(!setUpInit(EMapObjects::GREENWALL, spritePos, mLevelMap))
				{
					return false;
				}
			}
			else if (currentMap[i][j] == '4'){
				if(!setUpInit(EMapObjects::WATERWALL, spritePos, mLevelMap))
				{
					return false;
				}
			}
    }
  }

  return true;
}

Map0::Map0() : Map(0u)
{
}

Map1::Map1() : Map(1u)
{
}

Map2::Map2() : Map(2u)
{
}

Map3::Map3() : Map(3u)
{
}

Map4::Map4() : Map(4u)
{
}
