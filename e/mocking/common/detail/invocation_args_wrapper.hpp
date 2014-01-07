#ifndef escapement_e_mocking_common_detail_invocation_args_wrapper_hpp
#define escapement_e_mocking_common_detail_invocation_args_wrapper_hpp

#include <e/mocking/common/detail/argument_wrapper.hpp>

#include <tuple>

namespace e {
namespace mocking {
namespace common {
namespace detail {

template<typename T>
auto forward_wrapper(T && arg)
    -> decltype(make_arg_wrapper(std::forward<T>(arg))) {
  return make_arg_wrapper(std::forward<T>(arg));
}

template<typename... args>
auto wrap_args_as_tuple(args&&... a)
    -> decltype(std::make_tuple(forward_wrapper(a)...)) {
  return std::make_tuple(forward_wrapper(a)...);
}

} // namespace detail
} // namespace common
} // namespace mocking
} // namespace e

#endif // escapement_e_mocking_common_detail_invocation_args_wrapper_hpp
