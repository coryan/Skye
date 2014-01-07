#include <e/mocking/common/detail/unknown_arguments_capture_by_value.hpp>

#include <boost/test/unit_test.hpp>

using namespace e::mocking::common::detail;

BOOST_AUTO_TEST_CASE( test_unknown_arguments_capture_by_value ) {
  std::string a("a"), b("b"), c("c"), d("d");

  auto c1 = unknown_arguments_capture_by_value::capture(1, 2, a, b);
  BOOST_CHECK_EQUAL(c1->argument_count(), 4);
  std::ostringstream os;
  c1->stream(os);
  BOOST_CHECK_EQUAL(os.str(), "<1,2,a,b>");

  auto c3 = unknown_arguments_capture_by_value::capture(a, b, c);
  BOOST_CHECK_EQUAL(c3->argument_count(), 3);
  os.str("");
  os.clear();
  c3->stream(os);
  BOOST_CHECK_EQUAL(os.str(), "<a,b,c>");
  auto c4 = unknown_arguments_capture_by_value::capture(a, b, c);
  BOOST_CHECK(c3->equals(c4));
  auto c5 = unknown_arguments_capture_by_value::capture(b, c, d);
  BOOST_CHECK(not c3->equals(c5));

  c4 = unknown_arguments_capture_by_value::capture(a, b, c, 1, 2, 3, 4);
  BOOST_CHECK_EQUAL(c4->argument_count(), 7);
}
