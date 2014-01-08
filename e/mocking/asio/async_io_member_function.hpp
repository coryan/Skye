#ifndef escapement_e_mocking_asio_async_io_member_function_hpp
#define escapement_e_mocking_asio_async_io_member_function_hpp

#include <e/mocking/asio/detail/async_function_argument_capture.hpp>
#include <e/mocking/common/mock_template_function.hpp>

#include <boost/system/error_code.hpp>

namespace e {
namespace mocking {
namespace asio {

typedef detail::async_function_argument_capture<
  void(boost::system::error_code const &,std::size_t)> async_io_capture;

typedef e::mocking::common::mock_template_function<
  void, async_io_capture> async_read_member_function;

typedef e::mocking::common::mock_template_function<
  void, async_io_capture> async_write_member_function;

} // namespace asio
} // namespace mocking
} // namespace e

#endif // escapement_e_mocking_asio_async_io_member_function_hpp
