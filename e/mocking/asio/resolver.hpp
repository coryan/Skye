#ifndef escapement_e_mocking_asio_resolver_hpp
#define escapement_e_mocking_asio_resolver_hpp

#include <e/mocking/asio/iterator.hpp>
#include <e/testing/mock_function.hpp>
#include <boost/asio/io_service.hpp>

#include <vector>

namespace e {
namespace mocking {
namespace asio {

/**
 * Base class for mocks that implement some boost::asio Resolver interface.
 */
class resolver : public service {
 public:
  static boost::asio::io_service::id id;
  explicit resolver(boost::asio::io_service & io)
      : service(io)
  {}

  typedef e::mocking::asio::iterator iterator;
  typedef iterator::resolver_result resolver_result;

  e::testing::mock_function<iterator(std::string const&)> resolve;
};

} // namespace asio
} // namespace mocking
} // namespace e

#endif // escapement_e_mocking_asio_service_hpp
