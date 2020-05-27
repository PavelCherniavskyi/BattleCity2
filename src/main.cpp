#include <functional>
#include <iostream>
#include <spdlog/spdlog.h>
#include <docopt/docopt.h>

#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>

static constexpr auto USAGE =
  R"(Battle_City_v2.0
   Usage:
      BattleCity [options]
   Options:
      -h --help     Show this screen.
      --version     Show version.
      -t --test     Test option.
)";

int main([[maybe_unused]] int argc, [[maybe_unused]] const char **argv)
{
   std::map<std::string, docopt::value> args = docopt::docopt(USAGE,{ std::next(argv), std::next(argv, argc) },true,"v2.0");

   for (auto const &arg : args) {
      std::cout << arg.first << arg.second << std::endl;
   }

   long test = args["--test"].asLong();

   //Use the default logger (stdout, multi-threaded, colored)
   spdlog::info("Hello, {}! Text: {}", "World", test);

   fmt::print("Hello, from {}\n", "{fmt}");

   sf::RenderWindow window(sf::VideoMode(640, 480), "ImGui + SFML = <3");
   window.setFramerateLimit(60);
   ImGui::SFML::Init(window);

   sf::CircleShape shape(100.f);
   shape.setFillColor(sf::Color::Green);

   sf::Clock deltaClock;
   while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
         ImGui::SFML::ProcessEvent(event);

         if (event.type == sf::Event::Closed) {
            window.close();
         }
      }

      ImGui::SFML::Update(window, deltaClock.restart());

      //    ImGui::ShowDemoWindow();

      ImGui::Begin("Hello, world!");
      ImGui::Button("Look at this pretty button");
      ImGui::End();

      window.clear();
      window.draw(shape);
      ImGui::SFML::Render(window);
      window.display();
   }

   ImGui::SFML::Shutdown();

   return 0;
}