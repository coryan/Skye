#ifndef escapement_e_testing_mock_returner_hpp
#define escapement_e_testing_mock_returner_hpp

#include <memory>
#include <stdexcept>
#include <string>

namespace e {
namespace testing {

template<typename return_type>
class returner {
 public:
  typedef std::unique_ptr<returner<return_type>> pointer;
  virtual ~returner() {}

  virtual return_type execute() = 0;
};

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

template<>
class default_returner<void> : public returner<void> {
 public:
  default_returner() {}
  virtual ~default_returner() {}

  virtual void execute() {}
};

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

template<typename return_type>
class value_returner : public returner<return_type> {
 public:
  value_returner(return_type value)
      : value_(value)
  {}
  virtual ~value_returner() {}

  virtual return_type execute() {
    return value_;
  }

 private:
  return_type value_;
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
template<typename return_value, typename object_type>
struct create_returner<return_value,object_type,true> {
  static typename returner<return_value>::pointer create(object_type object) {
    typedef typename returner<return_value>::pointer pointer; 
    return pointer(new value_returner<return_value>(object));
  }
};

/**
 * If the object type is not convertible to the return value, assume
 * it is a functor.
 */
template<typename return_value, typename object_type>
struct create_returner<return_value,object_type,false> {
  // TODO() Provide better error messages by using the concept-like
  // tricks in Boost.ASIO and Boost.Proto
  // http://cpp-next.com/archive/2010/09/expressive-c-why-template-errors-suck-and-what-you-can-do-about-it/
  static typename returner<return_value>::pointer create(object_type object) {
    typedef typename returner<return_value>::pointer pointer; 
    return pointer(new functor_returner<return_value,object_type>(object));
  }
};

} // namespace testing
} // namespace e

#endif // escapement_e_testing_mock_returner_hpp
