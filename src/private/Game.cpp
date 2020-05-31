//
// Created by pavel on 5/31/20.
//

#include <imgui-SFML.h>
#include <imgui.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Window/Event.hpp>
#include <spdlog/spdlog.h>
#include <map>
#include <docopt/docopt.h>
#include "../Game.hpp"

static constexpr auto OPTIONS_USAGE =
   R"(Battle_City_v2.0
  Usage:
      BattleCity
      BattleCity -t <x>
      BattleCity -h | --help
      BattleCity --version

  Options:
      -h --help     Show this screen.
      -t            Test option.
      --version     Show version.
)";

static constexpr auto SCREEN_WIDTH = 1280U;
static constexpr auto SCREEN_HEIGHT = 760U;
static constexpr auto FRAME_RATE_LIMIT = 60U;
static constexpr auto GAME_TITLE = "Battle_City_v2.0";

Game::Game()
   : mMainWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), GAME_TITLE)
   , mTestOption(0)
{}

int32 Game::Run()
{
   ImGui::SFML::Init(mMainWindow);
   mMainWindow.setFramerateLimit(FRAME_RATE_LIMIT);

   sf::Clock deltaClock;
   while (mMainWindow.isOpen()) {
      sf::Event event{};
      while (mMainWindow.pollEvent(event)) {
         ImGui::SFML::ProcessEvent(event);

         if (event.type == sf::Event::Closed) {
            mMainWindow.close();
         }
      }

      ImGui::SFML::Update(mMainWindow, deltaClock.restart());

      //    ImGui::ShowDemoWindow();

      ImGui::Begin("Hello, world!");
      ImGui::Button("Look at this pretty button");
      ImGui::End();

      mMainWindow.clear();
      //      window.draw(shape);
      ImGui::SFML::Render(mMainWindow);
      mMainWindow.display();
   }

   ImGui::SFML::Shutdown();
   return 0;
}

void Game::handleInput(int argc, const char **argv)
{
   std::map<std::string, docopt::value> args = docopt::docopt(OPTIONS_USAGE, { std::next(argv), std::next(argv, argc) }, true, "v2.0");

   if (args["-t"].asBool()) {
      mTestOption = args["<x>"].asLong();
   } else {
      spdlog::info("mTestOption is empty");
   }

   spdlog::info("Hello, mTestOption: {}", mTestOption);
}
