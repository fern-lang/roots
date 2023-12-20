#ifndef Roots_Filesystem_hpp
#define Roots_Filesystem_hpp

#include "./_defines.hpp"
#include "./bws_result.hpp"
#include "Error.hpp"
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace roots::fs {

using namespace roots::err;

using path_type = std::filesystem::path;

/// @brief Return the current working directory
auto cwd() -> path_type;

/// @brief Returns the home directory of the current user
auto home() -> path_type;

/// @brief Reads a file into a string (blocking)
auto readFile(const path_type &path) -> roots::result<std::string, Error>;

/// @brief Reads a file into a byte vector (blocking)
auto readFileBytes(const path_type &path)
    -> roots::result<std::vector<u8>, Error>;

/// @brief Check if a file exists
auto exists(const path_type &path) -> bool;

/// @brief Return the relative path of a file in a directory (or cwd if not
/// specified)
auto relativePath(const path_type &path, const path_type &dir = cwd())
    -> roots::result<path_type, Error>;

/// @brief Returns the absolute path of a file (error if it doesn't exist)
auto absolutePath(const path_type &path) -> roots::result<path_type, Error>;

/// @brief Returns true if the path is absolute
auto isAbsolute(const path_type &path) -> bool;

/// @brief Returns true if the path is relative
auto isRelative(const path_type &path) -> bool;

/// @brief Returns true if the path is a directory
auto isDir(const path_type &path) -> bool;

/// @brief Returns true if the path is a file
auto isFile(const path_type &path) -> bool;

/// @brief Searches for a file in a dir using a matcher function
auto search(const path_type &dir,
            const std::function<bool(const path_type &)> &matcher)
    -> roots::result<path_type, Error>;

/// @brief Writes a byte vector to a file
auto writeBytes(const path_type &path, const std::vector<u8> &bytes)
    -> roots::result<void, Error>;

} // namespace roots::fs

#endif
