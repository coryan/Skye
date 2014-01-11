#ifndef skye_asio_test_service_singleton_hpp
#define skye_asio_test_service_singleton_hpp

#include <boost/asio/io_service.hpp>

#include <memory>

namespace skye {
namespace asio {
namespace detail {

/**
 * Create a pre-defined io service to make implementation of mock
 * services easier, and allow tests to reset the io_service.
 */
template<bool unused>
class test_service_singleton {
 public:
  /// Returns the current instance, creates one if necessary.
  static boost::asio::io_service & instance() {
    if (not instance_) {
      return test_service_singleton::reset_instance();
    }
    return *instance_;
  }

  /// Creates a new instance and destroys the existing one
  static boost::asio::io_service & reset_instance() {
    instance_.reset(new boost::asio::io_service);
    return *instance_;
  }

  static std::unique_ptr<boost::asio::io_service> instance_;
};

template<bool unused>
std::unique_ptr<boost::asio::io_service>
test_service_singleton<unused>::instance_;

} // namespace detail
} // namespace asio
} // namespace skye

#endif // skye_asio_test_service_singleton_hpp

