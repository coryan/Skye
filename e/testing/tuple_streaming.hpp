#ifndef escapement_e_testing_tuple_streaming_hpp
#define escapement_e_testing_tuple_streaming_hpp

#include <tuple>
#include <iostream>

namespace e {
namespace testing {

template<typename tuple_t, std::size_t N>
struct tuple_contents_printer {
  static void print(std::ostream & os, tuple_t const & x) {
    tuple_contents_printer<tuple_t, N-1>::print(os, x);
    os << "," << std::get<N-1>(x);
  }
};

template<typename tuple_t>
struct tuple_contents_printer<tuple_t,1> {
  static void print(std::ostream & os, tuple_t const & x) {
    os << std::get<0>(x);
  }
};

template<typename tuple_t>
struct tuple_contents_printer<tuple_t,0> {
  static void print(std::ostream & , tuple_t const & ) {
  }
};

} // namespace testing
} // namespace e

namespace std {

template<typename... args>
ostream & operator<<(
    ostream & os, tuple<args...> const & x) {
  os << "<";
  e::testing::tuple_contents_printer<
    tuple<args...>, sizeof...(args)>::print(os, x);
  return os << ">";
}

} // namespace std

#endif // escapement_e_testing_tuple_streaming_hpp
