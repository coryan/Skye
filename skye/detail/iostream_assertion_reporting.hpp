#ifndef skye_detail_iostream_assertion_reporting_hpp
#define skye_detail_iostream_assertion_reporting_hpp

#include <skye/detail/validator.hpp>

#include <iostream>
#include <stdexcept>

namespace skye {
namespace detail {

/// Common logging function for Boost.Test reporting
inline void iostream_log_common(
    location const & where, std::string const & msg,
    std::string const & severity, bool success) {
  std::cerr << severity << " assertion"
            << "(" << where.file << ":" << where.line << ") "
            << (success ? "success" : "failure")
            << ": "
            << msg
            << std::endl;
}

/// Report to stderr, with no further action on failures.
struct iostream_check_reporting {
  /// Checkpoint progress through the unit test
  static void checkpoint(location const & ) {
  }

  /// Report a successful assertion.
  static void report_success(
      location const & where, std::string const & msg) {
    iostream_log_common(where, msg, "checked", true);
  }

  /// Report an assertion failure.
  static void report_failure(
      location const & where, std::string const & msg) {
    iostream_log_common(where, msg, "checked", false);
  }
};

/// Report to stderr, and raise an exception on failures.
struct iostream_require_reporting {
  /// Checkpoint progress through the unit test
  static void checkpoint(location const & ) {
  }

  /// Report a successful assertion.
  static void report_success(
      location const & where, std::string const & msg) {
    iostream_log_common(where, msg, "required", true);
  }

  /// Report an assertion failure.
  static void report_failure(
      location const & where, std::string const & msg) {
    iostream_log_common(where, msg, "required", false);
    throw std::runtime_error(msg);
  }
};

} // namespace detail
} // namespace skye

#endif // skye_detail_iostream_assertion_reporting_hpp
