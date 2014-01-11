#ifndef escapement_e_mocking_asio_async_accept_member_function_hpp
#define escapement_e_mocking_asio_async_accept_member_function_hpp

#include <skye/asio/detail/async_function_argument_capture.hpp>
#include <skye/common/mock_template_function.hpp>

#include <boost/system/error_code.hpp>

namespace e {
namespace mocking {
namespace asio {

typedef detail::async_function_argument_capture<
  void(boost::system::error_code const &)> async_accept_capture;
typedef e::mocking::common::mock_template_function<
  void, async_accept_capture> async_accept_member_function;

} // namespace asio
} // namespace mocking
} // namespace e

#endif // escapement_e_mocking_asio_async_accept_member_function_hpp
