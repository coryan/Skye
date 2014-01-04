#ifndef escapement_e_testing_mock_protocol_hpp
#define escapement_e_testing_mock_protocol_hpp

namespace e {
namespace testing {

class mock_acceptor;
class mock_endpoint;
class mock_resolver;
class mock_socket;

struct mock_protocol {
  typedef mock_acceptor acceptor;
  typedef mock_endpoint endpoint;
  typedef mock_resolver resolver;
  typedef mock_socket socket;
};

} // namespace testing
} // namespace e

#endif // escapement_e_testing_mock_protocol_hpp
