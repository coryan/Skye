#ifndef escapement_e_testing_mock_socket_hpp
#define escapement_e_testing_mock_socket_hpp

#include <e/testing/mock_async_io_member_function.hpp>
#include <e/testing/mock_async_connect_member_function.hpp>
#include <e/testing/mock_function.hpp>
#include <e/testing/mock_service.hpp>
#include <e/testing/mock_protocol.hpp>

namespace e {
namespace testing {

class mock_socket : public mock_service {
 public:
  typedef mock_protocol protocol_type;

  explicit mock_socket(boost::asio::io_service & io)
      : mock_service(io)
  {}
  ~mock_socket() {}

  mutable e::testing::mock_async_write_member_function async_write_some;
  mutable e::testing::mock_async_read_member_function async_read_some;
  mutable e::testing::mock_async_connect_member_function async_connect;

  mutable e::testing::mock_function<bool()> is_open;
  mutable e::testing::mock_function<void()> cancel;

  // TODO(ES-26) With a lot of macro magic and Boost.Preprocessor
  // hacks we can have some macros generate the functions and variable
  // declarations.
  void close() {
    return close_capture_0();
  }
  mutable e::testing::mock_function<void()> close_capture_0;

  virtual void close(boost::system::error_code & ec) {
    return close_capture_1(ec);
  }
  mutable e::testing::mock_function<
    void(boost::system::error_code&)> close_capture_1;
};

} // namespace testing
} // namespace e

#endif // escapement_e_testing_mock_socket_hpp
