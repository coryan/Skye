#include <skye/asio/iterator.hpp>

#include <boost/test/unit_test.hpp>

using namespace skye::asio;

BOOST_AUTO_TEST_CASE( mock_iterator_create ) {
  iterator::resolver_result result;
  result.push_back(mock_endpoint("abc"));
  result.push_back(mock_endpoint("bcd"));
  result.push_back(mock_endpoint("cde"));
  result.push_back(mock_endpoint("def"));

  iterator end;
  BOOST_CHECK(end == iterator());

  iterator i = iterator::create(result);
  BOOST_CHECK(i != end);

  std::string all;
  int counter = 0;
  for (; i != iterator(); ++i) {
    BOOST_CHECK_EQUAL(i->endpoint().value(), result.at(counter).value());
    ++counter;
    all += " ";
    all += i->endpoint().value();
  }
  BOOST_CHECK_EQUAL(all, " abc bcd cde def");
}
