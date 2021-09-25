#ifndef BATTLECITY_INPUTHANDLER_HPP
#define BATTLECITY_INPUTHANDLER_HPP

#include "Enums.hpp"

class InputHandler
{
public:
  int Parse(int argc, const char **argv);
  std::string GetOption(EInputOption aOption) const;
private:
  std::map<EInputOption, std::string> mOptions;
};


#endif// BATTLECITY_INPUTHANDLER_HPP