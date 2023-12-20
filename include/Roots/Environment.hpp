#ifndef Roots_Environment_hpp
#define Roots_Environment_hpp

#include "./_defines.hpp"
#include <string>
#include <thread>

namespace roots::env {

/// @brief Gets the value of an environment value
auto get(const std::string &key) -> std::string;

/// @brief Sets the value of an environment value
auto set(const std::string &key, const std::string &value) -> void;

/// @brief Gets the executable path of the current process
auto executablePath() -> std::string;

/// @brief Gets the numbers of processors in the current system
auto processorCount() -> std::size_t;

}

#endif
