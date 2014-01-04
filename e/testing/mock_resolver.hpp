#ifndef escapement_e_testing_mock_resolver_hpp
#define escapement_e_testing_mock_resolver_hpp

#include <e/testing/mock_iterator.hpp>
#include <e/testing/mock_function.hpp>
#include <boost/asio/io_service.hpp>

#include <vector>

namespace e {
namespace testing {

/**
 * Base class for mocks that implement some boost::asio Resolver interface.
 */
class mock_resolver : public mock_service {
 public:
  static boost::asio::io_service::id id;
  explicit mock_resolver(boost::asio::io_service & io)
      : mock_service(io)
  {}

  typedef mock_iterator iterator;
  typedef mock_iterator::resolver_result resolver_result;

  mock_function<mock_iterator(std::string const&)> resolve;
};

} // namespace testing
} // namespace e

#endif // escapement_e_testing_mock_service_hpp
