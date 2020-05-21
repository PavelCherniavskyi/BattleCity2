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
        R"(Naval Fate.
   Usage:
          naval_fate ship new <name>...
          naval_fate ship <name> move <x> <y> [--speed=<kn>]
          naval_fate ship shoot <x> <y>
          naval_fate mine (set|remove) <x> <y> [--moored | --drifting]
          naval_fate (-h | --help)
          naval_fate --version
   Options:
          -h --help     Show this screen.
          --version     Show version.
          --speed=<kn>  Speed in knots [default: 10].
          --moored      Moored (anchored) mine.
          --drifting    Drifting mine.
)";

int main([[maybe_unused]]int argc, [[maybe_unused]]const char **argv)
{
//   std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
//                                                              { std::next(argv), std::next(argv, argc) },
//                                                              true,// show help if requested
//                                                              "Naval Fate 2.0");// version string
//
//   for (auto const &arg : args) {
//      std::cout << arg.first << arg.second << std::endl;
//   }


   //Use the default logger (stdout, multi-threaded, colored)
   spdlog::info("Hello, {}!", "World");

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