#ifndef Roots_String_hpp
#define Roots_String_hpp

#include "./_defines.hpp"
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>

namespace roots::str {

/// @brief Checks if a string ends with a certain substring
auto endsWith(const std::string &str, const std::string &suffix) -> bool;

/// @brief Checks if a string starts with a certain substring
auto startsWith(const std::string &str, const std::string &prefix) -> bool;

/// @brief Splits a string into a vector of strings
auto split(const std::string &str, const std::string &delim)
    -> std::vector<std::string>;

/// @brief Replaces all occurrences of a substring in a string
auto replaceAll(const std::string &str, const std::string &from,
                const std::string &to) -> std::string;

/// @brief Joins a vector of strings into a single string
auto join(const std::vector<std::string> &strs, const std::string &delim)
    -> std::string;

/// @brief Converts a string to lowercase
auto toLower(const std::string &str) -> std::string;

/// @brief Converts a string to uppercase
auto toUpper(const std::string &str) -> std::string;

/// @brief Trims whitespace from the beginning and end of a string
auto trim(const std::string &str) -> std::string;

/// @brief Converts a string to C-style string by copying it
auto duplicateAsCString(const std::string &str) -> const i8 *;

} // namespace roots::str

#endif
