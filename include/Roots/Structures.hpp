#ifndef Roots_Structures_hpp
#define Roots_Structures_hpp

#include "./_defines.hpp"
#include "Concepts.hpp"
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace roots::structures {

using namespace roots::concepts;

/// @brief A "bitflags"-like type that can be used with enums
// (this type is not compatible with enum class)
template <BitFlagConvertible T> class BitFlags {
  u64 flags;

public:
  BitFlags() : flags(0) {}
  BitFlags(u64 flags) : flags(flags) {}
  BitFlags(const BitFlags &other) : flags(other.flags) {}

  auto operator=(const BitFlags &other) -> BitFlags & {
    flags = other.flags;
    return *this;
  }

  auto operator==(const BitFlags &other) const -> bool {
    return flags == other.flags;
  }

  auto operator!=(const BitFlags &other) const -> bool {
    return flags != other.flags;
  }

  auto operator[](T flag) const -> bool {
    return flags & (1 << static_cast<u64>(flag));
  }

  // Allows for `flags[Flag::Foo] = true;`
  auto operator[](T flag) -> bool & {
    return *reinterpret_cast<bool *>(reinterpret_cast<u64>(&flags) +
                                     static_cast<u64>(flag));
  }

  auto operator~() const -> BitFlags { return BitFlags(~flags); }

  auto set(T flag, bool value = true) -> void {
    if (value)
      flags |= (1 << static_cast<u64>(flag));
    else
      flags &= ~(1 << static_cast<u64>(flag));
  }

  auto get(T flag) const -> bool {
    return flags & (1 << static_cast<u64>(flag));
  }

  auto clear() -> void { flags = 0; }

  auto operator|(T flag) const -> BitFlags { return BitFlags(flags | flag); }

  auto operator&(T flag) const -> BitFlags { return BitFlags(flags & flag); }

  auto operator|=(T flag) -> BitFlags & {
    flags |= flag;
    return *this;
  }

  auto operator&=(T flag) -> BitFlags & {
    flags &= flag;
    return *this;
  }
};

/// @brief An alternative to std::views::values
template <typename K, typename V>
auto values(const std::unordered_map<K, V> &map) -> std::vector<V> {
  std::vector<V> values;
  values.reserve(map.size());
  for (const auto &[_, value] : map)
    values.push_back(value);
  return values;
}

/// @brief An alternative to std::views::keys
template <typename K, typename V>
auto keys(const std::unordered_map<K, V> &map) -> std::vector<K> {
  std::vector<K> keys;
  keys.reserve(map.size());
  for (const auto &[key, _] : map)
    keys.push_back(key);
  return keys;
}

/// @brief A "tagged union"-like type that can be used to create dynamic tagged
/// unions
template <typename... Ts> class TaggedUnion {
  u64 tag;
  union {
    std::aligned_union_t<0, Ts...> data;
    std::byte dummy;
  };

public:
  TaggedUnion() : tag(0), dummy{} {}

  template <typename T> TaggedUnion(T &&value) : tag(1), dummy{} {
    static_assert((std::is_same_v<T, Ts> || ...));
    new (&data) std::decay_t<T>(std::forward<T>(value));
  }

  TaggedUnion(const TaggedUnion &other) : tag(other.tag), dummy{} {
    if (tag != 0)
      new (&data) std::decay_t<decltype(other.data)>(other.data);
  }
  
  TaggedUnion(TaggedUnion &&other) : tag(other.tag), dummy{} {
    if (tag != 0)
      new (&data) std::decay_t<decltype(other.data)>(std::move(other.data));
  }

  ~TaggedUnion() {
    if (tag != 0)
      reinterpret_cast<std::decay_t<decltype(data)> *>(&data)->~decltype(data)();
  }

  auto operator=(const TaggedUnion &other) -> TaggedUnion & {
    if (tag != 0)
      reinterpret_cast<std::decay_t<decltype(data)> *>(&data)->~decltype(data)();
    tag = other.tag;
    if (tag != 0)
      new (&data) std::decay_t<decltype(other.data)>(other.data);
    return *this;
  }

  auto operator=(TaggedUnion &&other) -> TaggedUnion & {
    if (tag != 0)
      reinterpret_cast<std::decay_t<decltype(data)> *>(&data)->~decltype(data)();
    tag = other.tag;
    if (tag != 0)
      new (&data) std::decay_t<decltype(other.data)>(std::move(other.data));
    return *this;
  }

  template<typename T>
  auto get() -> T& {
    static_assert((std::is_same_v<T, Ts> || ...));
    if (tag != 0)
      return *reinterpret_cast<T*>(&data);
    else
      throw std::runtime_error("TaggedUnion is empty");
  }

  template<typename T>
  auto get() const -> const T& {
    static_assert((std::is_same_v<T, Ts> || ...));
    if (tag != 0)
      return *reinterpret_cast<const T*>(&data);
    else
      throw std::runtime_error("TaggedUnion is empty");
  }

  template<typename T>
  auto is() const -> bool {
    static_assert((std::is_same_v<T, Ts> || ...));
    return tag == 0;
  }

  template<typename T>
  auto set(T&& value) -> void {
    static_assert((std::is_same_v<T, Ts> || ...));
    if (tag != 0)
      reinterpret_cast<std::decay_t<decltype(data)> *>(&data)->~decltype(data)();
    tag = 1;
    new (&data) std::decay_t<T>(std::forward<T>(value));
  }

  auto clear() -> void {
    if (tag != 0)
      reinterpret_cast<std::decay_t<decltype(data)> *>(&data)->~decltype(data)();
    tag = 0;
  }

  auto operator==(const TaggedUnion& other) const -> bool {
    if (tag != other.tag)
      return false;
    if (tag == 0)
      return true;
    return data == other.data;
  }

  auto operator!=(const TaggedUnion& other) const -> bool {
    return !(*this == other);
  }
};

/// @brief A monostate / nil state type for TaggedUnion
struct NilTag_t {};
const NilTag_t NilTag = {};

} // namespace roots::structures

#endif
