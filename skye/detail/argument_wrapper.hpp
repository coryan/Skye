#ifndef skye_detail_invocation_argument_wrapper_hpp
#define skye_detail_invocation_argument_wrapper_hpp

#include <skye/detail/tuple_streaming.hpp>

#include <iostream>
#include <tuple>
#include <type_traits>
#include <vector>

namespace skye {
namespace detail {

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

inline std::ostream & operator<<(std::ostream & os, place_holder const&) {
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

/**
 * A traits class describe how an argument is wrapped.
 */
template<typename T>
struct argument_traits {
  typedef typename std::remove_reference<T>::type base_type;
  static bool const copyable = std::is_copy_constructible<base_type>::value;
  typedef typename argument_wrapper_type<base_type,copyable>::type type;
};

template<typename T>
bool const argument_traits<T>::copyable;

/**
 * Wrap an argument in the correct type, using perfect forwarding.
 */
template<typename T>
typename argument_traits<T>::type make_arg_wrapper(T && t) {
  return typename argument_traits<T>::type(std::forward<T>(t));
}

/**
 * Wrap an argument in the correct type, based on a const reference.
 */
template<typename T>
typename argument_traits<T>::type make_arg_wrapper(T const & t) {
  return typename argument_traits<T>::type(t);
}

/**
 * Forward a single argument to make_ar_wrapper function.
 */
template<typename T>
auto forward_wrapper(T && arg)
    -> decltype(make_arg_wrapper(std::forward<T>(arg))) {
  return make_arg_wrapper(std::forward<T>(arg));
}

/**
 * Wrap a list of arguments into a tuple using make_arg_wrapper() for each one.
 */
template<typename... args>
auto wrap_args_as_tuple(args&&... a)
    -> decltype(std::make_tuple(forward_wrapper(a)...)) {
  return std::make_tuple(forward_wrapper(a)...);
}

/**
 * Define the default strategy to capture arguments in mock functions.
 *
 * For simple mock functions, i.e., those where the argument types are
 * known in advance, this strategy copies the arguments to a tuple.
 * Each argument is wrapped to deal with non-copy constructible,
 * non-printable and non-comparable arguments.
 */
template<typename... arg_types>
class known_arguments_capture_by_value {
 public:
  /// A single argument capture.
  typedef decltype(wrap_args_as_tuple(std::declval<arg_types>()...)) value_type;

  /// The type representing a sequence of argument captures.
  typedef std::vector<value_type> capture_sequence;

  /// Capture a set of arguments.
  static value_type capture(arg_types&&... args) {
    return wrap_args_as_tuple(args...);
  }

  static bool equals(value_type const & lhs, value_type const & rhs) {
    return lhs == rhs;
  }

  static void stream(std::ostream & os, value_type const & x) {
    os << x;
  }
};

} // namespace detail
} // namespace skye

#endif // skye_detail_invocation_argument_wrapper_hpp
