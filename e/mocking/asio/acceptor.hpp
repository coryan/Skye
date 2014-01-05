#ifndef escapement_e_mocking_asio_acceptor_hpp
#define escapement_e_mocking_asio_acceptor_hpp

#include <e/mocking/asio/async_accept_member_function.hpp>
#include <e/mocking/asio/service.hpp>

namespace e {
namespace mocking {
namespace asio {


class acceptor : public service {
 public:
  typedef std::string endpoint_type;

  explicit acceptor(boost::asio::io_service & io)
      : service(io)
  {
    constructor_mock(this, &io);
  }
  acceptor(boost::asio::io_service & io, std::string const & ep, bool)
      : service(io)
  {
    constructor_mock(this, &io);
    local_endpoint.returns( ep );
  }
  virtual ~acceptor();

  static e::testing::mock_function<
    void(acceptor*,boost::asio::io_service*)> constructor_mock;

  async_accept_member_function async_accept;

  e::testing::mock_function<std::string()> local_endpoint;
};

} // namespace asio
} // namespace mocking
} // namespace e

#endif // escapement_e_mocking_asio_acceptor_hpp
