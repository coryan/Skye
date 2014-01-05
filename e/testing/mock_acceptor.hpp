#ifndef escapement_e_testing_mock_acceptor_hpp
#define escapement_e_testing_mock_acceptor_hpp

#include <e/testing/mock_async_accept_member_function.hpp>
#include <e/testing/mock_service.hpp>

namespace e {
namespace testing {

class mock_acceptor : public mock_service {
 public:
  typedef std::string endpoint_type;

  explicit mock_acceptor(boost::asio::io_service & io)
      : mock_service(io)
  {
    constructor_mock(this, &io);
  }
  virtual ~mock_acceptor() = 0;

  static mock_function<
    void(mock_acceptor*,boost::asio::io_service*)> constructor_mock;

  mock_async_accept_member_function async_accept;

  mock_function<std::string()> local_endpoint;
};

} // namespace testing
} // namespace e

#endif // escapement_e_testing_mock_acceptor_hpp
