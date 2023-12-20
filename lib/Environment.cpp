#include "Roots/Environment.hpp"

#ifdef ROOTS_PLATFORM_WINDOWS
#include <Windows.h>
#elif defined(ROOTS_PLATFORM_LINUX)
#include <unistd.h>
#include <stdlib.h>
#elif defined(ROOTS_PLATFORM_APPLE)
#include <mach-o/dyld.h>
#endif

namespace roots::env {

auto get(const std::string &key) -> std::string {
#ifdef ROOTS_PLATFORM_WINDOWS
  char buffer[4096];
  GetEnvironmentVariable(key.c_str(), buffer, 4096);
  return std::string(buffer);
#elif defined(ROOTS_PLATFORM_LINUX) || defined(ROOTS_PLATFORM_APPLE)
  char *value = getenv(key.c_str());

  return value == nullptr ? "" : std::string(value);
#endif
}

auto set(const std::string &key, const std::string &value) -> void {
#ifdef ROOTS_PLATFORM_WINDOWS
  SetEnvironmentVariable(key.c_str(), value.c_str());
#elif defined(ROOTS_PLATFORM_LINUX) || defined(ROOTS_PLATFORM_APPLE)
  setenv(key.c_str(), value.c_str(), 1);
#endif
}

auto executablePath() -> std::string {
  char buffer[kRootsFilesystemMaxPathLength];
#ifdef ROOTS_PLATFORM_WINDOWS
  GetModuleFileName(NULL, buffer, kRootsFilesystemMaxPathLength);
#elif defined(ROOTS_PLATFORM_LINUX)
  readlink("/proc/self/exe", buffer, kRootsFilesystemMaxPathLength);
#elif defined(ROOTS_PLATFORM_APPLE)
  uint32_t size = kRootsFilesystemMaxPathLength;
  _NSGetExecutablePath(buffer, &size);
#endif
  return std::string(buffer);
}

auto processorCount() -> std::size_t {
  // C++11 supports this
  return std::thread::hardware_concurrency();
}

} // namespace roots::env
