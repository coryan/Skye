#ifndef skye_detail_assertion_reporting_hpp
#define skye_detail_assertion_reporting_hpp

#if defined(SKYE_USE_BOOST_UNIT_TEST_FRAMEWORK)

#  include <skye/detail/boost_assertion_reporting.hpp>
namespace skye {
namespace detail {

typedef boost_check_reporting default_check_reporting;
typedef boost_require_reporting default_require_reporting;

} // namespace detail
} // namespace skye

#else /* SKYE_USE_BOOST_UNIT_TEST_FRAMEWORK */

#  include <skye/detail/iostream_assertion_reporting.hpp>
namespace skye {
namespace detail {

typedef iostream_check_reporting default_check_reporting;
typedef iostream_require_reporting default_require_reporting;

} // namespace detail
} // namespace skye

#endif /* Fallback, no special macro defined */

#endif // skye_detail_assertion_reporting_hpp
