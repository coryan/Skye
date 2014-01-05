#include <e/testing/invocation_args_wrapper.hpp>
#include <e/testing/tuple_streaming.hpp>

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

BOOST_AUTO_TEST_CASE( tuple_streaming_empty ) {
  std::tuple<> empty;
  std::ostringstream os;
  os << empty;
  BOOST_CHECK_EQUAL(os.str(), "<>");
}

BOOST_AUTO_TEST_CASE( tuple_streaming_single ) {
  std::tuple<int> single(42);
  std::ostringstream os;
  os << single;
  BOOST_CHECK_EQUAL(os.str(), "<42>");
}

BOOST_AUTO_TEST_CASE( tuple_streaming_triple ) {
  std::tuple<int,std::string,std::string> x(42, "x", "y");
  std::ostringstream os;
  os << x;
  BOOST_CHECK_EQUAL(os.str(), "<42,x,y>");
}

BOOST_AUTO_TEST_CASE( argument_wrapper_copy_constructible ) {
  int a = 1;
  int & b = a;
  int const & c = b;

  BOOST_CHECK_EQUAL(e::testing::wrap_arg<decltype(a)>::copyable, true);
  BOOST_CHECK_EQUAL(e::testing::wrap_arg<decltype(b)>::copyable, true);
  BOOST_CHECK_EQUAL(e::testing::wrap_arg<decltype(c)>::copyable, true);

  int x = e::testing::make_arg_wrapper(a);
  BOOST_CHECK_EQUAL(x, a);
  int y = e::testing::make_arg_wrapper(b);
  BOOST_CHECK_EQUAL(y, a);
  int z = e::testing::make_arg_wrapper(c);
  BOOST_CHECK_EQUAL(z, a);
}

BOOST_AUTO_TEST_CASE( argument_wrapper_no_copy ) {
  test_no_copy a(7);
  test_no_copy & b = a;
  test_no_copy const & c = b;

  BOOST_CHECK_EQUAL(e::testing::wrap_arg<decltype(a)>::copyable, false);
  BOOST_CHECK_EQUAL(e::testing::wrap_arg<decltype(b)>::copyable, false);
  BOOST_CHECK_EQUAL(e::testing::wrap_arg<decltype(c)>::copyable, false);

  auto x = e::testing::make_arg_wrapper(a);
  auto y = e::testing::make_arg_wrapper(b);
  auto z = e::testing::make_arg_wrapper(c);

  BOOST_CHECK_EQUAL(x.unsafe_pointer_value->value, a.value);
  BOOST_CHECK_EQUAL(y.unsafe_pointer_value->value, a.value);
  BOOST_CHECK_EQUAL(z.unsafe_pointer_value->value, a.value);
}

BOOST_AUTO_TEST_CASE( wrap_simple_args ) {
  auto w = e::testing::wrap_args_as_tuple(1, 2, 3, std::string("42"));
  std::ostringstream os;
  os << w;
  BOOST_CHECK_EQUAL(os.str(), "<1,2,3,42>");
}

BOOST_AUTO_TEST_CASE( wrap_simple_ref_args ) {
  int x = 1;
  int y = 2;
  int & a = x;
  int const & b = y;
  std::string const c("abc");
  std::string const & d = c;

  auto w = e::testing::wrap_args_as_tuple(a, b, c, d, c, b, a);
  std::ostringstream os;
  os << w;
  BOOST_CHECK_EQUAL(os.str(), "<1,2,abc,abc,abc,2,1>");
}

BOOST_AUTO_TEST_CASE( wrap_no_copy ) {

  test_no_copy a;
  test_no_copy & b = a;

  {
    std::ostringstream os;
    os << b;
    BOOST_CHECK_EQUAL(os.str(), "{42}");
  }

  test_no_copy const & c = a;
  auto w = e::testing::wrap_args_as_tuple(a, b, c, 1, 2);
  std::ostringstream os;
  os << w;
  BOOST_CHECK_EQUAL(os.str(), "<{unsafe},{unsafe},{unsafe},1,2>");
}
