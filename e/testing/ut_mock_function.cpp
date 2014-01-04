#include <e/testing/mock_function.hpp>

#include <boost/test/unit_test.hpp>

using namespace e::testing;

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

};

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
  BOOST_REQUIRE_EQUAL(std::get<0>(function.at(0)), 0);
  BOOST_REQUIRE_EQUAL(std::get<1>(function.at(0)), 1);
  BOOST_REQUIRE_EQUAL(std::get<0>(function.at(1)), 1);
  BOOST_REQUIRE_EQUAL(std::get<1>(function.at(1)), 2);

  function.returns( []() { return 23; } );
  r = function(2, 3);
  BOOST_CHECK_EQUAL(r, 23);

  BOOST_CHECK(function.has_calls());
  BOOST_REQUIRE_EQUAL(function.call_count(), 3);
  BOOST_REQUIRE_EQUAL(std::get<0>(function.at(2)), 2);
  BOOST_REQUIRE_EQUAL(std::get<1>(function.at(2)), 3);
}
