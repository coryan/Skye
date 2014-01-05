#ifndef escapement_e_mocking_asio_service_hpp
#define escapement_e_mocking_asio_service_hpp

#include <e/mocking/common/mock_function.hpp>
#include <boost/asio/io_service.hpp>

namespace e {
namespace mocking {
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
  static boost::asio::io_service::id id;
  explicit service(boost::asio::io_service & io)
      : boost::asio::io_service::service(io)
  {}

  e::mocking::common::mock_function<
    void(boost::asio::io_service::fork_event)> fork_service_capture;
  e::mocking::common::mock_function<void()> shutdown_service_capture;

 private:
  virtual void fork_service(
      boost::asio::io_service::fork_event event);
  virtual void shutdown_service();
};

} // namespace asio
} // namespace mocking
} // namespace e

#endif // escapement_e_mocking_asio_service_hpp
