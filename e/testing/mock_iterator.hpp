#ifndef escapement_e_testing_mock_iterator_hpp
#define escapement_e_testing_mock_iterator_hpp

#include <e/testing/mock_endpoint.hpp>
#include <memory>
#include <vector>

namespace e {
namespace testing {

/**
 * Define the iterator class for the Boost.ASIO mocks.
 */
class mock_iterator {
 public:
  typedef std::vector<mock_endpoint> resolver_result;
  typedef mock_endpoint value_type;
  typedef value_type * pointer;
  typedef value_type & reference;
  typedef std::ptrdiff_t difference_type;

  
  mock_iterator() 
      : shared_()
      , i_()
  {}
  static mock_iterator create(resolver_result && r) {
    return mock_iterator(r);
  }
  static mock_iterator create(resolver_result const & r) {
    return mock_iterator(r);
  }

  value_type const & operator*() const {
    return *i_;
  }
  value_type const * operator->() const {
    return i_.operator->();
  }
  mock_iterator & operator++() {
    increment();
    return *this;
  }
  mock_iterator operator++(int) {
    mock_iterator tmp(*this);
    ++*this;
    return *this;
  }

  bool operator==(mock_iterator const & rhs) const {
    if (not shared_ && not rhs.shared_) {
      return true;
    }
    if (shared_ != rhs.shared_) {
      return false;
    }
    return i_ == rhs.i_;
  }
  bool operator!=(mock_iterator const & rhs) const {
    return !(*this == rhs);
  }

 private:
  void increment() {
    if (not shared_) {
      return;
    }
    ++i_;
    if (i_ == shared_->end()) {
      shared_.reset();
      i_ = shared_->end();
      return;
    }
  }

 private:
  explicit mock_iterator(resolver_result && r)
      : shared_(new resolver_result(r))
      , i_(shared_->begin())
  {}
  explicit mock_iterator(resolver_result const & r)
      : shared_(new resolver_result(r))
      , i_(shared_->begin())
  {}


 private:
  typedef std::shared_ptr<resolver_result> contents;
  typedef resolver_result::const_iterator iterator;
  contents shared_;
  iterator i_;
};

} // namespace testing
} // namespace e

#endif // escapement_e_testing_mock_service_hpp
