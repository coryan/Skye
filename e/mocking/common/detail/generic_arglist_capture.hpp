#ifndef escapement_e_mocking_common_detail_generic_arglist_capture_hpp
#define escapement_e_mocking_common_detail_generic_arglist_capture_hpp

#include <memory>

namespace e {
namespace mocking {
namespace common {
namespace detail {

/**
 * Represent a capture where the types were not known at compile time,
 * such as in a template function.
 */
class arglist_capture_base {
 public:
  typedef std::shared_ptr<arglist_capture_base> pointer;
  virtual ~arglist_capture_base() = 0;
};

/**
 * Implement argument capture for a tuple with any number of fields.
 */
template<typename tuple_type>
class any_tuple_capture : public arglist_capture_base {
 public:
  typedef tuple_type value_type;
  virtual ~any_tuple_capture() {}

  /**
   * Wrap a tuple within a any_tuple_capture<>
   */
  static arglist_capture_base::pointer create(tuple_type && t) {
    return arglist_capture_base::pointer(
        new parametric_call(std::forward<tuple_type>(t)));
  }

 private:
  any_tuple_capture(tuple_type && t)
      : arglist_capture_base()
      , args_(t)
  {}

 private:
  value_type args_;
};

} // namespace detail
} // namespace common
} // namespace mocking
} // namespace e

#endif // escapement_e_mocking_common_detail_generic_arglist_capture_hpp
