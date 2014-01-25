#ifndef skye_asio_protocol_hpp
#define skye_asio_protocol_hpp

#include <skye/asio/acceptor.hpp>
#include <skye/asio/endpoint.hpp>
#include <skye/asio/resolver.hpp>
#include <skye/asio/socket.hpp>

namespace skye {
namespace asio {

/**
 * Define related types in the mock implementations of Boost.ASIO.
 */
struct protocol {
  typedef ::skye::asio::acceptor acceptor;
  typedef ::skye::asio::mock_endpoint endpoint;
  typedef ::skye::asio::resolver resolver;
  typedef ::skye::asio::socket socket;
};

} // namespace asio
} // namespace skye

#endif // skye_asio_protocol_hpp
