#include <skye/common/mock_template_function.hpp>

#include <boost/test/unit_test.hpp>
#include <sstream>

/**
 * Define helper types for the tests.
 */
namespace {

struct test_no_copy {
  test_no_copy()
      : value(42)
  {}
  explicit test_no_copy(int x)
      : value(x)
  {}

  test_no_copy(test_no_copy const &) = delete;
  test_no_copy & operator=(test_no_copy const &) = delete;

  int value;
};

std::ostream & operator<<(std::ostream & os, test_no_copy const & x ) {
  return os << "{" << x.value << "}";
}

} // anonymous namespace

using namespace e::mocking::common;

BOOST_AUTO_TEST_CASE( mock_template_function_void ) {
  mock_template_function<void> function;

  BOOST_CHECK(not function.has_calls());
  BOOST_CHECK_EQUAL(function.call_count(), 0);

  int a = 0;
  int b_value = 3;
  int const & b = b_value;
  std::string c("abc");
  std::string d_value("dce");
  std::string const & d = c;

  function(a, b);
  function(a, b, c);
  function(a, b, c, d);

  BOOST_REQUIRE_EQUAL(function.call_count(), 3);
}

BOOST_AUTO_TEST_CASE( mock_template_function_with_value ) {
  mock_template_function<std::string> function;

  BOOST_CHECK(not function.has_calls());
  BOOST_CHECK_EQUAL(function.call_count(), 0);

  int a = 0;
  int b_value = 3;
  int const & b = b_value;
  std::string c("abc");
  std::string d_value("dce");
  std::string const & d = c;

  BOOST_CHECK_THROW(function(a, b), std::runtime_error);

  function.returns( std::string("42") );
  function(a, b, c);
  function(a, b, c, d);

  BOOST_REQUIRE_EQUAL(function.call_count(), 3);
}

BOOST_AUTO_TEST_CASE( mock_template_function_void_no_copy ) {
  mock_template_function<void> function;

  BOOST_CHECK(not function.has_calls());
  BOOST_CHECK_EQUAL(function.call_count(), 0);

  int a = 0;
  test_no_copy b_value(42);
  test_no_copy const & b = b_value;

  function(a);
  function(a, b);

  BOOST_REQUIRE_EQUAL(function.call_count(), 2);
}

BOOST_AUTO_TEST_CASE( mock_template_function_check_no_calls ) {
  mock_template_function<void> function;

  function.check_called().never();
  function(42);
  function.check_called().once();
}

BOOST_AUTO_TEST_CASE( mock_template_function_asserts ) {
  mock_template_function<int> function;

  function.returns( 7 );

  function(42, std::string("foo"));
  function(42, std::string("foo"));
  function(42, std::string("foo"));
  function(42, std::string("foo"), 3, 1);
  function(std::string("foo"), std::string("bar"));

  function.check_called();
  function.check_called().at_least( 2 );
  function.check_called().at_least( 2 ).at_most( 30 );
  function.check_called().between( 3, 30 );
  function.check_called().exactly( 5 );

  function.check_called().never().with( 7, std::string("bar"));
  function.check_called().at_least( 3 ).with( 42, std::string("foo"));
  function.check_called().once().with( std::string("foo"), std::string("bar"));

}

