#include <e/testing/invocation.hpp>

#include <sstream>

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

ostream & operator<<(ostream & os, tuple<> const &) {
  return os << "<>";
}

template<typename... args>
ostream & operator<<(
    ostream & os, tuple<args...> const & x) {
  os << "<";
  e::testing::tuple_contents_printer<
    decltype(x), sizeof...(args)>::print(os, x);
  return os << ">";
}

} // namespace std

#include <boost/test/unit_test.hpp>

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

BOOST_AUTO_TEST_CASE( mock_invocation ) {
  typedef e::testing::invocation<int> invocation_mock;
  invocation_mock::value_type x(1);

  invocation_mock record;

  BOOST_CHECK(not record.has_calls());
  BOOST_CHECK_EQUAL(record.call_count(), 0);

  record(1);
  record(2);
  record(3);

  BOOST_REQUIRE_EQUAL(record.call_count(), 3);
  BOOST_CHECK_EQUAL(record.at(0), invocation_mock::value_type(1));
  BOOST_CHECK_EQUAL(record.at(1), invocation_mock::value_type(2));
  BOOST_CHECK_EQUAL(record.at(2), invocation_mock::value_type(3));
}
