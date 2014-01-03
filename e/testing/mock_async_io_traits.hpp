#ifndef escapement_e_testing_mock_async_io_traits_hpp
#define escapement_e_testing_mock_async_io_traits_hpp

#include <boost/asio/buffer.hpp>
#include <boost/system/error_code.hpp>
#include <memory>

namespace e {
namespace testing {

/**
 * Base class representing the arguments captured during a single
 * invocation to a mock asynchronous write function.
 *
 * @see mock_async_io_member_function for motivation and usage.
 */
class mock_async_write_arg_capture_base {
 public:
  typedef std::shared_ptr<mock_async_write_arg_capture_base> pointer;
  virtual ~mock_async_write_arg_capture_base() = 0;

  virtual void call_handler(
      boost::system::error_code const & ec, std::size_t bt) = 0;
  virtual std::size_t get_size() const = 0;
  virtual void const * get_data() const = 0;
};

/**
 * Concrete implementation class holding the arguments captured during
 * a single invocation to a mock asynchronous write function.
 *
 * @see mock_async_io_member_function for motivation and usage.
 */
template<typename buffer_sequence, typename handler>
class mock_async_write_arg_capture : public mock_async_write_arg_capture_base {
 public:
  typedef mock_async_write_arg_capture_base::pointer base_pointer;
  mock_async_write_arg_capture() = delete;
  mock_async_write_arg_capture(mock_async_write_arg_capture const &) = delete;
  mock_async_write_arg_capture(
      buffer_sequence const & bs, handler h)
      : buffers_(bs)
      , handler_(h)
  {}
  virtual ~mock_async_write_arg_capture() {}

  virtual void call_handler(
      boost::system::error_code const & ec, std::size_t bt) {
    handler_(ec, bt);
  }
  virtual std::size_t get_size() const {
    return boost::asio::buffer_size(buffers_);
  }
  virtual void const * get_data() const {
    // TODO(ES-27)
    typedef typename buffer_sequence::value_type buffer_type;
    buffer_type const & b = *buffers_.begin();
    return boost::asio::buffer_cast<void const*>(b);
  }

 private:
  buffer_sequence buffers_;
  handler handler_;
};

/**
 * Base class representing the arguments captured during a single
 * invocation to a mock asynchronous read member function.
 *
 * @see mock_async_io_member_function for motivation and usage.
 */
class mock_async_read_arg_capture_base {
 public:
  typedef std::shared_ptr<mock_async_read_arg_capture_base> pointer;
  virtual ~mock_async_read_arg_capture_base() = 0;

  virtual void call_handler(
      boost::system::error_code const & ec, std::size_t bt) = 0;
  virtual std::size_t get_size() const = 0;
  virtual void const * get_data() const = 0;
  virtual void set_data(void const * data, std::size_t size) = 0;
};

/**
 * Concrete implementation class holding the arguments captured during
 * a single invocation to a mock asynchronous read function.
 *
 * @see mock_async_io_member_function for motivation and usage.
 */
template<typename buffer_sequence, typename handler>
class mock_async_read_arg_capture
    : public mock_async_read_arg_capture_base {
 public:
  mock_async_read_arg_capture() = delete;
  mock_async_read_arg_capture(mock_async_read_arg_capture const &) = delete;
  mock_async_read_arg_capture(
      buffer_sequence const & bs, handler h)
      : buffers_(bs)
      , handler_(h)
  {}

  virtual void call_handler(
      boost::system::error_code const & ec, std::size_t bt) {
    handler_(ec, bt);
  }
  virtual std::size_t get_size() const {
    return boost::asio::buffer_size(buffers_);
  }
  virtual void const * get_data() const {
    // TODO(ES-27)
    typedef typename buffer_sequence::value_type buffer_type;
    buffer_type const & b = *buffers_.begin();
    return boost::asio::buffer_cast<void*>(b);
  }
  virtual void set_data(void const * data, std::size_t size) {
    boost::asio::buffer_copy(
        buffers_, boost::asio::buffer(data, size));
  }

 private:
  buffer_sequence buffers_;
  handler handler_;
};

/**
 * Define the traits to instantiate a mock_async_write_function
 */
struct mock_async_write_traits {
  typedef mock_async_write_arg_capture_base::pointer base_pointer;

  template<typename handler, typename buffer_sequence>
  static base_pointer create(
      handler h, buffer_sequence && bs) {
    return base_pointer(
        new mock_async_write_arg_capture<buffer_sequence,handler>(
            std::forward<buffer_sequence>(bs), h));
  }
};

/**
 * Define the traits to instantiate a mock_async_read_function
 */
struct mock_async_read_traits {
  typedef mock_async_read_arg_capture_base::pointer base_pointer;

  template<typename handler, typename buffer_sequence>
  static base_pointer create(
      handler h, buffer_sequence && bs) {
    return base_pointer(
        new mock_async_read_arg_capture<buffer_sequence,handler>(
            std::forward<buffer_sequence>(bs), h));
  }
};

} // namespace testing
} // namespace e

#endif // escapement_e_testing_mock_async_io_traits_hpp
