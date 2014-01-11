#ifndef skye_detail_tuple_streaming_hpp
#define skye_detail_tuple_streaming_hpp

#include <tuple>
#include <iostream>

namespace skye {
namespace detail {

/**
 * Print the contents of a generic tuple using iostream.
 *
 * This is a very naive implementation in that it assumes all the
 * objects have an streaming operator defined, but for types involved
 * in a unit test this is often safe.
 * TODO(ES-40)
 */
template<typename tuple_t, std::size_t N>
struct tuple_contents_printer {
  static void print(std::ostream & os, tuple_t const & x) {
    tuple_contents_printer<tuple_t, N-1>::print(os, x);
    os << "," << std::get<N-1>(x);
  }
};

/**
 * Partial specialization for tuples with a single element.
 */
template<typename tuple_t>
struct tuple_contents_printer<tuple_t,1> {
  static void print(std::ostream & os, tuple_t const & x) {
    os << std::get<0>(x);
  }
};

/**
 * Partial specialization for empty tuples.
 */
template<typename tuple_t>
struct tuple_contents_printer<tuple_t,0> {
  static void print(std::ostream & , tuple_t const & ) {
  }
};

} // namespace detail
} // namespace skye

namespace std {

/**
 * Introduce a streaming operator for all tuples.
 */
template<typename... args>
ostream & operator<<(
    ostream & os, tuple<args...> const & x) {
  os << "<";
  skye::detail::tuple_contents_printer<
    tuple<args...>, sizeof...(args)>::print(os, x);
  return os << ">";
}

} // namespace std

#endif // skye_detail_tuple_streaming_hpp
