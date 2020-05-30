//
// Created by pavel on 5/31/20.
//

#include <imgui-SFML.h>
#include <imgui.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Window/Event.hpp>
#include <spdlog/spdlog.h>
#include "../Game.hpp"

Game::Game()
{
}

int32 Game::Run()
{
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

