#include <e/mocking/asio/service.hpp>

namespace {
/// A pre-defined io service to make implementation of mock services easier.
boost::asio::io_service io_internal;
} // anonymous namespace

namespace e {
namespace mocking {
namespace asio {

service::service()
    : boost::asio::io_service::service(io_internal)
{
}

boost::asio::io_service & service::io_service_for_testing() {
  return io_internal;
}

void service::fork_service(
    boost::asio::io_service::fork_event event)
{
  fork_service_capture(event);
}

void service::shutdown_service()
{
  shutdown_service_capture();
}

} // namespace asio
} // namespace mocking
} // namespace e
