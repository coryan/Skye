#ifndef skye_mock_template_function_hpp
#define skye_mock_template_function_hpp

#include <skye/detail/argument_wrapper.hpp>
#include <skye/detail/unknown_arguments_capture_by_value.hpp>
#include <skye/detail/mock_returner.hpp>
#include <skye/detail/function_assertion.hpp>
#include <skye/detail/assertion_reporting.hpp>

#include <utility>

namespace skye {

/**
 * Implement a mock template function (standalone or member of a class.)
 *
 * This class is intented to support writing unit tests with mocked
 * objects and functions, but where the mocked function itself is a
 * template.  Typically one would define a member variable with the
 * name of the function to be mocked:
 *
 * @code
 * class my_mock_class : ... {
 *   // define a template member function, returning void.
 *   mock_template_function<void> f1;
 *   // define a template member function, returning int.
 *   mock_template_function<int> f2;
 * };
 *
 * void test_foo() {
 *   my_mock_class x;
 *   // f1 does not need to define a return value before it is called:
 *   f1( int(42), true );
 *   f1( std::string("abc"), std::vector<int>(), long(42), false );
 *
 *   // f2 does require a return value, but it is easy to set:
 *   f2.returns( 7 );
 *   f2( int(42), true );
 *   f2( std::string("abc"), std::vector<int>(), long(42), false );
 *   // ... code here can check that the mock object was called
 *   // corrrectly.  Well, poorly, the captures are really hard to use
 *   assert(f2.call_count() == 7);
 * }
 * @endcode
 *
 * @tparam return_type the return type from the mock function.
 * @tparam capture_strategy_T define how arguments are to be captured,
 *   and to some degree the capabilities available to operate on the
 *   arguments. The type must meet the
 *   detail::capture_strategy_requirements interface.
 */
template<
  typename return_type,
  typename capture_strategy_T = detail::unknown_arguments_capture_by_value>
class mock_template_function {
 public:
  //@{
  /**
   * @name Type traits
   */
  typedef capture_strategy_T capture_strategy;
  typedef typename capture_strategy::value_type value_type;
  typedef typename capture_strategy::capture_sequence capture_sequence;
  typedef typename capture_sequence::const_iterator iterator;
  typedef std::unique_ptr<detail::returner<return_type>> returner_pointer;
  //@}

  /// Constructor
  mock_template_function()
      : captures_()
      , returner_(new detail::default_returner<return_type>()) {
  }

  /**
   * Stores the arguments and returns the value provided by the
   * current functor.
   *
   * For non-void return types, this operator raises an error if the
   * user has not set an specific functor or value to return.
   */
  template<typename... arg_types>
  return_type operator()(arg_types&&... args) {
    captures_.push_back(capture_strategy::capture(args...));
    return returner_->execute();
  }

  /**
   * Set the return value, functor, function pointer or lambda
   * expression.
   *
   * Use type traits to discover how to treat the object provided.
   */
  template<typename object_type>
  void returns(object_type && object) {
    typedef typename std::remove_reference<object_type>::type value_type;
    bool const convertible =
        std::is_convertible<value_type, return_type>::value;

    returner_ = detail::create_returner<
      return_type,object_type,convertible>::create(
          std::forward<object_type>(object));
  }

  /// Create a new function assertion, where failures do not terminate
  /// the current test.
  detail::function_assertion<
    capture_strategy, detail::default_check_reporting>
  check(detail::location const & where) {
    return detail::function_assertion<
      capture_strategy, detail::default_check_reporting>(
          captures_, where);
  }

  /// Create a new function assertion, where failures terminate the
  /// current test.
  detail::function_assertion<
    capture_strategy, detail::default_require_reporting>
  require(detail::location const & where) {
    return detail::function_assertion<
      capture_strategy, detail::default_require_reporting>(
          captures_, where);
  }

  //@{
  /**
   * @name Accessors
   */
  void clear() {
    captures_.clear();
  }
  bool has_calls() const {
    return not captures_.empty();
  }
  std::size_t call_count() const {
    return captures_.size();
  }
  iterator begin() const {
    return captures_.begin();
  }
  iterator end() const {
    return captures_.end();
  }
  value_type & at(std::size_t i) {
    return captures_.at(i);
  }
  value_type const & at(std::size_t i) const {
    return captures_.at(i);
  }
  //@}

 private:
  capture_sequence captures_;
  returner_pointer returner_;
};

} // namespace skye

#endif // skye_mock_template_function_hpp
