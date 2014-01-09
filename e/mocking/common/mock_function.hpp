#ifndef escapement_e_mocking_common_mock_function_hpp
#define escapement_e_mocking_common_mock_function_hpp

#include <e/mocking/common/detail/argument_wrapper.hpp>
#include <e/mocking/common/detail/mock_returner.hpp>
#include <e/mocking/common/detail/function_assertion.hpp>
#include <e/mocking/common/detail/boost_assertion_reporting.hpp>
#include <utility>

namespace e {
namespace mocking {
namespace common {

template<typename T>
class mock_function;

/**
 * Implement a mock function (standalone or member of a class.)
 *
 * This class is intented to support writing unit tests with mocked
 * objects and functions.  Typically one would define a member
 * variable with the name of the function to be mocked:
 *
 * @code
 * class my_mock_class : ... {
 *   // define a couple of mock functions
 *   mock_function<int(std::string const&, std::string &)> f1;
 *   mock_function<void(int,int,int)> f2;
 *
 *   // ugly virtual function
 *   virtual std::string f3(int x, char const *y) {
 *     return f3_capture(x, y);
 *   }
 *   mock_function<std::string(int,char const*)> f3_capture;
 * };
 *
 * void test_foo(foo & f) {
 *   my_mock_class x;
 *   // f1 and f3 need to define return values, but not f2...
 *   f1.returns( 42 );
 *   f3.returns( []() { return std::string("foo"); }
 *   // ... code here can call x.f1(), x.f2(), x.f3()
 *   f.use(x);
 *   // ... code here can check that the mock object was called
 *   // corrrectly.  Admittedly the facilities are poor, but this is
 *   // work in progress.
 *   assert(f2.call_count() == 7);
 *   assert(f1.call_count() >= 1);
 *   assert(std::get<0>(f1.at(0)) == 42);
 * }
 * @endcode
 */
template<typename return_type, typename... arg_types>
class mock_function<return_type(arg_types...)> {
 public:
  //@{
  /**
   * @name Type traits
   */
  /// Hardcoded capture strategy.
  typedef detail::known_arguments_capture_by_value<arg_types...>
    capture_strategy;
  typedef typename capture_strategy::value_type value_type;
  typedef typename capture_strategy::capture_sequence capture_sequence;
  typedef typename capture_sequence::const_iterator iterator;
  typedef std::unique_ptr<detail::returner<return_type>> returner_pointer;
  //@}

  mock_function()
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
  return_type operator()(arg_types... args) {
    captures_.push_back(detail::wrap_args_as_tuple(args...));
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
  detail::function_assertion<capture_strategy, detail::boost_check_reporting>
  check(detail::location const & where) {
    return detail::function_assertion<
      capture_strategy, detail::boost_check_reporting>(
          captures_, where);
  }

  /// Create a new function assertion, where failures terminate the
  /// current test.
  detail::function_assertion<capture_strategy, detail::boost_require_reporting>
  require(detail::location const & where) {
    return detail::function_assertion<
      capture_strategy, detail::boost_require_reporting>(
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

} // namespace common
} // namespace mocking
} // namespace e

#endif // escapement_e_mocking_common_mock_function_hpp
