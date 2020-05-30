//
// Created by pavel on 5/31/20.
//

#ifndef BATTLECITY_OPTIONSHANDLER_HPP
#define BATTLECITY_OPTIONSHANDLER_HPP

#include <spdlog/spdlog.h>
#include <docopt/docopt.h>

static constexpr auto USAGE =
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

void handleInput(int argc, const char **argv)
{
   std::map<std::string, docopt::value> args = docopt::docopt(USAGE,{ std::next(argv), std::next(argv, argc) },true,"v2.0");

   long test;
   if(args["-t"].asBool())
   {
      test = args["<x>"].asLong();
   }


   //Use the default logger (stdout, multi-threaded, colored)
   spdlog::info("Hello, {}! Text: {}", "World", test);

   fmt::print("Hello, from {}\n", "{fmt}");
}

#endif//BATTLECITY_OPTIONSHANDLER_HPP
