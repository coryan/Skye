#ifndef escapement_e_testing_mock_acceptor_hpp
#define escapement_e_testing_mock_acceptor_hpp

#include <e/testing/mock_async_accept_member_function.hpp>
#include <e/testing/mock_service.hpp>

namespace e {
namespace testing {

class mock_acceptor : public mock_service {
 public:
  explicit mock_acceptor(boost::asio::io_service & io)
      : mock_service(io)
  {
    constructor_mock(this, &io);
  }
  virtual ~mock_acceptor() = 0;

  static invocation<mock_acceptor*,boost::asio::io_service*> constructor_mock;

  mock_async_accept_member_function async_accept;

  //@{
  /**
   * @name Guidelines for the Turtle mocking framework
   */
  virtual std::string local_endpoint() = 0;
  //@}
};

} // namespace testing
} // namespace e

#endif // escapement_e_testing_mock_acceptor_hpp
