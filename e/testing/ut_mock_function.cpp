#include <e/testing/mock_function.hpp>
#include <e/testing/tuple_streaming.hpp>

#include <boost/test/unit_test.hpp>

using namespace e::testing;

BOOST_AUTO_TEST_CASE( mock_function_void ) {
  mock_function<void(int,int)> function;

  BOOST_CHECK(not function.has_calls());
  BOOST_CHECK_EQUAL(function.call_count(), 0);

  function(0, 1);
  function(1, 2);
  BOOST_CHECK(function.has_calls());
  BOOST_REQUIRE_EQUAL(function.call_count(), 2);
  BOOST_REQUIRE_EQUAL(std::get<0>(function.at(0)), 0);
  BOOST_REQUIRE_EQUAL(std::get<1>(function.at(0)), 1);
  BOOST_REQUIRE_EQUAL(std::get<0>(function.at(1)), 1);
  BOOST_REQUIRE_EQUAL(std::get<1>(function.at(1)), 2);
}

BOOST_AUTO_TEST_CASE( mock_function_int ) {
  mock_function<int(int,int)> function;

  BOOST_CHECK(not function.has_calls());
  BOOST_CHECK_EQUAL(function.call_count(), 0);

  BOOST_CHECK_THROW(function(0, 1), std::runtime_error);

  function.returns( 42 );
  int r = function(1, 2);
  BOOST_CHECK_EQUAL(r, 42);

  BOOST_CHECK(function.has_calls());
  BOOST_REQUIRE_EQUAL(function.call_count(), 2);
  BOOST_CHECK_EQUAL(std::get<0>(function.at(0)), 0);
  BOOST_CHECK_EQUAL(std::get<1>(function.at(0)), 1);
  BOOST_CHECK_EQUAL(std::get<0>(function.at(1)), 1);
  BOOST_CHECK_EQUAL(std::get<1>(function.at(1)), 2);

  function.returns( []() { return 23; } );
  r = function(2, 3);
  BOOST_CHECK_EQUAL(r, 23);

  BOOST_CHECK(function.has_calls());
  BOOST_REQUIRE_EQUAL(function.call_count(), 3);
  BOOST_CHECK_EQUAL(std::get<0>(function.at(2)), 2);
  BOOST_CHECK_EQUAL(std::get<1>(function.at(2)), 3);
}

BOOST_AUTO_TEST_CASE( mock_function_reference_args ) {
  mock_function<int(std::string const &,std::string &)> function;

  BOOST_CHECK(not function.has_calls());

  function.returns( 42 );
  std::string const x("this is a string");
  std::string y("this is another string");
  int r = function(x, y);
  BOOST_CHECK_EQUAL(r, 42);

  BOOST_REQUIRE_EQUAL(function.call_count(), 1);
  BOOST_CHECK_EQUAL(std::get<0>(function.at(0)), "this is a string");
  BOOST_CHECK_EQUAL(std::get<1>(function.at(0)), "this is another string");
}

BOOST_AUTO_TEST_CASE( mock_function_return_by_value ) {
  mock_function<std::string(int, char const*)> function;

  BOOST_CHECK(not function.has_calls());

  function.returns( std::string("42") );
  char const arg[] = "test array";
  std::string r = function(sizeof(arg), arg);
  BOOST_CHECK_EQUAL(r, "42");

  BOOST_REQUIRE_EQUAL(function.call_count(), 1);
  BOOST_CHECK_EQUAL(std::get<0>(function.at(0)), sizeof(arg));
  BOOST_CHECK_EQUAL(std::get<1>(function.at(0)), arg);
}

BOOST_AUTO_TEST_CASE( mock_function_return_by_reference ) {
  mock_function<std::string const &(int, char const*)> function;

  BOOST_CHECK(not function.has_calls());

  char const arg[] = "test array";
  std::string expected("42");

  function.returns( std::string("42") );
  std::string const & actual1 = function(sizeof(arg), arg);
  BOOST_CHECK_EQUAL(actual1, expected);

  function.returns( [&expected]() -> std::string const& { return expected; } );
  std::string const & actual2 = function(sizeof(arg), arg);
  BOOST_CHECK_EQUAL(actual2, expected);

  BOOST_REQUIRE_EQUAL(function.call_count(), 2);
  BOOST_CHECK_EQUAL(std::get<0>(function.at(0)), sizeof(arg));
  BOOST_CHECK_EQUAL(std::get<1>(function.at(0)), arg);
  BOOST_CHECK_EQUAL(std::get<0>(function.at(1)), sizeof(arg));
  BOOST_CHECK_EQUAL(std::get<1>(function.at(1)), arg);
}
/**
 * Provide simple classes and functions to exercise mock_function.
 */
namespace {

template<typename T>
std::string use_mock(T & mock) {
  std::string x("this is a string");
  std::string y("this is another string");
  (void) mock.f1(x, y);
  mock.f2(3, 4, 5);

  return mock.f3(42, "hello");
}

class base {
 public:
  virtual ~base() {}

  virtual std::string f3(int x, char const * y) = 0;
};

class my_mock_class : public base {
 public:
  my_mock_class() {}
  virtual ~my_mock_class() {}

  mock_function<int(std::string const&, std::string&)> f1;
  mock_function<void(int,int,int)> f2;

  virtual std::string f3(int x, char const * y) {
    return f3_capture(x, y);
  }
  mock_function<std::string(int,char const*)> f3_capture;
};

template<typename used_type>
void static_polymorphism_use(used_type & object) {
  std::string non_const("y");
  object.f1(std::string("x"), non_const);
  object.f2(1, 2, 3);
  object.f3(5, "1234");
}

void dynamic_polymorphism_use(base & object) {
  object.f3(7, "123456");
}

} // anonymous namespace

BOOST_AUTO_TEST_CASE( mock_function_member_functions ) {
  my_mock_class object;
  object.f1.returns( 42 );
  object.f3_capture.returns( std::string("42") );

  static_polymorphism_use(object);
  BOOST_REQUIRE_EQUAL(object.f1.call_count(), 1);
  BOOST_REQUIRE_EQUAL(
      object.f1.at(0), std::make_tuple(std::string("x"), std::string("y")));

  BOOST_REQUIRE_EQUAL(object.f2.call_count(), 1);
  BOOST_REQUIRE_EQUAL(
      object.f2.at(0), std::make_tuple(1, 2, 3));

  BOOST_REQUIRE_EQUAL(object.f3_capture.call_count(), 1);
  BOOST_REQUIRE_EQUAL(
      std::get<0>(object.f3_capture.at(0)), 5);

  dynamic_polymorphism_use(object);

  BOOST_REQUIRE_EQUAL(object.f3_capture.call_count(), 2);
  BOOST_REQUIRE_EQUAL(
      std::get<0>(object.f3_capture.at(1)), 7);
}

