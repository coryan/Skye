#include <e/config_files_location.hpp>
#include <e/config.hpp>

#include <boost/filesystem.hpp>
#include <stdexcept>

namespace fs = boost::filesystem;

bool e::usable_config_files_directory(fs::path const & dir) {
  return fs::exists(dir) and is_directory(dir);
}

fs::path e::config_files_location_impl(
    char const * program_path,
    char const * escapement_root,
    char const * program_root,
    directory_validator validator) {
  // Discover the name of the configuration directory for the platform
  fs::path sysconfdir = e::sysconfdir();
  fs::path bindir = e::bindir();
  fs::path sysconf_leaf = sysconfdir.filename();
  fs::path bin_leaf = bindir.filename();

  // If there is a *_ROOT environment variable defined, use that ...
  if (program_root != nullptr) {
    fs::path dir = fs::path(program_root) / sysconf_leaf;
    if (validator(dir)) {
      return dir;
    }
  }
  // ... otherwise, if ESCAPEMENT_ROOT is defined, use that ...
  if (escapement_root != nullptr) {
    fs::path dir = fs::path(escapement_root) / sysconf_leaf;
    if (validator(dir)) {
      return dir;
    }
  }
  // ... otherwise, if ESCAPEMENT_SYSCONFDIR was defined at
  // compile-time *and* the destination exists and is a directory, use
  // that ...
  if (validator(sysconfdir)) {
    return sysconfdir;
  }

  // ... otherwise, discover where the program is running and guess
  // where the data directory might be ...
  fs::path program(program_path);
  if (program.parent_path().filename() == bin_leaf) {
    fs::path dir = program.parent_path().parent_path() / sysconf_leaf;
    if (validator(dir)) {
      return dir;
    }
  }

  throw std::runtime_error("Cannot determine config files location");
}

