#include <skye/asio/acceptor.hpp>

namespace e {
namespace mocking {
namespace asio {

e::mocking::common::mock_function<void(acceptor*,boost::asio::io_service*)>
acceptor::constructor_mock;

acceptor::~acceptor()
{}

} // namespace asio
} // namespace mocking
} // namespace e
