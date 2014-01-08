#ifndef escapement_e_mocking_asio_detail_async_function_argument_capture_hpp
#define escapement_e_mocking_asio_detail_async_function_argument_capture_hpp

#include <e/mocking/common/detail/argument_wrapper.hpp>

#include <memory>

namespace e {
namespace mocking {
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
  virtual return_type call_functor(functor_args... args) const = 0;

  /// Return the number of arguments in the call.
  virtual std::size_t argument_count() const = 0;
};

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

  virtual return_type call_functor(functor_args... args) const override {
    std::size_t const N = std::tuple_size<tuple_type>::value;
    return std::get<N-1>(tuple_).value(args...);
  }

  virtual std::size_t argument_count() const override {
    return std::tuple_size<tuple_type>::value;
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
  typedef typename async_function_argument_capture_holder<
    return_type(arg_types...)>::pointer value_type;

  template<typename... call_types>
  static value_type capture(call_types&&... args) {
    auto t = e::mocking::common::detail::wrap_args_as_tuple(args...);
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
} // namespace mocking
} // namespace e

#endif // escapement_e_mocking_asio_detail_async_function_argument_capture_hpp
