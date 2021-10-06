#include <gtest/gtest.h>
#include <config.h>
#include <SFML/Graphics/Texture.hpp>
#include "Enums.hpp"
#include "ResourceHolders/ResourceHolder.hpp"
#include "InputHandler.hpp"
#include "Definitions.hpp"

TEST(ResourceHolder, ResourceHolderFunctionalityTest)
{
  const std::string path = fmt::format("{}/media/Textures/SpriteSheet.png", CMAKE_SOURCE_DIR);
  ResourceHolder<EImage, sf::Texture> singleResourceHolder;
  singleResourceHolder.LoadFromFile(EImage::MAIN_SHEET, path);

  auto main_texture = singleResourceHolder.Get(EImage::MAIN_SHEET);
  ASSERT_TRUE(main_texture);
  EXPECT_TRUE(main_texture->getSize() == sf::Vector2u(kTextureWidth, kTextureHeight));

  ResourceHolder<EImage, std::vector<sf::Texture>> vectorResourceHolder;
  std::vector<sf::IntRect> playerTankRec = { { 1, 2, 13, 13 }, { 17, 2, 13, 13 } };
  vectorResourceHolder.LoadFromFile(EImage::T_10, path, playerTankRec);
  auto tank_texture = vectorResourceHolder.Get(EImage::T_10);
  ASSERT_TRUE(tank_texture);
  ASSERT_EQ(tank_texture->size(), 2u);
  EXPECT_EQ(tank_texture->at(0).getSize(),
    sf::Vector2u(static_cast<uint32_t>(playerTankRec[0].width), static_cast<uint32_t>(playerTankRec[0].height)));
  EXPECT_EQ(tank_texture->at(1).getSize(),
    sf::Vector2u(static_cast<uint32_t>(playerTankRec[1].width), static_cast<uint32_t>(playerTankRec[1].height)));
}

// TODO: write tests for all resources holders

TEST(InputHandler, InputHandlerTest)
{
  const int size = 3;
  std::array<std::string, size> args{ "./BattleCity2", "--test", "testStr" };
  char** argv = new char*[args.size()];
  for (std::size_t i = 0u; i < args.size(); i++)
  {
    argv[i] = new char[args[i].size() + 1];
    strncpy(argv[i], args[i].c_str(), args[i].size());
  }
  InputHandler input;
  input.Parse(size, argv);

  ASSERT_EQ(args.size(), size);
  EXPECT_EQ(args[2], input.GetOption(EInputOption::TEST));

  for (std::size_t i = 0u; i < args.size(); i++)
  {
    delete[] argv[i];
  }
  delete[] argv;
}