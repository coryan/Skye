#ifndef escapement_e_testing_invocation_args_wrapper_hpp
#define escapement_e_testing_invocation_args_wrapper_hpp

#include <tuple>
#include <type_traits>
#include <utility>
#include <iostream>

// Please read the documentation for make_arg_wrapper() for an
// explanation of what is going on here.
namespace e {
namespace testing {

template<typename T, bool copy_constructible>
struct argument_wrapper;

template<typename T>
struct argument_wrapper<T,true> {
  typedef T type;
};

template<typename T>
struct unsafe_pointer_holder {
  explicit unsafe_pointer_holder(T const & t)
      : unsafe_pointer_value(&t)
  {}

  T const * unsafe_pointer_value;
};

template<typename T>
std::ostream & operator<<(
    std::ostream & os, unsafe_pointer_holder<T> const & ) {
  return os << "{unsafe}";
}

template<typename T>
struct argument_wrapper<T,false> {
  typedef unsafe_pointer_holder<T> type;
};

/**
 * Create a suitable wrapper type to save an invocation argument into
 * a tuple.
 *
 * In the implementation of mock functions we want to save the
 * arguments passed to each call into a sequence of tuples.  This is
 * easy when the arguments are all copy constructible, but a tuple
 * with a non-copyable element is also not copyable.
 *
 * As a workaround, we save a pointer to the argument, but wrapped
 * with a suitable object to indicate that access to the value is
 * unsafe.  In many tests it would be safe to dereference the pointer,
 * but we do not know this to be a fact.
 */
template<typename T>
struct wrap_arg {
  typedef typename std::remove_reference<T>::type no_ref_type;
  static bool const copyable = std::is_copy_constructible<no_ref_type>::value;

  typedef typename argument_wrapper<no_ref_type,copyable>::type type;

  static type wrap(T && t) {
    return type(t);
  }
};

template<class T>
bool const wrap_arg<T>::copyable;

template<typename T>
auto make_arg_wrapper(T const & t) -> decltype(wrap_arg<T>::wrap(t)) {
  return wrap_arg<T>::wrap(t);
}

template<typename T>
auto make_arg_wrapper(T && t) -> decltype(wrap_arg<T>::wrap(t)) {
  return wrap_arg<T>::wrap(t);
}

template<typename T>
auto forward_wrapper(T && arg) -> decltype(wrap_arg<T>::wrap(arg)) {
  return wrap_arg<T>::wrap(std::forward<T>(arg));
}

template<typename... args>
struct deduce_wrap_args_as_tuple_types {
  typedef std::tuple<typename wrap_arg<args>::type...> tuple;
};

template<typename... args>
std::tuple<typename wrap_arg<args>::type...>
wrap_args_as_tuple(args&&... a) {
  return std::tuple<typename wrap_arg<args>::type...>(
      forward_wrapper(a)...);
}

} // namespace testing
} // namespace e


#endif // escapement_e_testing_invocation_args_wrapper_hpp
