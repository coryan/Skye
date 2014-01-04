#include <e/testing/mock_iterator.hpp>

#include <boost/test/unit_test.hpp>

using namespace e::testing;

BOOST_AUTO_TEST_CASE( mock_iterator_create ) {
  mock_iterator::resolver_result result;
  result.push_back(mock_endpoint("abc"));
  result.push_back(mock_endpoint("bcd"));
  result.push_back(mock_endpoint("cde"));
  result.push_back(mock_endpoint("def"));

  mock_iterator end;
  BOOST_CHECK(end == mock_iterator());

  mock_iterator i = mock_iterator::create(result);
  BOOST_CHECK(i != end);

  std::string all;
  for (; i != mock_iterator(); ++i) {
    all += " ";
    all += i->endpoint().value();
  }
  BOOST_CHECK_EQUAL(all, " abc bcd cde def");
}
