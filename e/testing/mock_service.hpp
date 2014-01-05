#ifndef escapement_e_testing_mock_service_hpp
#define escapement_e_testing_mock_service_hpp

#include <e/testing/mock_function.hpp>
#include <boost/asio/io_service.hpp>

namespace e {
namespace testing {

/**
 * Base class for mocks that implement some boost::asio Service interface.
 *
 * Mocking classes used in boost::asio can be complicated because the
 * interfaces are fairly complex and very precisely defined.  This
 * class is a start to refactor some of the code used in creating such
 * interfaces.
 */
class mock_service : public boost::asio::io_service::service {
 public:
  static boost::asio::io_service::id id;
  explicit mock_service(boost::asio::io_service & io)
      : boost::asio::io_service::service(io)
  {}

  mock_function<void(boost::asio::io_service::fork_event)> fork_service_capture;
  mock_function<void()> shutdown_service_capture;

 private:
  virtual void fork_service(
      boost::asio::io_service::fork_event event);
  virtual void shutdown_service();
};

} // namespace testing
} // namespace e

#endif // escapement_e_testing_mock_service_hpp
