#ifndef escapement_e_mocking_asio_socket_hpp
#define escapement_e_mocking_asio_socket_hpp

#include <skye/asio/async_io_member_function.hpp>
#include <skye/asio/async_connect_member_function.hpp>
#include <skye/asio/service.hpp>
#include <skye/asio/protocol.hpp>
#include <skye/common/mock_function.hpp>

namespace e {
namespace mocking {
namespace asio {

class socket : public service {
 public:
  typedef protocol protocol_type;

  socket()
  {}
  explicit socket(boost::asio::io_service & io)
      : service(io)
  {}
  ~socket() {}

  mutable e::mocking::asio::async_write_member_function async_write_some;
  mutable e::mocking::asio::async_read_member_function async_read_some;
  mutable e::mocking::asio::async_connect_member_function async_connect;

  mutable e::mocking::common::mock_function<bool()> is_open;
  mutable e::mocking::common::mock_function<void()> cancel;

  // TODO(ES-26) With a lot of macro magic and Boost.Preprocessor
  // hacks we can have some macros generate the functions and variable
  // declarations.
  void close() {
    return close_capture_0();
  }
  mutable e::mocking::common::mock_function<void()> close_capture_0;

  virtual void close(boost::system::error_code & ec) {
    return close_capture_1(ec);
  }
  mutable e::mocking::common::mock_function<
    void(boost::system::error_code&)> close_capture_1;
};

} // namespace asio
} // namespace mocking
} // namespace e

#endif // escapement_e_mocking_asio_socket_hpp
