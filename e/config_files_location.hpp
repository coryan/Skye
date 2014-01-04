#ifndef escapment_lib_e_config_files_location_hpp
#define escapment_lib_e_config_files_location_hpp

#include <boost/filesystem/path.hpp>
#include <cstdlib>

namespace e {

/**
 * Directory validation functor.
 */
typedef bool (*directory_validator)(boost::filesystem::path const &);

/**
 * Return true if the path is a readable directory.
 */
bool usable_config_files_directory(boost::filesystem::path const &);

/**
 * Define the configuration file search algorithm for Escapement.
 *
 * Please @see config_files_location_base, this function just hides
 * the implementation.
 *
 * @param program_path the path for the current program (typically
 *   argv[0])
 * @param escapement_root the value of the ESCAPEMENT_ROOT environment
 *   variable, or nullptr if not defined
 * @param program_root the value of the program specific *_ROOT
 *   environment variable, or nullptr if not defined
 * @param validator validate the computed directories,
 *   intended for testing.
 */
boost::filesystem::path config_files_location_impl(
    char const * program_path,
    char const * escapement_root,
    char const * program_root,
    directory_validator validator = usable_config_files_directory);

/**
 * Search for a configuration file in the standard places.
 *
 * Escapement configuration files can be located in multiple places:
 * # Each program defines its own '*_ROOT' environment variable, the
 *   files are search there if the variable is defined, otherwise
 * # the files are found in the generic 'ESCAPEMENT_ROOT' location, if
 *   that environment variable is defined, otherwise
 * # the files are found in the installation directory, if the
 *   directory exists, otherwise,
 * # the files are found relative to the program path name.
 *
 * This class uses a template parameter to allow unit testing without
 * changes to the environment variables.
 */
template<typename getenv_functor>
struct config_files_location_base {
  boost::filesystem::path operator()(
      char const * program_path,
      char const * program_root_name,
      directory_validator validator = usable_config_files_directory) {
    getenv_functor functor;
    char const * program_root = functor(program_root_name);
    char const * escapement_root = functor("ESCAPEMENT_ROOT");
    return config_files_location_impl(
        program_path, escapement_root, program_root, validator);
  }
};

struct use_getenv {
  char const * operator()(char const * name) { return std::getenv(name); }
};

/**
 * Define the typical functor to search for config file locations.
 */
typedef config_files_location_base<use_getenv> config_files_location;

} // namespace e

#endif // escapment_lib_e_config_files_location_hpp
