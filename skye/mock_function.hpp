#ifndef skye_mock_function_hpp
#define skye_mock_function_hpp

#include <skye/detail/argument_wrapper.hpp>
#include <skye/detail/default_return.hpp>
#include <skye/detail/function_assertion.hpp>
#include <skye/detail/assertion_reporting.hpp>
#include <skye/detail/set_action_proxy.hpp>

#include <list>

namespace skye {

/**
 * Unimplemented, only the specialization for function signatures is
 * of any interest.
 */
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
 *   f3.action( []() { return std::string("foo"); }
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
  typedef std::function<bool(arg_types&&...)> predicate;
  typedef detail::set_action_proxy<return_type,predicate> set_action_proxy;
  typedef typename set_action_proxy::return_function return_function;
  typedef typename set_action_proxy::callback callback;
  typedef std::list<std::pair<predicate, return_function>> side_effects;
  //@}

  mock_function()
      : captures_()
      , side_effects_()
      , default_return_(detail::default_return<return_type>) {
  }

  /**
   * Stores the arguments and returns the value provided by the
   * current functor.
   *
   * For non-void return types, this operator raises an error if the
   * user has not set an specific functor or value to return.
   */
  return_type operator()(arg_types... args) {
    auto v = capture_strategy::capture(std::forward<arg_types>(args)...);
    captures_.push_back(v);
    for (auto & i : side_effects_) {
      if (i.first(std::forward<arg_types>(args)...)) {
        return i.second();
      }
    }
    return default_return_();
  }

  /// Set a simple return value.
  template<typename object_type>
  void returns(object_type && object) {
    callback cb = [this](return_function f) mutable {
      default_return_ = f;
    };
    set_action_proxy(cb).returns(object);
  }

  /// Throw an exception.
  template<typename object_type>
  void throws(object_type && object) {
    callback cb = [this](return_function f) mutable {
      default_return_ = f;
    };
    set_action_proxy(cb).throws(object);
  }

  /// Return using a functor object.
  template<typename functor_type>
  void action(functor_type functor) {
    callback cb = [this](return_function f) mutable {
      default_return_ = f;
    };
    set_action_proxy(cb).action(functor);
  }

  /// Prepare a proxy for a given predicate.
  set_action_proxy whenp(predicate p) {
    callback cb = [this,p](return_function f) mutable {
      this->side_effects_.push_back(std::make_pair(p, f));
    };
    return set_action_proxy(cb);
  }

  /// Create a predicate that matches the arguments and returns a
  /// proxy for it.
  set_action_proxy when(arg_types&&... args) {
    auto match = capture_strategy::capture(std::forward<arg_types>(args)...);
    predicate p = [match](arg_types&&... args) {
      auto v = capture_strategy::capture(std::forward<arg_types>(args)...);
      return capture_strategy::equals(match, v);
    };
    return whenp(p);
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


  /**
   * Reset the mock to its initial state.
   */
  void clear() {
    clear_captures();
    clear_returns();
  }

  /**
   * Clear any settings for returns().
   */
  void clear_returns() {
    side_effects_.clear();
    default_return_ = detail::default_return<return_type>;
  }

  /**
   * Clear any captured calls.
   */
  void clear_captures() {
    captures_.clear();
  }

  //@{
  /**
   * @name Accessors
   */
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
  side_effects side_effects_;
  return_function default_return_;
};

} // namespace skye

#endif // skye_mock_function_hpp
