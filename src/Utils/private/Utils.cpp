#include "../Utils.hpp"

namespace Utils{

bool IsMovingAction(EActions action)
{
  switch (action)
  {
  case +EActions::LEFT:
  case +EActions::RIGHT:
  case +EActions::UP:
  case +EActions::DOWN: 
    return true;

  default:
    return false;
  }
}

}