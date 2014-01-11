#ifndef skye_asio_detail_async_function_argument_capture_hpp
#define skye_asio_detail_async_function_argument_capture_hpp

#include <skye/common/detail/argument_wrapper.hpp>

#include <boost/asio/buffer.hpp>

#include <memory>
#include <stdexcept>

namespace skye {
namespace asio {
namespace detail {

/**
 * Unimplemented general version.
 *
 * Only the partial specialization for function types is expected to
 * be used.
 */
template<typename signature>
class async_function_argument_capture_holder;

/**
 * Unimplemented general version.
 *
 * Only the partial specialization for function types is expected to
 * be used.
 */
template<typename tuple_type, typename signature>
class async_function_argument_capture_tuple;

/**
 * Define the interface for classes that hold an async_* function
 * argument set.
 *
 * In Boost.ASIO most async_* functions receive a number of arguments
 * of varying types, but their last argument is always a functor-like
 * type with a known signature.  This class defines the interface used
 * when such an argument set is captured.
 */
template<typename return_type, typename... functor_args>
class async_function_argument_capture_holder<return_type(functor_args...)>
{
 public:
  /// These objects are held through a shared_ptr<> often, so define
  /// an typedef for it.
  typedef std::shared_ptr<async_function_argument_capture_holder> pointer;

  /// Destructor.
  virtual ~async_function_argument_capture_holder() {}

  /// Compare two holders for equality.
  virtual bool equals(pointer const & other) const = 0;

  /// Print the argument values using iostreams.
  virtual void stream(std::ostream & os) const = 0;

  /// Call (invoke) the captured functor using the argument provided.
  virtual return_type call_functor(functor_args... args) = 0;

  /// Return the number of arguments in the call.
  virtual std::size_t argument_count() const = 0;

  //@{
  /**
   * Buffer operations.
   *
   * Some async functions in Boost.ASIO have generic buffer objects as
   * the first arguments to the call.  These function allow access,
   * but raise an exception if the acess would not work (for example,
   * if the parameters do not match the necessary interface.)
   * Admittedly this is a bit of a hack, we should separate this to
   * its own strategy.
   * TODO(ES-50)
   */
  virtual std::size_t get_buffer_size() const = 0;
  virtual void const * get_buffer_data() const = 0;
  virtual void set_buffer_data(void const * data, std::size_t size) = 0;
  //@}
};

//@{
/**
 * @name SFINAE protected functions to get buffer properties.
 *
 * Use SFINAE either implement get_buffer_size(), get_buffer_data
 * and set_buffer_data() or raise exceptions if the argument does not
 * support these member functions.
 */
/// Get the buffer size.
template<typename wrapper>
auto safeish_get_buffer_size(wrapper const & w, bool)
    -> decltype(boost::asio::const_buffer(*w.value.begin()),
                boost::asio::buffer_size(w.value)) {
  return boost::asio::buffer_size(w.value);
}

/// Fallback case for get_buffer_size()
template<typename... arg_types>
std::size_t safeish_get_buffer_size(arg_types...) {
  throw std::runtime_error(
      "Captured argument does not seem to be an ASIO buffer");
}

/// Get the buffer size.
template<typename wrapper>
auto safeish_get_buffer_data(wrapper const & w, bool)
    -> decltype(
        boost::asio::const_buffer(*w.value.begin()),
        boost::asio::buffer_cast<void const*>(w.value)) {
  return boost::asio::buffer_cast<void const*>(w.value);
}

/// Fallback case for get_buffer_size()
template<typename... arg_types>
void const * safeish_get_buffer_data(arg_types...) {
  throw std::runtime_error(
      "Captured argument does not seem to be an ASIO buffer");
}

/// Get the buffer size.
template<typename wrapper, typename source>
auto safeish_set_buffer_data(wrapper & w, source const & b)
    -> decltype(boost::asio::mutable_buffer(*w.value.begin()), void()) {
  boost::asio::buffer_copy(w.value, b);
}

/// Fallback case for get_buffer_size()
template<typename... arg_types>
void safeish_set_buffer_data(arg_types...) {
  throw std::runtime_error(
      "Captured argument does not seem to be an ASIO buffer");
}
//@}

/**
 * Implement a async_function_argument_capture_holder for a tuple.
 *
 * Capture all the arguments (include the functor) into a single
 * tuple, and then wrap the tuple into an object derived from
 * async_function_argument_capture_tuple.
 */
template<typename tuple_type, typename return_type, typename... functor_args>
class async_function_argument_capture_tuple<
  tuple_type, return_type(functor_args...)> 
    : public async_function_argument_capture_holder<
  return_type(functor_args...)>
{
 public:
  typedef async_function_argument_capture_holder<
    return_type(functor_args...)> base;
  typedef typename base::pointer pointer;

  async_function_argument_capture_tuple() = default;
  async_function_argument_capture_tuple(
      async_function_argument_capture_tuple const &) = default;

  /// Create a new object given a tuple rvalue reference.
  static pointer create(tuple_type && t) {
    return pointer(new async_function_argument_capture_tuple(
        std::forward<tuple_type>(t)));
  }

  virtual bool equals(pointer const & other) const override {
    auto rhs = dynamic_cast<
      async_function_argument_capture_tuple const*>(other.get());
    if (rhs == nullptr) {
      return false;
    }
    return tuple_ == rhs->tuple_;
  }

  virtual void stream(std::ostream & os) const override {
    os << tuple_;
  }

  virtual return_type call_functor(functor_args... args) override {
    std::size_t const N = std::tuple_size<tuple_type>::value;
    return std::get<N-1>(tuple_).value(args...);
  }

  virtual std::size_t argument_count() const override {
    return std::tuple_size<tuple_type>::value;
  }

  virtual std::size_t get_buffer_size() const override {
    return safeish_get_buffer_size(std::get<0>(tuple_), true);
  }
  virtual void const * get_buffer_data() const  override {
    return safeish_get_buffer_data(std::get<0>(tuple_), true);
  }
  virtual void set_buffer_data(void const * data, std::size_t size) override {
    // TODO(ES-27)
    return safeish_set_buffer_data(
        std::get<0>(tuple_), boost::asio::buffer(data, size));
  }

 private:
  /// Constructor.
  async_function_argument_capture_tuple(tuple_type && tuple)
      : tuple_(tuple)
  {}


 private:
  tuple_type tuple_;
};

/**
 * Unimplemented general version.
 *
 * Only the partial specialization for function types is expected to
 * be used.
 */
template<class signature>
class async_function_argument_capture;

/**
 * Define an argument capture strategy for async_* functions as found
 * in Boost.ASIO.
 *
 * @see capture:strategy:motivation for a motivation.
 */
template<class return_type, typename... arg_types>
class async_function_argument_capture<return_type(arg_types...)>
{
 public:
  /// A single argument capture.
  typedef typename async_function_argument_capture_holder<
    return_type(arg_types...)>::pointer value_type;

  /// The type representing a sequence of argument captures.
  typedef std::vector<value_type> capture_sequence;

  /// Capture a set of arguments.
  template<typename... call_types>
  static value_type capture(call_types&&... args) {
    auto t = skye::common::detail::wrap_args_as_tuple(args...);
    return async_function_argument_capture_tuple<
      decltype(t),return_type(arg_types...)>::create(std::move(t));
  }

  static bool equals(value_type lhs, value_type rhs) {
    return lhs->equals(rhs);
  }

  static void stream(std::ostream & os, value_type x) {
    x->stream(os);
  }

  static return_type call_functor(value_type value, arg_types... args) {
    return value->call_functor(args...);
  }
};

} // namespace detail
} // namespace asio
} // namespace skye

#endif // skye_asio_detail_async_function_argument_capture_hpp
