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
}
