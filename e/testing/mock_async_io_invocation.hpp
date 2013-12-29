#ifndef escapement_e_testing_mock_async_io_invocation_hpp
#define escapement_e_testing_mock_async_io_invocation_hpp

#include <boost/asio/buffer.hpp>
#include <boost/system/error_code.hpp>
#include <memory>

namespace e {
namespace testing {

/**
 * Record a single invocation to a mock asynchronous member function.
 *
 * Please @see mock_async_io_member_function for details
 */
class mock_async_write_invocation_base {
 public:
  typedef std::shared_ptr<mock_async_write_invocation_base> pointer;
  virtual ~mock_async_write_invocation_base() = 0;

  virtual void call_handler(
      boost::system::error_code const & ec, std::size_t bt) = 0;
  virtual std::size_t get_size() const = 0;
  virtual void const * get_data() const = 0;
};

template<typename buffer_sequence, typename handler>
class mock_async_write_invocation : public mock_async_write_invocation_base {
 public:
  typedef mock_async_write_invocation_base::pointer base_pointer;
  mock_async_write_invocation() = delete;
  mock_async_write_invocation(mock_async_write_invocation const &) = delete;
  mock_async_write_invocation(
      buffer_sequence const & bs, handler h)
      : buffers_(bs)
      , handler_(h)
  {}
  virtual ~mock_async_write_invocation() {}

  virtual void call_handler(
      boost::system::error_code const & ec, std::size_t bt) {
    handler_(ec, bt);
  }
  virtual std::size_t get_size() const {
    return boost::asio::buffer_size(buffers_);
  }
  virtual void const * get_data() const {
    // TODO(coryan) this only works for simple tests with a single buffer.
    typedef typename buffer_sequence::value_type buffer_type;
    buffer_type const & b = *buffers_.begin();
    return boost::asio::buffer_cast<void const*>(b);
  }

 protected:
  buffer_sequence buffers_;
  handler handler_;
};

class mock_async_read_invocation_base {
 public:
  typedef std::shared_ptr<mock_async_read_invocation_base> pointer;
  virtual ~mock_async_read_invocation_base() = 0;

  virtual void call_handler(
      boost::system::error_code const & ec, std::size_t bt) = 0;
  virtual std::size_t get_size() const = 0;
  virtual void const * get_data() const = 0;
  virtual void set_data(void const * data, std::size_t size) = 0;
};

template<typename buffer_sequence, typename handler>
class mock_async_read_invocation
    : public mock_async_read_invocation_base {
 public:
  mock_async_read_invocation() = delete;
  mock_async_read_invocation(mock_async_read_invocation const &) = delete;
  mock_async_read_invocation(
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
    // TODO(coryan) this only works for simple tests with a single buffer.
    typedef typename buffer_sequence::value_type buffer_type;
    buffer_type const & b = *buffers_.begin();
    return boost::asio::buffer_cast<void*>(b);
  }
  virtual void set_data(void const * data, std::size_t size) {
    boost::asio::buffer_copy(
        buffers_, boost::asio::buffer(data, size));
  }

 protected:
  buffer_sequence buffers_;
  handler handler_;
};

struct mock_async_write_traits {
  typedef mock_async_write_invocation_base::pointer base_pointer;
  template<typename buffer_sequence, typename handler>
  static base_pointer create(
      buffer_sequence const & bs, handler h) {
    return base_pointer(
        new mock_async_write_invocation<buffer_sequence,handler>(bs, h));
  }
};

struct mock_async_read_traits {
  typedef mock_async_read_invocation_base::pointer base_pointer;
  template<typename buffer_sequence, typename handler>
  static base_pointer create(
      buffer_sequence const & bs, handler h) {
    return base_pointer(
        new mock_async_read_invocation<buffer_sequence,handler>(bs, h));
  }
};

} // namespace testing
} // namespace e

#endif // escapement_e_testing_mock_async_io_invocation_hpp
