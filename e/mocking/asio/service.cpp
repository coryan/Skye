#include <e/mocking/asio/service.hpp>

namespace e {
namespace mocking {
namespace asio {

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
