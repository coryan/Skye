#ifndef escapement_e_mocking_asio_async_io_member_function_hpp
#define escapement_e_mocking_asio_async_io_member_function_hpp

#include <e/mocking/asio/async_io_traits.hpp>
#include <e/mocking/asio/async_function.hpp>

#include <vector>

namespace e {
namespace mocking {
namespace asio {

typedef async_function<async_read_traits> async_read_member_function;

typedef async_function<async_write_traits> async_write_member_function;

} // namespace asio
} // namespace mocking
} // namespace e

#endif // escapement_e_mocking_asio_async_io_member_function_hpp
