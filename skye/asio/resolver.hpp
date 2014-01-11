#ifndef skye_asio_resolver_hpp
#define skye_asio_resolver_hpp

#include <skye/asio/iterator.hpp>
#include <skye/mock_function.hpp>

#include <boost/asio/io_service.hpp>
#include <vector>

namespace skye {
namespace asio {

/**
 * Base class for mocks that implement some boost::asio Resolver interface.
 */
class resolver : public service {
 public:
  static boost::asio::io_servicid id;
  explicit resolver(boost::asio::io_service & io)
      : service(io)
  {}

  typedef skye::asio::iterator iterator;
  typedef iterator::resolver_result resolver_result;

  skye::mock_function<iterator(std::string const&)> resolve;
};

} // namespace asio
} // namespace skye

#endif // skye_asio_service_hpp
