#ifndef escapement_e_testing_mock_async_io_member_function_hpp
#define escapement_e_testing_mock_async_io_member_function_hpp

#include <e/testing/mock_async_io_invocation.hpp>

#include <vector>

namespace e {
namespace testing {

/**
 * A class to mock member functions conforming to the boost::asio
 * async_{read,write}_some() interface.
 *
 * The Turtle mocking library does not seem able to support template
 * member functions, or if it does I (coryan@) cannot figure out how
 * to make it happen.  So, I am writing my own (hopefully modest,)
 * version.  The basic problem is that at least in principle each call
 * could have a different set of template parameters.
 *
 * This mocking class will conform to the "action -> assertion" model
 * of mocking, while Turtle tends to support "record -> replay".  I
 * find the latter very tedious.
 *
 * TODO(coryan): I expect this could grow into a real extension to the
 * Turtle mocking library, but I will write this on an as-needed
 * basis.
 */
template<typename traits>
class mock_async_io_member_function {
 public:
  typedef typename traits::base_pointer value_type;
  typedef std::vector<value_type> call_sequence;
  typedef typename call_sequence::const_iterator iterator;

  mock_async_io_member_function()
  {}

  template<typename buffer_sequence, typename handler>
  void operator()(buffer_sequence const & bs, handler h) {
    value_type invocation = traits::create(bs, h);
    calls_.push_back(invocation);
  }

  void clear() {
    calls_.clear();
  }

  bool has_calls() const {
    return not calls_.empty();
  }
  std::size_t call_count() const {
    return calls_.size();
  }
  value_type & at(int index) {
    return calls_.at(index);
  }
  value_type const & at(int index) const {
    return calls_.at(index);
  }


  iterator begin() const {
    return calls_.begin();
  }
  iterator end() const {
    return calls_.end();
  }

 private:
  call_sequence calls_;
};

typedef mock_async_io_member_function<mock_async_read_traits>
  mock_async_read_member_function;
typedef mock_async_io_member_function<mock_async_write_traits>
  mock_async_write_member_function;

} // namespace testing
} // namespace e

#endif // escapement_e_testing_mock_async_io_member_function_hpp
