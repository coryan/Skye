#ifndef skye_detail_default_return_hpp
#define skye_detail_default_return_hpp

#include <stdexcept>

namespace skye {
namespace detail {

template<typename return_type>
return_type default_return() {
  throw std::runtime_error("You must set returns() before the first call.");
}

template<>
void default_return<void>() {
}

} // namespace detail
} // namespace skye

#endif // skye_detail_default_return_hpp
