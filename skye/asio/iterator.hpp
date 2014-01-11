#ifndef escapement_e_mocking_asio_iterator_hpp
#define escapement_e_mocking_asio_iterator_hpp

#include <e/mocking/asio/endpoint.hpp>
#include <memory>
#include <vector>

namespace e {
namespace mocking {
namespace asio {

/**
 * Define the iterator class for the Boost.ASIO mocks.
 */
class iterator {
 public:
  typedef std::vector<mock_endpoint> resolver_result;
  typedef mock_endpoint value_type;
  typedef value_type * pointer;
  typedef value_type & reference;
  typedef std::ptrdiff_t difference_type;

  
  iterator() 
      : shared_()
      , i_()
  {}
  static iterator create(resolver_result && r) {
    return iterator(r);
  }
  static iterator create(resolver_result const & r) {
    return iterator(r);
  }

  value_type const & operator*() const {
    return *i_;
  }
  value_type const * operator->() const {
    return i_.operator->();
  }
  iterator & operator++() {
    increment();
    return *this;
  }
  iterator operator++(int) {
    iterator tmp(*this);
    ++*this;
    return *this;
  }

  bool operator==(iterator const & rhs) const {
    if (not shared_ && not rhs.shared_) {
      return true;
    }
    if (shared_ != rhs.shared_) {
      return false;
    }
    return i_ == rhs.i_;
  }
  bool operator!=(iterator const & rhs) const {
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
    }
  }

 private:
  explicit iterator(resolver_result && r)
      : shared_(new resolver_result(r))
      , i_(shared_->begin())
  {}
  explicit iterator(resolver_result const & r)
      : shared_(new resolver_result(r))
      , i_(shared_->begin())
  {}


 private:
  typedef std::shared_ptr<resolver_result> contents;
  typedef resolver_result::const_iterator contents_iterator;
  contents shared_;
  contents_iterator i_;
};

} // namespace asio
} // namespace mocking
} // namespace e

#endif // escapement_e_mocking_asio_service_hpp
