#include <e/mocking/common/detail/argument_wrapper.hpp>
#include <e/mocking/common/detail/tuple_streaming.hpp>

#include <boost/test/unit_test.hpp>

using namespace e::mocking::common::detail;

template<typename... arg_types>
class known_arguments_capture_by_value {
 public:
  typedef decltype(wrap_args_as_tuple(std::declval<arg_types>()...)) value_type;

  static value_type capture(arg_types&&... args) {
    return wrap_args_as_tuple(args...);
  }
};

BOOST_AUTO_TEST_CASE( test_known_arguments_capture_by_value ) {
  typedef known_arguments_capture_by_value<
    int,std::string,std::string &> capture;

  std::string ref("foo");

  auto c1 = capture::capture(1, std::string("bar"), ref);
  BOOST_CHECK_EQUAL(std::get<2>(c1), "foo");
}
