#ifndef escapement_e_testing_mock_acceptor_hpp
#define escapement_e_testing_mock_acceptor_hpp

#include <e/testing/mock_service.hpp>

namespace e {
namespace testing {

class mock_acceptor : public mock_service {
 public:
  explicit mock_acceptor(boost::asio::io_service & io)
      : mock_service(io)
  {}
  virtual ~mock_acceptor() = 0;

  parametric_invocation async_accept;

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
