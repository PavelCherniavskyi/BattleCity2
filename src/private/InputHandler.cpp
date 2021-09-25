#include <boost/program_options.hpp>
#include "../InputHandler.hpp"
#include <iostream>

namespace po = boost::program_options;

int InputHandler::Parse(int argc, const char **argv)
{
  po::options_description desc("Options");
  desc.add_options()
    ("help", "produce help message")
    ("test", po::value<std::string>(), "test option")
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
    mOptions.insert(std::make_pair(EInputOption::TEST, vm["test"].as<std::string>()));
  }

  return 0;
}

std::string InputHandler::GetOption(EInputOption aOption) const
{
  const auto it = mOptions.find(aOption);
  if(it != mOptions.end())
  {
    return it->second;
  }
  
  return std::string();
}