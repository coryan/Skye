#include <e/mocking/common/detail/argument_wrapper.hpp>

#include <boost/test/unit_test.hpp>

using namespace e::mocking::common::detail;

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

struct no_copy {
  no_copy(no_copy const &) = delete;
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

std::ostream & operator<<(std::ostream & os, no_copy const & x) {
  return os << x.value;
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

struct no_compare {
  no_compare(no_compare const &) = default;
  no_compare& operator=(no_compare const &) = default;
  no_compare() = default;
  explicit no_compare(int x)
      : value(x)
  {}

  int value;
};

std::ostream & operator<<(std::ostream & os, no_compare const & x) {
  return os << x.value;
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
