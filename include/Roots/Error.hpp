#ifndef Roots_Error_hpp
#define Roots_Error_hpp

#include "./_defines.hpp"
#include "./bws_result.hpp"
#include <string>

namespace roots::err {

struct Error {
  std::string desc;
  bool fatal;

  Error(std::string desc, bool fatal = false)
      : desc(desc), fatal(fatal) {}

  Error() : desc(""), fatal(false) {}
  Error(const Error &other)
      : desc(other.desc), fatal(other.fatal) {}
  Error(Error &&other)
      : desc(std::move(other.desc)), fatal(other.fatal) {}

  auto operator=(const Error &other) -> Error & {
    desc = other.desc;
    fatal = other.fatal;
    return *this;
  }

  auto operator=(Error &&other) -> Error & {
    desc = std::move(other.desc);
    fatal = other.fatal;
    return *this;
  }

  auto toString() const -> const std::string {
    return desc + (fatal ? " (fatal)" : "");
  }

  auto isFatal() const -> bool { return fatal; }

  friend auto operator<<(std::ostream &os, const Error &err) -> std::ostream & {
    return os << err.toString();
  }
};

} // namespace roots::err

#endif
