#ifndef escapement_e_testing_mock_async_accept_traits_hpp
#define escapement_e_testing_mock_async_accept_traits_hpp

#include <e/testing/invocation_args_wrapper.hpp>
#include <boost/system/error_code.hpp>
#include <memory>

namespace e {
namespace testing {

/**
 * Base class representing the arguments captured during a single
 * invocation to a mock asynchronous accept function.
 *
 * @see mock_async_io_member_function for motivation and usage.
 */
class mock_async_accept_arg_capture_base {
 public:
  typedef std::shared_ptr<mock_async_accept_arg_capture_base> pointer;
  virtual ~mock_async_accept_arg_capture_base() = 0;

  virtual void call_handler(
      boost::system::error_code const & ec) = 0;
  virtual std::size_t arg_count() const = 0;
};

/**
 * Concrete implementation class holding the arguments captured during
 * a single invocation to a mock asynchronous accept function.
 *
 * @see mock_async_io_member_function for motivation and usage.
 */
template<typename handler, typename args_tuple_T>
class mock_async_accept_arg_capture
    : public mock_async_accept_arg_capture_base {
 public:
  typedef mock_async_accept_arg_capture_base::pointer base_pointer;
  typedef args_tuple_T args_tuple;

  mock_async_accept_arg_capture() = delete;
  mock_async_accept_arg_capture(mock_async_accept_arg_capture const &) = delete;
  mock_async_accept_arg_capture(
      handler h, args_tuple && t)
      : handler_(h)
      , args_(t)
  {}
  virtual ~mock_async_accept_arg_capture() {}

  virtual void call_handler(
      boost::system::error_code const & ec) {
    handler_(ec);
  }
  virtual std::size_t arg_count() const {
    return 1 + std::tuple_size<args_tuple>::value;
  }

 private:        
  handler handler_;
  args_tuple args_;
};

/**
 * Define the traits to instantiate a mock_async_accept_function
 */
struct mock_async_accept_traits {
  typedef mock_async_accept_arg_capture_base::pointer base_pointer;

  template<typename handler, typename... args>
  static base_pointer create(handler h, args&&... a) {
    auto t = wrap_args_as_tuple(a...);
    return base_pointer(
        new mock_async_accept_arg_capture<handler,decltype(t)>(
            h, std::move(t)));
  }
};


} // namespace testing
} // namespace e

#endif // escapement_e_testing_mock_async_accept_traits_hpp
