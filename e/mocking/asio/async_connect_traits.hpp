#ifndef escapement_e_mocking_asio_async_connect_traits_hpp
#define escapement_e_mocking_asio_async_connect_traits_hpp

#include <e/testing/invocation_args_wrapper.hpp>
#include <boost/system/error_code.hpp>
#include <memory>

namespace e {
namespace mocking {
namespace asio {

/**
 * Base class representing the arguments captured during a single
 * invocation to a mock asynchronous connect function.
 *
 * @see async_io_member_function for motivation and usage.
 */
class async_connect_arg_capture_base {
 public:
  typedef std::shared_ptr<async_connect_arg_capture_base> pointer;
  virtual ~async_connect_arg_capture_base() = 0;

  virtual void call_handler(
      boost::system::error_code const & ec) = 0;
  virtual std::size_t arg_count() const = 0;
};

/**
 * Concrete implementation class holding the arguments captured during
 * a single invocation to a mock asynchronous connect function.
 *
 * @see async_io_member_function for motivation and usage.
 */
template<typename handler, typename args_tuple_T>
class async_connect_arg_capture
    : public async_connect_arg_capture_base {
 public:
  typedef async_connect_arg_capture_base::pointer base_pointer;
  typedef args_tuple_T args_tuple;

  async_connect_arg_capture() = delete;
  async_connect_arg_capture(
      async_connect_arg_capture const &) = delete;
  async_connect_arg_capture(
      handler h, args_tuple && t)
      : handler_(h)
      , args_(t)
  {}
  virtual ~async_connect_arg_capture() {}

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
 * Define the traits to instantiate a async_connect_function
 */
struct async_connect_traits {
  typedef async_connect_arg_capture_base::pointer base_pointer;

  template<typename handler, typename... args>
  static base_pointer create(handler h, args&&... a) {
    auto t = e::testing::wrap_args_as_tuple(a...);
    return base_pointer(
        new async_connect_arg_capture<handler,decltype(t)>(
            h, std::move(t)));
  }
};


} // namespace asio
} // namespace mocking
} // namespace e

#endif // escapement_e_mocking_asio_async_connect_traits_hpp
