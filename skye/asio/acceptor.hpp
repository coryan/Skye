#ifndef skye_asio_acceptor_hpp
#define skye_asio_acceptor_hpp

#include <skye/asio/async_accept_member_function.hpp>
#include <skye/asio/service.hpp>

namespace skye {
namespace asio {

class acceptor : public service {
 public:
  typedef std::string endpoint_type;

  explicit acceptor(boost::asio::io_service & io)
      : service(io)
  {}
  acceptor(boost::asio::io_service & io, std::string const & ep, bool)
      : service(io)
  {}
  virtual ~acceptor() {
  }

  async_accept_member_function async_accept;

  skye::mock_function<std::string()> local_endpoint;
};

} // namespace asio
} // namespace skye

#endif // skye_asio_acceptor_hpp
