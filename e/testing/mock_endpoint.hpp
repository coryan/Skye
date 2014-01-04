#ifndef escapement_e_testing_mock_endpoint_hpp
#define escapement_e_testing_mock_endpoint_hpp

#include <iostream>

namespace e {
namespace testing {

/**
 * Define the endpoint class for the Boost.ASIO mocks.
 */
class mock_endpoint {
 public:
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
  return os << "endpoint(" << x.value() << ")";
}

} // namespace testing
} // namespace e

#endif // escapement_e_testing_mock_service_hpp
