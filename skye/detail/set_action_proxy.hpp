#ifndef skye_detail_set_action_proxy_hpp
#define skye_detail_set_action_proxy_hpp

#include <functional>
#include <stdexcept>
#include <utility>

namespace skye {
namespace detail {

/**
 * Modify the actions in a mock_function subject to the constraints
 * of a predicate.
 *
 * This class is used in the implementation of when() restrictions for
 * return values and other actions.
 */
template<typename return_type, typename predicate>
class set_action_proxy {
 public:
  typedef std::function<return_type()> return_function;
  typedef std::function<void(return_function)> callback;

  /**
   * Constructor, use callback to set the actions in the mock function.
   */
  set_action_proxy(callback cb)
      : callback_(cb)
      , used_(false)
  {}
  ~set_action_proxy() {
    if (not used_) {
      throw std::runtime_error("Called when() without setting an action");
    }
  }

  set_action_proxy() = delete;
  set_action_proxy(set_action_proxy const &) = delete;
  set_action_proxy & operator=(set_action_proxy const &) = delete;
  set_action_proxy(set_action_proxy &&) = default;
    
  template<typename object_type>
  void returns(object_type && object) {
    typedef typename std::remove_reference<object_type>::type value_type;
    static_assert(
        std::is_convertible<value_type, return_type>::value,
        "The values provided in returns() must be convertible to return_value");

    return_function f = [object]() { return object; };
    callback_(f);
    used_ = true;
  }

  template<typename object_type>
  void throws(object_type && object) {
    return_function f = [object]() -> return_type { throw object; };
    callback_(f);
    used_ = true;
  }

  template<typename functor_type>
  void action(functor_type functor) {
    typedef typename std::remove_reference<functor_type>::type value_type;
    static_assert(
        std::is_convertible<value_type, return_function>::value,
        "The functor provided in action() must be storable in"
        " std::function<return_type()>");

    callback_(return_function(functor));
    used_ = true;
  }

 private:
  callback callback_;
  bool used_;
};

} // namespace detail
} // namespace skye

#endif // skye_detail_set_action_proxy_hpp
