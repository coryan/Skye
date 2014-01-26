#ifndef skye_mock_function_hpp
#define skye_mock_function_hpp

#include <skye/detail/argument_wrapper.hpp>
#include <skye/detail/mock_returner.hpp>
#include <skye/detail/function_assertion.hpp>
#include <skye/detail/assertion_reporting.hpp>

#include <functional>
#include <list>
#include <utility>

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
  typedef std::shared_ptr<detail::returner<return_type>> returner_pointer;

  //@}

  mock_function()
      : captures_()
      , side_effects_()
      , returner_(new detail::default_returner<return_type>) {
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
        return i.second->execute();
      }
    }
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

  typedef std::function<bool(arg_types&&...)> predicate;
  typedef std::function<void(returner_pointer)> callback;
  typedef std::list<std::pair<predicate, returner_pointer>> side_effects;

  class when_proxy {
   public:
    when_proxy() = delete;
    when_proxy(when_proxy const &) = delete;
    when_proxy & operator=(when_proxy const &) = delete;
    when_proxy(when_proxy &&) = default;
    when_proxy(callback cb)
        : callback_(cb)
        , used_(false)
    {}
    ~when_proxy() {
      if (not used_) {
        throw std::runtime_error("Called when() without setting an action");
      }
    }
    
    template<typename object_type>
    void returns(object_type && object) {
      typedef typename std::remove_reference<object_type>::type value_type;
      bool const convertible =
          std::is_convertible<value_type, return_type>::value;

      auto r = detail::create_returner<
        return_type,object_type,convertible>::create(
            std::forward<object_type>(object));
      callback_(std::move(r));
      used_ = true;
    }

   private:
    callback callback_;
    bool used_;
  };


  when_proxy whenp(predicate p) {
    callback cb = [this,p](returner_pointer r) mutable {
      this->side_effects_.push_back(std::make_pair(p, r));
    };
    return when_proxy(cb);
  }

  /// Create a predicate that matches the arguments and returns a
  /// proxy for it.
  when_proxy when(arg_types&&... args) {
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
    returner_.reset(new detail::default_returner<return_type>);
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
  returner_pointer returner_;
};

} // namespace skye

#endif // skye_mock_function_hpp
