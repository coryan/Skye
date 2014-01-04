#ifndef escapement_e_testing_mock_function_hpp
#define escapement_e_testing_mock_function_hpp

#include <e/testing/invocation_args_wrapper.hpp>
#include <e/testing/mock_returner.hpp>
#include <vector>
#include <memory>
#include <utility>

namespace e {
namespace testing {

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
 *   // ... code here can check that the mock object was called corrrectly.
 *   assert(f2.call_count() == 7);
 * }
 * @endcode
 */
template<typename return_type, typename... arg_types>
class mock_function<return_type(arg_types...)> {
 public:
  mock_function()
      : captures_()
      , returner_(new default_returner<return_type>()) {
  }
  
  typedef typename deduce_wrap_args_as_tuple_types<
    arg_types...>::tuple value_type;
  typedef std::vector<value_type> capture_sequence;
  typedef typename capture_sequence::const_iterator iterator;
  typedef std::unique_ptr<returner<return_type>> returner_pointer;

  /**
   * Stores the arguments and returns the value provided by the
   * current functor.
   *
   * For non-void return types, this operator raises an error if the
   * user has not set an specific functor or value to return.
   */
  return_type operator()(arg_types... args) {
    captures_.push_back(wrap_args_as_tuple(args...));
    return returner_->execute();
  }

  template<typename object_type>
  void returns(object_type && object) {
    typedef typename std::remove_reference<object_type>::type value_type;
    bool const convertible =
        std::is_convertible<value_type, return_type>::value;

    returner_ = create_returner<
      return_type,object_type,convertible>::create(
          std::forward<object_type>(object));
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

 private:
  capture_sequence captures_;
  returner_pointer returner_;
};

} // namespace testing
} // namespace e

#endif // escapement_e_testing_mock_function_hpp
