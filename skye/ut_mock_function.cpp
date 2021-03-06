#include <skye/mock_function.hpp>
#include <skye/detail/tuple_streaming.hpp>

#include <boost/test/unit_test.hpp>

using namespace skye;

/// Helper objects and types for the test
namespace {
/**
 * Provide a global object to return by reference in
 * mock_function_return_by_reference.
 */
std::string global_string;
}

/**
 * @test Verify that functions returning void can be mocked.
 */
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

/**
 * @test Verify that functions returning int can be mocked.
 */
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

  function.action( []() { return 23; } );
  r = function(2, 3);
  BOOST_CHECK_EQUAL(r, 23);

  BOOST_CHECK(function.has_calls());
  BOOST_REQUIRE_EQUAL(function.call_count(), 3);
  BOOST_CHECK_EQUAL(std::get<0>(function.at(2)), 2);
  BOOST_CHECK_EQUAL(std::get<1>(function.at(2)), 3);
}

/**
 * @test Verify that functions with arguments passed by reference can be mocked.
 */
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

/**
 * @test Verify that functions returning user-defined types by value
 * can be mocked.
 */
BOOST_AUTO_TEST_CASE( mock_function_return_by_value ) {
  mock_function<std::string(std::size_t, char const*)> function;

  BOOST_CHECK(not function.has_calls());

  function.returns( std::string("42") );
  char const arg[] = "test array";
  std::size_t const argsize = sizeof(arg);
  std::string r = function(argsize, arg);
  BOOST_CHECK_EQUAL(r, "42");

  BOOST_REQUIRE_EQUAL(function.call_count(), 1);
  BOOST_CHECK_EQUAL(std::get<0>(function.at(0)), argsize);
  BOOST_CHECK_EQUAL(std::get<1>(function.at(0)), arg);
}

/**
 * @test Verify that functions returning user-defined types by reference
 * can be mocked.
 */
BOOST_AUTO_TEST_CASE( mock_function_return_by_reference ) {
  mock_function<std::string const &(std::size_t, char const*)> function;

  BOOST_CHECK(not function.has_calls());

  char const arg[] = "test array";
  std::string expected("42");

  global_string = expected;
  function.action( []() -> std::string const& { return global_string; } );
  std::string const & actual2 = function(sizeof(arg), arg);
  BOOST_CHECK_EQUAL(actual2, expected);

  BOOST_REQUIRE_EQUAL(function.call_count(), 1);
  BOOST_CHECK_EQUAL(std::get<0>(function.at(0)), sizeof(arg));
  BOOST_CHECK_EQUAL(std::get<1>(function.at(0)), arg);
}

/**
 * Provide simple classes and functions to exercise mock_function.
 */
namespace {

/// A class to use mocked classes using dynamic and static polymorphism.
template<typename T>
std::string use_mock(T & mock) {
  std::string x("this is a string");
  std::string y("this is another string");
  (void) mock.f1(x, y);
  mock.f2(3, 4, 5);

  return mock.f3(42, "hello");
}

/// The base class for the mocked objects.
class base {
 public:
  virtual ~base() {}

  virtual std::string f3(int x, char const * y) = 0;
};

/// Mock implementation of the base class.
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

/// Test use of mock objects through static polyphormism.
template<typename used_type>
void static_polymorphism_use(used_type & object) {
  std::string non_const("y");
  object.f1(std::string("x"), non_const);
  object.f2(1, 2, 3);
  object.f3(5, "1234");
}

/// Test use of mock objects through dynamic polymorphism.
void dynamic_polymorphism_use(base & object) {
  object.f3(7, "123456");
}

} // anonymous namespace

/**
 * @test Verify that member functions of different types can be mocked.
 */
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

/**
 * @test Verify that we can make assertions about functions called
 * never or once.
 */
BOOST_AUTO_TEST_CASE( mock_function_check_no_calls ) {
  mock_function<void(int)> function;

  function.check_called().never();
  function(42);
  function.check_called().once();
}

/**
 * @test Verify that several types of assertions work correctly.
 */
BOOST_AUTO_TEST_CASE( mock_function_asserts ) {
  mock_function<int(int,std::string)> function;

  function.returns( 7 );
  for (int i = 0; i != 2; ++i) {
    function( 42, std::string("foo"));
  }
  for (int i = 0; i != 9; ++i) {
    function( 7, std::string("bar"));
  }
  function( 7, std::string("foo"));

  function.check_called();
  function.check_called().at_least( 2 );
  function.check_called().at_least( 2 ).at_most( 30 );
  function.check_called().between( 3, 30 );
  function.check_called().exactly( 12 );

  function.check_called().at_least( 3 ).with( 7, std::string("bar" ));
  function.check_called().with( 7, std::string("bar" ));
}
