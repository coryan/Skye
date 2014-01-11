#ifndef skye_asio_async_connect_member_function_hpp
#define skye_asio_async_connect_member_function_hpp

#include <skye/asio/detail/async_function_argument_capture.hpp>
#include <skye/mock_template_function.hpp>

#include <boost/system/error_code.hpp>

namespace skye {
namespace asio {

typedef detail::async_function_argument_capture<
  void(boost::system::error_code const &)> async_accept_capture;
typedef skye::mock_template_function<
  void, async_accept_capture> async_connect_member_function;

} // namespace asio
} // namespace skye

#endif // skye_asio_async_connect_member_function_hpp
