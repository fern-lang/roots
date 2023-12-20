#include "Roots/Filesystem.hpp"
#include "Roots/Environment.hpp"

namespace roots::fs {

auto cwd() -> path_type { return std::filesystem::current_path(); }

auto home() -> path_type {
  return std::filesystem::path(roots::env::get("HOME"));
}

auto readFile(const path_type &path) -> roots::result<std::string, Error> {
  if (!roots::fs::exists(path)) {
    return roots::fail(
        Error(ErrKind::FileIo, "unable to read, file does not exist"));
  }

  std::ifstream file(path);
  std::string contents((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());

  return path_type(contents);
}

auto readFileBytes(const path_type &path)
    -> roots::result<std::vector<u8>, Error> {
  if (!roots::fs::exists(path)) {
    return roots::fail(
        Error(ErrKind::FileIo, "unable to read, file does not exist"));
  }

  std::ifstream file(path, std::ios::binary);
  std::vector<u8> contents((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());

  return contents;
}

auto exists(const path_type &path) -> bool {
  return std::filesystem::exists(path);
}

auto relativePath(const path_type &path, const path_type &dir)
    -> roots::result<path_type, Error> {
  if (!roots::fs::exists(path)) {
    return roots::fail(Error(ErrKind::FileIo,
                           "unable to get relative path, file does not exist"));
  }

  return std::filesystem::relative(path, dir);
}

auto absolutePath(const path_type &path) -> roots::result<path_type, Error> {
  if (!roots::fs::exists(path)) {
    return roots::fail(Error(ErrKind::FileIo,
                           "unable to get absolute path, file does not exist"));
  }

  return std::filesystem::absolute(path);
}

auto isAbsolute(const path_type &path) -> bool { return path.is_absolute(); }

auto isRelative(const path_type &path) -> bool { return path.is_relative(); }

auto isDir(const path_type &path) -> bool {
  return std::filesystem::is_directory(path);
}

auto isFile(const path_type &path) -> bool {
  return std::filesystem::is_regular_file(path);
}

auto search(const path_type &dir,
            const std::function<bool(const path_type &path)> &matcher)
    -> roots::result<path_type, Error> {
  if (!roots::fs::exists(dir)) {
    return roots::fail(
        Error(ErrKind::FileIo, "unable to search, directory does not exist"));
  }

  for (auto &p : std::filesystem::recursive_directory_iterator(dir)) {
    if (matcher(p.path())) {
      return p.path();
    }
  }

  return roots::fail(
      Error(ErrKind::FileIo, "unable to search, no matching file found"));
}

auto writeBytes(const path_type &path, const std::vector<u8> &bytes)
    -> roots::result<void, Error> {
  if (!roots::fs::exists(path)) {
    return roots::fail(
        Error(ErrKind::FileIo, "unable to write, file does not exist"));
  }

  std::ofstream file(path, std::ios::binary);
  file.write((char *)bytes.data(), bytes.size());
  file.close();

  return roots::result<void, Error>();
}

} // namespace roots::fs
