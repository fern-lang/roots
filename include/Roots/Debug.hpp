#ifndef Roots_Debug_hpp
#define Roots_Debug_hpp

#include "./_defines.hpp"
#include "Filesystem.hpp"
#include <iostream>
#include <ostream>

class __nullstream : public std::ostream {
  __nullstream() : std::ostream(nullptr) {}
  ~__nullstream() override = default;

public:
  static auto get() -> std::ostream & {
    static __nullstream instance;
    return instance;
  }
};

#define RootsDebugLog                                                          \
  (RootsDebug ? std::cerr : __nullstream::get()) << "["                                                             \
            << roots::fs::relativePath(__FILE__).value_or(                     \
                   std::filesystem::path("../invalid/path"))                   \
            << ":" << __LINE__ << "@" << __funcname__ << "] "

#endif
