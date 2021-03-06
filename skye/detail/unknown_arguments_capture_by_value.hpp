#ifndef skye_detail_unknown_arguments_capture_by_value_hpp
#define skye_detail_unknown_arguments_capture_by_value_hpp

#include <skye/detail/argument_wrapper.hpp>
#include <skye/detail/tuple_streaming.hpp>
#include <memory>

namespace skye {
namespace detail {

/**
 * Define the interface for classes that hold an argument set of unknown types.
 *
 * When capturing arguments for a mock template function we need to
 * hold each capture in a homogenous type so we can operate on them.
 * Even though the types, and the number, of the arguments is
 * unknown.
 */
class unknown_arguments_by_value_holder {
 public:
  /// These objects are held through a shared_ptr<> often, so define
  /// an typedef for it.
  typedef std::shared_ptr<unknown_arguments_by_value_holder> pointer;

  /// Destructor.
  virtual ~unknown_arguments_by_value_holder() {}

  /**
   * Compare two holders for equality.
   *
   * This member function is expected to perform a deep comparison,
   * basically member-by-member.
   */
  virtual bool equals(pointer const & other) const = 0;

  /// Print the argument values using iostreams.
  virtual void stream(std::ostream & os) const = 0;

  /// Return the number of arguments in the call.
  virtual std::size_t argument_count() const = 0;
};

/**
 * Implement a unknown_arguments_by_value_holder for a tuple.
 *
 * Capture the arguments into a single tuple, and then wrap the tuple
 * into an object derived from unknown_arguments_by_value_holder.
 */
template<typename tuple_type>
class unknown_arguments_by_value_holder_tuple
    : public unknown_arguments_by_value_holder {
 public:
  typedef unknown_arguments_by_value_holder base;
  typedef typename base::pointer pointer;

  unknown_arguments_by_value_holder_tuple() = default;
  unknown_arguments_by_value_holder_tuple(
      unknown_arguments_by_value_holder_tuple const &) = default;

  /// Create a new object given a tuple rvalue reference.
  static pointer create(tuple_type && t) {
    return pointer(new unknown_arguments_by_value_holder_tuple(
        std::forward<tuple_type>(t)));
  }

  virtual bool equals(pointer const & other) const override {
    auto rhs = dynamic_cast<
      unknown_arguments_by_value_holder_tuple const*>(other.get());
    if (rhs == nullptr) {
      return false;
    }
    return tuple_ == rhs->tuple_;
  }

  virtual std::size_t argument_count() const override {
    return std::tuple_size<tuple_type>::value;
  }

  virtual void stream(std::ostream & os) const override {
    os << tuple_;
  }

 private:
  /// Constructor
  unknown_arguments_by_value_holder_tuple(tuple_type && tuple)
      : tuple_(tuple)
  {}

 private:
  tuple_type tuple_;
};

/**
 * Define a strategy to capture arguments in mock functions.
 *
 * Hold each capture in an object (@ref unknown_arguments_by_value_holder_tuple)
 * which is a template class, parametric on the tuple used to hold the
 * arguments.  The class inherits from arguments_by_value_holder, and
 * provides virtual functions to print the values, count the
 * arguments, and match against another capture.  These virtual
 * functions are used in the implementation of check_called() and its
 * related assertion Mock.
 *
 * @see unknown_arguments_by_value_holder_tuple
 * @see unknown_arguments_by_value_holder
 * @see report_with_check
 * @see capture:strategy:motivation
 */
struct unknown_arguments_capture_by_value {
  /// A single argument capture.
  typedef unknown_arguments_by_value_holder::pointer value_type;

  /// The type representing a sequence of argument captures.
  typedef std::vector<value_type> capture_sequence;

  template<typename... arg_types>
  static value_type capture(arg_types&&... args) {
    auto t = wrap_args_as_tuple(args...);
    return unknown_arguments_by_value_holder_tuple<decltype(t)>::create(
        std::move(t));
  }

  static bool equals(value_type lhs, value_type rhs) {
    return lhs->equals(rhs);
  }

  static void stream(std::ostream & os, value_type x) {
    x->stream(os);
  }
};

} // namespace detail
} // namespace skye

#endif // skye_detail_unknown_arguments_capture_by_value_hpp
