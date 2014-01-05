#ifndef escapement_e_mocking_asio_protocol_hpp
#define escapement_e_mocking_asio_protocol_hpp

namespace e {
namespace mocking {
namespace asio {

class acceptor;
class mock_endpoint;
class resolver;
class socket;

struct protocol {
  typedef e::mocking::asio::acceptor acceptor;
  typedef e::mocking::asio::mock_endpoint endpoint;
  typedef e::mocking::asio::resolver resolver;
  typedef e::mocking::asio::socket socket;
};

} // namespace asio
} // namespace mocking
} // namespace e

#endif // escapement_e_mocking_asio_protocol_hpp
