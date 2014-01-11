#include <skye/asio/service.hpp>
#include <memory>

/// Hide the objects from collisions with other namespaces / users.
namespace {

/// A pre-defined io service to make implementation of mock services easier.
std::unique_ptr<boost::asio::io_service> io_internal;

} // anonymous namespace

namespace e {
namespace mocking {
namespace asio {

service::service()
    : boost::asio::io_service::service(io_service_for_testing())
{
}

boost::asio::io_service & service::io_service_for_testing() {
  if (not io_internal) {
    return reset_io_service_for_testing();
  }
  return *io_internal;
}

boost::asio::io_service & service::reset_io_service_for_testing() {
  io_internal.reset(new boost::asio::io_service);
  return *io_internal;
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
