#include <skye/detail/argument_wrapper.hpp>
#include <skye/detail/tuple_streaming.hpp>

#include <boost/test/unit_test.hpp>

using namespace skye::detail;

/**
 * Implement simple types to drive the tests.
 */
namespace {
/**
 * A non-copy-constructible class (i.e., one where std::is_copyable<>
 * returns false), to test how such arguments are wrapped.
 */
struct no_copy {
  no_copy(no_copy const &) = delete;
  no_copy()
      : value(42)
  {}
  explicit no_copy(int x)
      : value(x)
  {}

  bool operator==(int rhs) const {
    return value == rhs;
  }
  bool operator!=(int rhs) const {
    return !(*this == rhs);
  }
  bool operator==(no_copy const & rhs) const {
    return value == rhs.value;
  }
  bool operator!=(no_copy const & rhs) const {
    return !(*this == rhs);
  }

  int value;
};

/**
 * The streaming operator for no_copy
 */
std::ostream & operator<<(std::ostream & os, no_copy const & x) {
  return os << "no_copy{value=" << x.value << "}";
}

/**
 * A class without a streaming operator, i.e., one where the following
 * code snippet would fail to compile:
 *
 * @code
 * void function(std::ostream & os, no_stream const & x) {
 *   os << x;
 * }
 * @endcode
 */
struct no_stream {
  no_stream(no_stream const &) = default;
  no_stream& operator=(no_stream const &) = default;
  no_stream() = default;
  explicit no_stream(int x)
      : value(x)
  {}

  bool operator==(int rhs) const {
    return value == rhs;
  }
  bool operator!=(int rhs) const {
    return !(*this == rhs);
  }
  bool operator==(no_stream const & rhs) const {
    return value == rhs.value;
  }
  bool operator!=(no_stream const & rhs) const {
    return !(*this == rhs);
  }

  int value;
};

/**
 * A class without equality and inequality operators.
 */
struct no_compare {
  no_compare(no_compare const &) = default;
  no_compare& operator=(no_compare const &) = default;
  no_compare() = default;
  explicit no_compare(int x)
      : value(x)
  {}

  int value;
};

/**
 * The streaming operator for no_compare.
 */
std::ostream & operator<<(std::ostream & os, no_compare const & x) {
  return os << x.value;
}

} // anonymous namespace

BOOST_AUTO_TEST_CASE( argument_wrapper_int ) {
  int x = 5;
  auto w1 = make_arg_wrapper(x);
  BOOST_CHECK_EQUAL(w1, 5);

  int const & y = x;
  auto w2 = make_arg_wrapper(y);
  bool const is_same = std::is_same<decltype(w1), decltype(w2)>::value;
  BOOST_CHECK_MESSAGE(is_same, "The two wrappers should return the same type");
  BOOST_CHECK_EQUAL(w1, w2);

  std::ostringstream os;
  os << w1;
  BOOST_CHECK_EQUAL(os.str(), "5");
}

BOOST_AUTO_TEST_CASE( argument_wrapper_string ) {
  std::string s("foo bar baz");

  auto w1 = make_arg_wrapper(s);
  BOOST_CHECK_EQUAL(w1, "foo bar baz");

  auto w2 = make_arg_wrapper(std::string("bar baz"));
  BOOST_CHECK_NE(w1, w2);

  std::ostringstream os;
  os << w1;
  BOOST_CHECK_EQUAL(os.str(), "foo bar baz");
}

BOOST_AUTO_TEST_CASE( argument_wrapper_copy_references ) {
  using namespace skye::detail;

  int a = 1;
  int & b = a;
  int const & c = b;

  BOOST_CHECK_EQUAL(argument_traits<decltype(a)>::copyable, true);
  BOOST_CHECK_EQUAL(argument_traits<decltype(b)>::copyable, true);
  BOOST_CHECK_EQUAL(argument_traits<decltype(c)>::copyable, true);

  int x = make_arg_wrapper(a);
  BOOST_CHECK_EQUAL(x, a);
  int y = make_arg_wrapper(b);
  BOOST_CHECK_EQUAL(y, a);
  int z = make_arg_wrapper(c);
  BOOST_CHECK_EQUAL(z, a);
}

BOOST_AUTO_TEST_CASE( argument_wrapper_no_copy ) {
  no_copy x(42);
  no_copy y(42);

  auto w1 = make_arg_wrapper(x);
  BOOST_CHECK_NE(w1, y);

  std::ostringstream os;
  os << w1;
  BOOST_CHECK_EQUAL(os.str(), "[::place_holder::]");
}

BOOST_AUTO_TEST_CASE( argument_wrapper_no_stream ) {
  no_stream x(42);

  auto w1 = make_arg_wrapper(x);
  BOOST_CHECK_MESSAGE(
      w1 == no_stream(42),
      "Mismatched values, got=" << x.value << ", expected=" << 42);

  std::ostringstream os;
  os << w1;
  BOOST_CHECK_EQUAL(os.str(), "[::non_streamable::]");
}

BOOST_AUTO_TEST_CASE( argument_wrapper_no_compare ) {
  no_compare x(42);

  auto w1 = make_arg_wrapper(x);
  auto w2 = make_arg_wrapper(x);
  BOOST_CHECK_MESSAGE(
      w1 != w2,
      "Mismatched values, got=" << w1 << ", expected=" << x.value);

  BOOST_CHECK_EQUAL(w1.value.value, w2.value.value);

  std::ostringstream os;
  os << w1;
  BOOST_CHECK_EQUAL(os.str(), "42");
}

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

BOOST_AUTO_TEST_CASE( wrap_simple_args ) {
  using namespace skye::detail;

  auto w = wrap_args_as_tuple(1, 2, 3, std::string("42"));
  std::ostringstream os;
  os << w;
  BOOST_CHECK_EQUAL(os.str(), "<1,2,3,42>");
}

BOOST_AUTO_TEST_CASE( wrap_simple_ref_args ) {
  using namespace skye::detail;

  int x = 1;
  int y = 2;
  int & a = x;
  int const & b = y;
  std::string const c("abc");
  std::string const & d = c;

  auto w = wrap_args_as_tuple(a, b, c, d, c, b, a);
  std::ostringstream os;
  os << w;
  BOOST_CHECK_EQUAL(os.str(), "<1,2,abc,abc,abc,2,1>");
}

BOOST_AUTO_TEST_CASE( wrap_no_copy ) {
  using namespace skye::detail;

  no_copy a;
  no_copy & b = a;

  {
    std::ostringstream os;
    os << b;
    BOOST_CHECK_EQUAL(os.str(), "no_copy{value=42}");
  }

  no_copy const & c = a;
  auto w = wrap_args_as_tuple(a, b, c, 1, 2);
  std::ostringstream os;
  os << w;
  BOOST_CHECK_EQUAL(os.str(),
                    "<[::place_holder::],[::place_holder::]"
                    ",[::place_holder::],1,2>");
}
