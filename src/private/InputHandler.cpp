#include <boost/program_options.hpp>
#include "../InputHandler.hpp"
#include <iostream>

namespace po = boost::program_options;

int InputHandler::Parse(int argc, char **argv)
{
  po::options_description desc("Options");
  desc.add_options()
    ("help", "produce help message")
    ("test", po::value<std::string>(), "test option")
    ("debug,d", "Debug mode with empty map")
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
    SPDLOG_INFO("Debug is here");
  }

  return 0;
}

std::string InputHandler::GetOption(EInputOption aOption) const
{
  auto it = mOptions.find(aOption);
  if(it == mOptions.end())
  {
    SPDLOG_WARN("option {} is not found", aOption._to_string());
    return std::string();
  }
  return it->second;
}