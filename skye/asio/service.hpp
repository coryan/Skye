#ifndef skye_asio_service_hpp
#define skye_asio_service_hpp

#include <skye/asio/detail/test_service_singleton.hpp>
#include <skye/mock_function.hpp>

#include <boost/asio/io_service.hpp>

namespace skye {
namespace asio {

/**
 * Base class for mocks that implement some boost::asio Service interface.
 *
 * Mocking classes used in boost::asio can be complicated because the
 * interfaces are fairly complex and very precisely defined.  This
 * class is a start to refactor some of the code used in creating such
 * interfaces.
 */
class service : public boost::asio::io_service::service {
 public:
  service()
      : boost::asio::io_service::service(
          detail::test_service_singleton<true>::instance())
  {}
  explicit service(boost::asio::io_service & io)
      : boost::asio::io_service::service(io)
  {}

  /// Returns the singleton io service for testing purposes
  static boost::asio::io_service & io_service_for_testing() {
    return detail::test_service_singleton<true>::instance();
  }

  /// Reset the singleton, usually called at the beginning of each test.
  static boost::asio::io_service & reset_io_service_for_testing() {
    return detail::test_service_singleton<true>::reset_instance();
  }

  skye::mock_function<
    void(boost::asio::io_service::fork_event)> fork_service_capture;
  skye::mock_function<void()> shutdown_service_capture;

 private:
  virtual void fork_service(
      boost::asio::io_service::fork_event event) override {
    fork_service_capture(event);
  }
  virtual void shutdown_service() override {
    shutdown_service_capture();
  }
};

} // namespace asio
} // namespace skye

#endif // skye_asio_service_hpp
