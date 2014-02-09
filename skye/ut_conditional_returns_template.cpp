#include <skye/mock_template_function.hpp>

#include <boost/test/unit_test.hpp>

/// @test Verify that conditional returns work as expected for mock templates.
BOOST_AUTO_TEST_CASE(template_conditional_returns) {
  typedef skye::mock_template_function<int> mock_type;

  mock_type mock_function;

  mock_function.returns( 42 );
  int actual = mock_function("foo");

  BOOST_CHECK_EQUAL(actual, 42);

  mock_function.clear();
  BOOST_CHECK_THROW(mock_function(7), std::exception);

  mock_function.when( 7 ).returns( 42 );
  mock_function.when( std::string("hi"), 6 ).returns( 43 );
  BOOST_CHECK_EQUAL(mock_function(7), 42);
  BOOST_CHECK_EQUAL(mock_function(std::string("hi"), 6), 43);
  BOOST_CHECK_THROW(mock_function(), std::exception);

  mock_function.clear_returns();
  mock_function.returns( 43 );
  BOOST_CHECK_EQUAL(mock_function(7), 43);
}
