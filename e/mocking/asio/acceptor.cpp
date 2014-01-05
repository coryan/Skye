#include <e/mocking/asio/acceptor.hpp>

namespace e {
namespace mocking {
namespace asio {

testing::mock_function<void(acceptor*,boost::asio::io_service*)>
acceptor::constructor_mock;

acceptor::~acceptor()
{}

} // namespace asio
} // namespace mocking
} // namespace e
