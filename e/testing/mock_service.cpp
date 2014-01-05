#include <e/testing/mock_service.hpp>

void e::testing::mock_service::fork_service(
    boost::asio::io_service::fork_event event)
{
  fork_service_capture(event);
}

void e::testing::mock_service::shutdown_service()
{
  shutdown_service_capture();
}
