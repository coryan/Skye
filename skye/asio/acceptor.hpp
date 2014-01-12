#ifndef skye_asio_acceptor_hpp
#define skye_asio_acceptor_hpp

#include <skye/asio/async_accept_member_function.hpp>
#include <skye/asio/service.hpp>
#include <skye/asio/endpoint.hpp>

namespace skye {
namespace asio {

class acceptor : public service {
 public:
  typedef mock_endpoint endpoint_type;

  explicit acceptor(boost::asio::io_service & io)
      : service(io)
  {}
  acceptor(
      boost::asio::io_service & io, endpoint_type & ep, bool)
      : service(io)
  {}
  virtual ~acceptor() {
  }

  async_accept_member_function async_accept;

  skye::mock_function<endpoint_type()> local_endpoint;
};

} // namespace asio
} // namespace skye

#endif // skye_asio_acceptor_hpp
