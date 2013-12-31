#include <e/testing/invocation.hpp>
#include <e/testing/tuple_streaming.hpp>

#include <boost/test/unit_test.hpp>
#include <sstream>

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

BOOST_AUTO_TEST_CASE( mock_invocation_simple ) {
  typedef e::testing::invocation<int> invocation_mock;

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

BOOST_AUTO_TEST_CASE( mock_invocation_several_types ) {
  typedef e::testing::invocation<
    int,std::string&,int const&,char*> invocation_mock;

  invocation_mock record;

  BOOST_CHECK(not record.has_calls());
  BOOST_CHECK_EQUAL(record.call_count(), 0);

  int a = 0;
  std::string b;
  int c_base = 2;
  int const & c = c_base;
  char d[] = "1234";

  b="abc"; record(++a, b, c, d);
  b="bcd"; record(++a, b, c, d);
  b="cde"; record(++a, b, c, d);

  BOOST_REQUIRE_EQUAL(record.call_count(), 3);
  BOOST_CHECK_EQUAL(record.at(0),
                    invocation_mock::value_type(1, "abc", 2, d));
  BOOST_CHECK_EQUAL(record.at(1),
                    invocation_mock::value_type(2, "bcd", 2, d));
  BOOST_CHECK_EQUAL(record.at(2),
                    invocation_mock::value_type(3, "cde", 2, d));
}

BOOST_AUTO_TEST_CASE( mock_invocation_no_copy ) {
  typedef e::testing::invocation<int,test_no_copy&> invocation_mock;

  invocation_mock record;

  BOOST_CHECK(not record.has_calls());
  BOOST_CHECK_EQUAL(record.call_count(), 0);

  int a = 0;
  test_no_copy b(42);

  record(++a, b);
  record(++a, b);
  record(++a, b);

  BOOST_REQUIRE_EQUAL(record.call_count(), 3);
  BOOST_CHECK_EQUAL(std::get<0>(record.at(0)), 1);
  BOOST_CHECK_EQUAL(std::get<1>(record.at(0)).unsafe_pointer_value, &b);
  BOOST_CHECK_EQUAL(std::get<0>(record.at(1)), 2);
  BOOST_CHECK_EQUAL(std::get<1>(record.at(1)).unsafe_pointer_value, &b);
  BOOST_CHECK_EQUAL(std::get<0>(record.at(2)), 3);
  BOOST_CHECK_EQUAL(std::get<1>(record.at(2)).unsafe_pointer_value, &b);
}

BOOST_AUTO_TEST_CASE( mock_invocation_parametric_simple ) {
  typedef e::testing::parametric_invocation invocation_mock;

  invocation_mock record;

  BOOST_CHECK(not record.has_calls());
  BOOST_CHECK_EQUAL(record.call_count(), 0);

  int a = 0;
  int b_value = 3;
  int const & b = b_value;
  std::string c("abc");
  std::string d_value("dce");
  std::string const & d = c;

  record(a, b);
  record(a, b, c);
  record(a, b, c, d);

  BOOST_REQUIRE_EQUAL(record.call_count(), 3);
}

BOOST_AUTO_TEST_CASE( mock_invocation_parametric_no_copy ) {
  typedef e::testing::parametric_invocation invocation_mock;

  invocation_mock record;

  BOOST_CHECK(not record.has_calls());
  BOOST_CHECK_EQUAL(record.call_count(), 0);

  int a = 0;
  test_no_copy b_value(42);
  test_no_copy const & b = b_value;

  record(a);
  record(a, b);

  BOOST_REQUIRE_EQUAL(record.call_count(), 2);
}
