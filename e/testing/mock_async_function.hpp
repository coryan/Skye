#ifndef escapement_e_testing_mock_async_function_hpp
#define escapement_e_testing_mock_async_function_hpp

#include <vector>
#include <utility>

namespace e {
namespace testing {

/**
 * A class to mock member functions conforming to the boost::asio
 * async_[operation]() interface.
 *
 * To mock objects in boost::asio we need to provide function-like
 * objects that:
 *   1) Implement a template operator() where the parameters are only
 *      known at compile time.  In other words, we need to mock a
 *      template function, and sometimes a template member function.
 *   2) All these asynchronous functions take a callback handler as
 *      the last parameter.
 *   3) For a single mocked function, the callback handlers all meet
 *      the same interface, but they can be of different types (such
 *      as different lambdas, with different bound values.)
 *   4) The tests would want to exercise the handlers with specific
 *      inputs.
 *
 * Typically the mocked object will be used as follows:
 *
 * struct some_mocked_class : public e::testing::mock_service {
 *
 * mock_async_invocation<...,...> async_operation;
 * };
 *
 * some_mocked_class s;
 * tested_class o(s);
 *
 * The tested class would call s.async_operation(...) with different
 * types and values.  The tester would want to simulate callbacks,
 * using:
 *
 * // Call the handler received in the fourth callback...
 * s.async_operator.at(3)->call_handler(1, 2, 3);
 * ================================================================
 * 
 * The Turtle mocking library does not seem able to support template
 * member functions, or if it does I (coryan@) cannot figure out how
 * to make it happen.  So, I am writing my own (hopefully modest,)
 * version.  The basic problem is that at least in principle each call
 * could have a different set of template parameters.
 *
 * This mocking class will conform to the "action -> assertion" model
 * of mocking, while Turtle tends to support "record -> replay".  I
 * find the latter very tedious.
 *
 * TODO(ES-26)
 *
 * Boost ASIO asynchronous calls can be characterized by the type of
 * their handler, here we capture this in a traits class that derived
 * classes (such as mock_async_io_function or
 * mock_async_accept_function.
 */
template<typename traits>
class mock_async_function {
 public:
  typedef typename traits::base_pointer value_type;
  typedef std::vector<value_type> call_sequence;
  typedef typename call_sequence::const_iterator iterator;

  mock_async_function()
  {}


  //@{
  /**
   * @name Mock interface query and reset
   */
  void reset() {
    calls_.clear();
  }
  bool has_calls() const {
    return not calls_.empty();
  }
  std::size_t call_count() const {
    return calls_.size();
  }
  value_type & at(int index) {
    return calls_.at(index);
  }
  value_type const & at(int index) const {
    return calls_.at(index);
  }
  iterator begin() const {
    return calls_.begin();
  }
  iterator end() const {
    return calls_.end();
  }
  //@}

  //@{
  /**
   * @name Function behavior with multipler argument counts.
   *
   * Overload operator() to make this look like a function object.
   * This is necessarily a template because:
   * 1) Boost ASIO implements multiple versions of these functions,
   * with different argument types.
   * 2) Boost ASIO requires some of these calls to take different
   * number of arguments.
   * 3) Though the interface for the handler functor is known, its
   * actual type is not.
   *
   * Because the handler is the last argument we can not use variadic
   * templates and must provide a suitable number of overloads.  Those
   * overloads that are not used (or fail to compile because there is
   * no traits::create() function) are not instantiated.
   *
   */
  template<typename T1, typename handler>
  void operator()(T1 && arg1, handler h) {
    refactor_functor_operator(
        h, std::forward<T1>(arg1));
  }

  template<typename T1, typename T2, typename handler>
  void operator()(T1 && arg1, T2 && arg2, handler h) {
    refactor_functor_operator(
        h, std::forward<T1>(arg1), std::forward<T2>(arg2));
  }

  template<typename T1, typename T2, typename T3, typename handler>
  void operator()(T1 && arg1, T2 && arg2, T3 && arg3, handler h) {
    refactor_functor_operator(
        h, std::forward<T1>(arg1), std::forward<T2>(arg2),
        std::forward<T3>(arg3));
  }

  template<typename T1, typename T2, typename T3, typename T4, typename handler>
  void operator()(T1 && arg1, T2 && arg2, T3 && arg3, T4 && arg4, handler h) {
    refactor_functor_operator(
        h, std::forward<T1>(arg1), std::forward<T2>(arg2),
        std::forward<T3>(arg3), std::forward<T4>(arg4));
  }
  //@}

 private:
  template<typename handler, typename... arg_types>
  void refactor_functor_operator(handler h, arg_types&&... a) {
    value_type capture = traits::create(h, std::forward<arg_types>(a)...);
    calls_.push_back(capture);
  }

 private:
  call_sequence calls_;
};

} // namespace testing
} // namespace e

#endif // escapement_e_testing_mock_async_function_hpp
