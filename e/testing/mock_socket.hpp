#ifndef escapement_e_testing_mock_socket_hpp
#define escapement_e_testing_mock_socket_hpp

#include <e/testing/mock_service.hpp>
#include <e/testing/mock_async_io_member_function.hpp>

namespace e {
namespace testing {

class mock_socket : public mock_service {
 public:
  explicit mock_socket(boost::asio::io_service & io)
      : mock_service(io)
  {}
  virtual ~mock_socket() = 0;

  e::testing::mock_async_write_member_function async_write_some;
  e::testing::mock_async_read_member_function async_read_some;

  //@{
  /**
   * @name Guidelines for the Turtle mocking framework
   */
  virtual void close() = 0;
  virtual void close(boost::system::error_code & ec) = 0;
  virtual void cancel() = 0;
  //@}
};

} // namespace testing
} // namespace e

#endif // escapement_e_testing_mock_socket_hpp
