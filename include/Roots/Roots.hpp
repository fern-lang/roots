#ifndef Roots_hpp
#define Roots_hpp

#include "Roots/Environment.hpp"
#include "Roots/Error.hpp"
#include "Roots/Filesystem.hpp"
#include "Roots/Memory.hpp"
#include "Roots/String.hpp"
#include "Roots/Structures.hpp"
#include "Roots/Concepts.hpp"

// operator<< for roots::result
template <roots::concepts::Printable T, roots::concepts::Printable E>
auto operator<<(std::ostream &os, const roots::result<T, E> &r) -> std::ostream & {
  if (r.has_value()) {
    os << "Ok(" << r.value() << ")";
  } else {
    os << "Err(" << r.error() << ")";
  }
  return os;
}

#endif
