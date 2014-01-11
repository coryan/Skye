#include <skye/asio/detail/async_function_argument_capture.hpp>
#include <skye/common/detail/tuple_streaming.hpp>

#include <boost/test/unit_test.hpp>

using namespace skye::asio::detail;

/// Simple types to exercise the code
namespace {

/// Define a trivial functio
void do_stuff(int x, int y) {}

} // anonymous namespace

BOOST_AUTO_TEST_CASE( test_async_function_argument_capture ) {
  typedef async_function_argument_capture<void(int,int)> capture_strategy;

  std::string a("a"), b("b"), c("c"), d("d");
  auto c1 = capture_strategy::capture(
      a, b, [](int const & x, int const & y) { do_stuff(x, y); });
  BOOST_CHECK_EQUAL(c1->argument_count(), 3);

  int x = 0;
  auto c2 = capture_strategy::capture(
      a, b, c, [&x](int a, int b) { x = a + b; });
  BOOST_CHECK_EQUAL(c2->argument_count(), 4);

  c2->call_functor(1, 2);
  BOOST_CHECK_EQUAL(x, 3);
}
