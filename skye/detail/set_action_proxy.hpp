#ifndef skye_detail_set_action_proxy_hpp
#define skye_detail_set_action_proxy_hpp

#include <skye/detail/mock_returner.hpp>

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
  typedef std::shared_ptr<detail::returner<return_type>> returner_pointer;
  typedef std::function<void(returner_pointer)> callback;

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

} // namespace detail
} // namespace skye

#endif // skye_detail_set_action_proxy_hpp
