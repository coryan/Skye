#include <e/testing/mock_acceptor.hpp>

e::testing::invocation<e::testing::mock_acceptor*,boost::asio::io_service*>
e::testing::mock_acceptor::constructor_mock;

e::testing::mock_acceptor::~mock_acceptor()
{}
