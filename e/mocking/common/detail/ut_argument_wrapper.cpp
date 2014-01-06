#include <e/mocking/common/detail/argument_wrapper.hpp>

#include <boost/test/unit_test.hpp>

#include <iostream>
#include <type_traits>

using namespace e::mocking::common::detail;

/**
 * Wrap an argument so we can guarantee there are streaming and
 * comparison operators defined.
 */
template<typename T>
struct argument_wrapper {
  typedef T value_type;

  explicit argument_wrapper(T const & t)
      : value(t)
  {}
  explicit argument_wrapper(T && t)
      : value(t)
  {}

  operator T const&() const {
    return value;
  }
  
  value_type value;
};

/**
 * Helper function that determines if there is a operator<<() defined
 * for std::ostream and T::value_type.
 *
 * There are two overloads, but one has a variadic template so it is
 * only used if the first one fails.  The first one only works if
 * decltype(os << t.value) is a legal expression, otherwise SFINAE
 * removes it from consideration.
 */
template<typename T>
auto safe_streaming(
    std::ostream & os, argument_wrapper<T> const & t, bool)
    -> decltype(os << t.value, void()) {
  os << t.value;
}

/**
 * Generic overload used when streaming operator is not defined.
 */
template<typename T, typename... non_streamable>
void safe_streaming(
    std::ostream & os, argument_wrapper<T> const & t, non_streamable...) {
  os << "[::non_streamable::]";
}

/**
 * Safe streaming operator for the wrapper classes.
 */
template<typename T>
std::ostream & operator<<(std::ostream & os, argument_wrapper<T> const & t) {
  safe_streaming(os, t, true);
  return os;
}

/**
 * Helper function that determines if there is a operator==() defined
 * for a pair of wrapped arguments.
 *
 * @see safe_streaming for an explanation of the technique.
 */
template<typename T>
auto safe_equals(
    argument_wrapper<T> const & lhs, argument_wrapper<T> const & rhs, bool)
    -> decltype(lhs.value == rhs.value, bool()) {
  return lhs.value == rhs.value;
}

template<typename T, typename U>
auto safe_equals(
    argument_wrapper<T> const & lhs, U const & rhs, bool)
    -> decltype(rhs == lhs.value, bool()) {
  return rhs == lhs.value;
}

template<typename T, typename... no_equals>
bool safe_equals(
    argument_wrapper<T> const & lhs, no_equals...) {
  return false;
}

template<typename T>
bool operator==(
    argument_wrapper<T> const & lhs, argument_wrapper<T> const & rhs) {
  return safe_equals(lhs, rhs, true);
}

template<typename T, typename U>
bool operator==(
    argument_wrapper<T> const & lhs, U const & rhs) {
  return safe_equals(lhs, rhs, true);
}

template<typename T>
bool operator!=(
    argument_wrapper<T> const & lhs, argument_wrapper<T> const & rhs) {
  return !(lhs == rhs);
}

template<typename T, typename U>
bool operator!=(
    argument_wrapper<T> const & lhs, U const & rhs) {
  return !(lhs == rhs);
}

/**
 * A simple object to hold in place of non-copy-constructible objects.
 */
struct place_holder {
  place_holder() = default;
  place_holder(place_holder const &) = default;
  place_holder & operator=(place_holder const &) = default;


  template<typename T>
  explicit place_holder(T const &) {}

  template<typename T>
  place_holder & operator=(T const &) {
    return *this;
  }

  template<typename T>
  bool operator==(T const &) const {
    return false;
  }
  template<typename T>
  bool operator!=(T const &) const {
    return false;
  }

  bool operator==(place_holder const &) const {
    return true;
  }
  bool operator!=(place_holder const &) const {
    return true;
  }
};

std::ostream & operator<<(std::ostream & os, place_holder const&) {
  return os << "[::place_holder::]";
}

/**
 * Determine how are copy-constructible vs. non-copy-constructible
 * objects captured.
 */
template<typename T, bool copyable>
struct argument_wrapper_type;

/**
 * For copy construtible objects, use the object itself.
 */
template<typename T>
struct argument_wrapper_type<T,true> {
  typedef argument_wrapper<T> type;
};

/**
 * For non-copy constructible object, use a place holder
 */
template<typename T>
struct argument_wrapper_type<T,false> {
  typedef place_holder type;
};

template<typename T>
struct argument_traits {
  typedef typename std::remove_reference<T>::type base_type;
  static bool const copyable = std::is_copy_constructible<base_type>::value;
  typedef typename argument_wrapper_type<base_type,copyable>::type type;
};

template<typename T>
typename argument_traits<T>::type make_arg_wrapper(T && t) {
  return typename argument_traits<T>::type(std::forward<T>(t));
}

template<typename T>
typename argument_traits<T>::type make_arg_wrapper(T const & t) {
  return typename argument_traits<T>::type(t);
}


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
