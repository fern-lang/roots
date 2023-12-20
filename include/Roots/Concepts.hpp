#ifndef Roots_Concepts_hpp
#define Roots_Concepts_hpp

#include "./_defines.hpp"
#include <concepts>
#include <ostream>

namespace roots::concepts {

template <typename T>
concept BitFlagConvertibleEnum =
    std::is_enum<T>::value && std::is_convertible<T, u64>::value;

template <typename T>
concept BitFlagConvertible =
    (std::is_enum<T>::value && std::is_convertible_v<T, u64>) || requires(T t) {
      { static_cast<u64>(t) }; // enum class with underlying type u64
    };

template <typename T>
concept Printable = requires(std::ostream &os, T t) {
  { os << t };
};

} // namespace roots::concepts

#endif
