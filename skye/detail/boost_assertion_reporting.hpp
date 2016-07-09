#ifndef skye_detail_boost_assertion_reporting_hpp
#define skye_detail_boost_assertion_reporting_hpp

#include <skye/detail/validator.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

namespace skye {
namespace detail {

/// Common logging function for Boost.Test reporting
inline void boost_log_common(
    location const & where, std::string const & msg,
    boost::unit_test::log_level ll, bool success) {
    boost::unit_test::unit_test_log
        << boost::unit_test::log::begin(where.file, where.line)
        << ll << msg
        << boost::unit_test::log::end();
#if BOOST_VERSION < 106000
    boost::unit_test::framework::assertion_result(success);
#else
    boost::unit_test::framework::assertion_result(
        boost::unit_test::assertion_result(success));
#endif // BOOST_VERSION
}

/// Report to Boost.Test using BOOST_CHECK_* semantics.
struct boost_check_reporting {
  /// Checkpoint progress through the unit test
  static void checkpoint(location const & where) {
    boost::unit_test::unit_test_log.set_checkpoint(where.file, where.line);
  }

  /// Report a successful assertion.
  static void report_success(
      location const & where, std::string const & msg) {
    boost_log_common(
        where, msg, boost::unit_test::log_successful_tests, true);
  }

  /// Report an assertion failure.
  static void report_failure(
      location const & where, std::string const & msg) {
    boost_log_common(
        where, msg, boost::unit_test::log_all_errors, false);
  }
};

/// Report to Boost.Test using BOOST_REQUIRE_* semantics.
struct boost_require_reporting {
  /// Checkpoint progress through the unit test
  static void checkpoint(location const & where) {
    boost::unit_test::unit_test_log.set_checkpoint(where.file, where.line);
  }

  /// Report a successful assertion.
  static void report_success(
      location const & where, std::string const & msg) {
    boost_log_common(
        where, msg, boost::unit_test::log_successful_tests, true);
  }

  /// Report an assertion failure.
  static void report_failure(
      location const & where, std::string const & msg) {
    boost_log_common(
        where, msg, boost::unit_test::log_fatal_errors, false);
    boost::unit_test::framework::test_unit_aborted(
        boost::unit_test::framework::current_test_case() );
    throw boost::execution_aborted();
  }
};

} // namespace detail
} // namespace skye

#endif // skye_detail_boost_assertion_reporting_hpp
