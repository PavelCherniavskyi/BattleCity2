#include <boost/program_options.hpp>
#include "../InputHandler.hpp"
#include "../Utils/Utils.hpp"
#include <iostream>

namespace po = boost::program_options;

int InputHandler::Parse(int argc, char **argv)
{
  po::options_description desc("Options");
  desc.add_options()
    ("help", "produce help message")
    ("test", po::value<std::string>(), "test option")
    ("debug,d", "Debug mode with empty map")
    ("level,l", po::value<std::string>(), "load level index")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help"))
  {
    desc.print(std::cout);
    return -1;
  }

  if (vm.count("test"))
  {
    mOptions.emplace(EInputOption::TEST, vm["test"].as<std::string>());
  }
  if (vm.count("debug"))
  {
    mOptions.emplace(EInputOption::DEBUG, "not empty");
  }
  if (vm.count("level"))
  {
    int level = stoi(vm["level"].as<std::string>());
    if(level >= static_cast<int>(ELevels::_size()) || level < 0)
    {
      level = 0;
      SPDLOG_WARN("Level {} is not valid. Set to 0", level);
    }
    ELevels elevel = Utils::Num2Elevels(static_cast<size_t>(level));
    auto parsedToValidLevel = std::to_string(static_cast<size_t>(elevel));
    mOptions.emplace(EInputOption::LEVEL, parsedToValidLevel);
  }

  return 0;
}

std::string InputHandler::GetOption(EInputOption aOption) const
{
  auto it = mOptions.find(aOption);
  if(it == mOptions.end())
  {
    return std::string();
  }
  return it->second;
}