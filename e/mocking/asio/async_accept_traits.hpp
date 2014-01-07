#ifndef escapement_e_mocking_asio_async_accept_traits_hpp
#define escapement_e_mocking_asio_async_accept_traits_hpp

#include <e/mocking/common/detail/argument_wrapper.hpp>
#include <boost/system/error_code.hpp>
#include <memory>

namespace e {
namespace mocking {
namespace asio {

/**
 * Base class representing the arguments captured during a single
 * invocation to a mock asynchronous accept function.
 *
 * @see async_io_member_function for motivation and usage.
 */
class async_accept_arg_capture_base {
 public:
  typedef std::shared_ptr<async_accept_arg_capture_base> pointer;
  virtual ~async_accept_arg_capture_base() = 0;

  virtual void call_handler(
      boost::system::error_code const & ec) = 0;
  virtual std::size_t arg_count() const = 0;
};

/**
 * Concrete implementation class holding the arguments captured during
 * a single invocation to a mock asynchronous accept function.
 *
 * @see async_io_member_function for motivation and usage.
 */
template<typename handler, typename args_tuple_T>
class async_accept_arg_capture
    : public async_accept_arg_capture_base {
 public:
  typedef async_accept_arg_capture_base::pointer base_pointer;
  typedef args_tuple_T args_tuple;

  async_accept_arg_capture() = delete;
  async_accept_arg_capture(async_accept_arg_capture const &) = delete;
  async_accept_arg_capture(
      handler h, args_tuple && t)
      : handler_(h)
      , args_(t)
  {}
  virtual ~async_accept_arg_capture() {}

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
 * Define the traits to instantiate a async_accept_function
 */
struct async_accept_traits {
  typedef async_accept_arg_capture_base::pointer base_pointer;

  template<typename handler, typename... args>
  static base_pointer create(handler h, args&&... a) {
    auto t = e::mocking::common::detail::wrap_args_as_tuple(a...);
    return base_pointer(
        new async_accept_arg_capture<handler,decltype(t)>(
            h, std::move(t)));
  }
};


} // namespace asio
} // namespace mocking
} // namespace e

#endif // escapement_e_mocking_asio_async_accept_traits_hpp
