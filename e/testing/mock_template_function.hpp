#ifndef escapement_e_testing_mock_template_function_hpp
#define escapement_e_testing_mock_template_function_hpp

#include <e/testing/invocation_args_wrapper.hpp>
#include <e/testing/generic_arglist_capture.hpp>
#include <e/testing/mock_returner.hpp>

#include <vector>
#include <memory>
#include <utility>

namespace e {
namespace testing {

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
 */
template<class return_type>
class mock_template_function {
 public:
  //@{
  /**
   * @name Type traits
   */
  typedef generic_capture_base::pointer value_type;
  typedef std::vector<value_type> capture_sequence;
  typedef typename capture_sequence::const_iterator iterator;
  typedef std::unique_ptr<returner<return_type>> returner_pointer;
  //@}

  /// Constructor
  mock_template_function()
      : captures_()
      , returner_(new default_returner<return_type>()) {
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
    auto t = wrap_args_as_tuple(args...);
    captures_.push_back(
        generic_arglist_capture<decltype(t)>::create(std::move(t)));
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
  capture_sequence captures_;
  returner_pointer returner_;
};

} // namespace testing
} // namespace e

#endif // escapement_e_testing_mock_template_function_hpp
