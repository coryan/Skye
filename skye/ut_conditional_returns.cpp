#include <skye/mock_function.hpp>

#include <boost/test/unit_test.hpp>

/// @test Verify that conditional returns work as expected for mock functions.
BOOST_AUTO_TEST_CASE(conditional_returns) {
  typedef skye::mock_function<int(int)> mock_type;

  mock_type mock_function;

  mock_function.returns( 42 );
  int actual = mock_function(7);

  BOOST_CHECK_EQUAL(actual, 42);

  mock_function.clear_returns();
  BOOST_CHECK_THROW(mock_function(7), std::exception);

  mock_function.when( 7 ).returns( 42 );
  mock_function.when( 6 ).returns( 41 );
  BOOST_CHECK_EQUAL(mock_function(7), 42);
  BOOST_CHECK_EQUAL(mock_function(6), 41);
  BOOST_CHECK_THROW(mock_function(9), std::exception);

  mock_function.clear_returns();
  mock_function.returns( 42 );
  BOOST_CHECK_EQUAL(mock_function(6), 42);
}
