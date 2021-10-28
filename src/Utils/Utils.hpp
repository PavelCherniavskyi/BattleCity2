#ifndef BATTLECITY_UTILS_UTILS_HPP
#define BATTLECITY_UTILS_UTILS_HPP

#include "../Enums.hpp"
#include <type_traits>

namespace Utils{


bool IsMovingAction(EActions action);

template <typename T, typename _ = void>
struct is_vector { 
    static const bool value = false;
};

template <typename T>
struct is_vector< T, typename std::enable_if_t<std::is_same_v<T, std::vector<typename T::value_type>>>>
{
    static const bool value = true;
};


}
#endif// BATTLECITY_UTILS_UTILS_HPP