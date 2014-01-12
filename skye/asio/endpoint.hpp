#ifndef skye_asio_endpoint_hpp
#define skye_asio_endpoint_hpp

#include <iostream>

namespace skye {
namespace asio {

struct protocol;

/**
 * Define the endpoint class for the Boost.ASIO mocks.
 */
class mock_endpoint {
 public:
  typedef protocol protocol_type;

  mock_endpoint() = default;
  explicit mock_endpoint(std::string const & value)
      : value_(value)
  {}

  std::string const & value() const {
    return value_;
  }
  mock_endpoint const & endpoint() const {
    return *this;
  }
  bool operator==(mock_endpoint const & rhs) const {
    return value_ == rhs.value_;
  }
  bool operator<(mock_endpoint const & rhs) const {
    return value_ < rhs.value_;
  }

 private:
  std::string value_;
};

std::ostream & operator<<(std::ostream & os, mock_endpoint const & x) {
  return os << "mock_endpoint(" << x.value() << ")";
}

} // namespace asio
} // namespace skye

#endif // skye_asio_service_hpp
