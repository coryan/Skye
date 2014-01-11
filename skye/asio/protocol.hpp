#ifndef skye_asio_protocol_hpp
#define skye_asio_protocol_hpp

namespace skye {
namespace asio {

class acceptor;
class mock_endpoint;
class resolver;
class socket;

struct protocol {
  typedef skye::asio::acceptor acceptor;
  typedef skye::asio::mock_endpoint endpoint;
  typedef skye::asio::resolver resolver;
  typedef skye::asio::socket socket;
};

} // namespace asio
} // namespace skye

#endif // skye_asio_protocol_hpp
