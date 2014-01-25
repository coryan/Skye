#ifndef skye_detail_mock_returner_hpp
#define skye_detail_mock_returner_hpp

#include <memory>
#include <stdexcept>
#include <string>

namespace skye {
namespace detail {

/**
 * Define the interface for objects used to return values from mock functions.
 */
template<typename return_type>
class returner {
 public:
  typedef std::unique_ptr<returner<return_type>> pointer;
  virtual ~returner() {}

  virtual return_type execute() = 0;
};

/**
 * Implement the default skye::detail::returner for any type.
 *
 * By default a mock function instantiates an object of this type to
 * "return" values, but this in fact only raises exceptions.
 */
template<typename return_type>
class default_returner : public returner<return_type> {
 public:
  default_returner() {}
  virtual ~default_returner() {}

  virtual return_type execute() {
    std::string msg("You must set returns() before the first call.");
    throw std::runtime_error(msg);
  }
};

/**
 * Specialize default_returner for void.
 *
 * Simply perform a no-op instead of raising exceptions.
 */
template<>
class default_returner<void> : public returner<void> {
 public:
  default_returner() {}
  virtual ~default_returner() {}

  virtual void execute() {}
};

/**
 * Implement a skye::detail::returner based on a functor object.
 */
template<typename return_type, typename functor_type>
class functor_returner : public returner<return_type> {
 public:
  functor_returner(functor_type functor)
      : functor_(functor)
  {}
  virtual ~functor_returner() {}

  virtual return_type execute() {
    return functor_();
  }

 private:
  functor_type functor_;
};

/**
 * Implement a skye::detail::returner given a constant value.
 */
template<typename return_type, typename value_type>
class value_returner : public returner<return_type> {
 public:
  value_returner(value_type && value)
      : value_(value)
  {}
  virtual ~value_returner() {}

  virtual return_type execute() {
    return value_;
  }

 private:
  value_type value_;
};

/**
 * A helper class to create returner objects for either values or functors.
 */
template<typename return_value, typename object_type, bool convertible>
struct create_returner;

/**
 * If the object type is convertible to the return value, assume it is
 * a value, store it and use it as the return in each call.
 */
template<typename return_type, typename object_type>
struct create_returner<return_type,object_type,true> {
  // Try to generate more sensible error messages for the poor user.
  typedef typename std::remove_reference<object_type>::type value_type;
  static_assert(
      std::is_convertible<value_type, return_type>::value,
      "value_type (object_type after all reference qualifiers are removed) "
      "should be convertible to return_type.");

  static typename returner<return_type>::pointer create(object_type && object) {
    typedef typename returner<return_type>::pointer pointer; 
    return pointer(new value_returner<return_type,value_type>(
        std::forward<value_type>(object)));
  }
};

/**
 * If the object type is not convertible to the return value, assume
 * it is a functor.
 */
template<typename return_value, typename object_type>
struct create_returner<return_value,object_type,false> {
  // TODO(SMF-5)
  static typename returner<return_value>::pointer create(object_type object) {
    typedef typename returner<return_value>::pointer pointer; 
    return pointer(new functor_returner<return_value,object_type>(object));
  }
};

} // namespace detail
} // namespace skye

#endif // skye_detail_mock_returner_hpp
