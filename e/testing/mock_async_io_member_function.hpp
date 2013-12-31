#ifndef escapement_e_testing_mock_async_io_member_function_hpp
#define escapement_e_testing_mock_async_io_member_function_hpp

#include <e/testing/mock_async_io_traits.hpp>
#include <e/testing/mock_async_function.hpp>

#include <vector>

namespace e {
namespace testing {

typedef mock_async_function<mock_async_read_traits>
  mock_async_read_member_function;
typedef mock_async_function<mock_async_write_traits>
  mock_async_write_member_function;

} // namespace testing
} // namespace e

#endif // escapement_e_testing_mock_async_io_member_function_hpp
