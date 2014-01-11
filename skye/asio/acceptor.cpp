#include <skye/asio/acceptor.hpp>

namespace skye {
namespace asio {

skye::mock_function<void(acceptor*,boost::asio::io_service*)>
acceptor::constructor_mock;

acceptor::~acceptor()
{}

} // namespace asio
} // namespace skye
