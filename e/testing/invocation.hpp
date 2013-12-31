#ifndef escapement_e_testing_invocation_hpp
#define escapement_e_testing_invocation_hpp

#include <e/testing/invocation_args_wrapper.hpp>
#include <vector>
#include <memory>

namespace e {
namespace testing {

/**
 * Represent a sequence of invocations to a 'mock' member function.
 *
 * I grew tired of the Turtle mocking framework, its 'record ->
 * replay' approach is extremely tedious.  So I am slowly growing a
 * new framework for mocking.
 */
template<typename... args>
class invocation {
 public:
  invocation() {}
  ~invocation() {}

  typedef typename deduce_wrap_args_as_tuple_types<args...>::tuple value_type;
  typedef std::vector<value_type> capture_sequence;

  void operator()(args... a) {
    captures_.push_back(wrap_args_as_tuple(a...));
  }

  void clear() {
    captures_.clear();
  }
  bool has_calls() const {
    return not captures_.empty();
  }
  std::size_t call_count() const {
    return captures_.size();
  }

  typedef typename capture_sequence::const_iterator iterator;

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
  
 private:
  capture_sequence captures_;
};

class parametric_call_base {
 public:
  typedef std::shared_ptr<parametric_call_base> pointer;
  virtual ~parametric_call_base() = 0;
};

template<typename tuple_type>
class parametric_call : public parametric_call_base {
 public:
  typedef tuple_type value_type;
  virtual ~parametric_call() {}

  static parametric_call_base::pointer create(tuple_type && t) {
    return parametric_call_base::pointer(
        new parametric_call(std::forward<tuple_type>(t)));
  }

  value_type const & args() const {
    return args_;
  }

 private:
  parametric_call(tuple_type && t)
      : parametric_call_base()
      , args_(t)
  {}

 private:
  value_type args_;
};

class parametric_invocation {
 public:
  parametric_invocation() {}
  ~parametric_invocation() {}

  typedef parametric_call_base::pointer value_type;
  typedef std::vector<value_type> capture_sequence;

  template<typename... arg_types>
  void operator()(arg_types&&... a) {
    typedef decltype(wrap_args_as_tuple(a...)) stored_tuple;
  
    stored_tuple t = wrap_args_as_tuple(a...);
    captures_.push_back(
        parametric_call<stored_tuple>::create(std::move(t)));
  }

  void clear() {
    captures_.clear();
  }
  bool has_calls() const {
    return not captures_.empty();
  }
  std::size_t call_count() const {
    return captures_.size();
  }

  typedef typename capture_sequence::const_iterator iterator;

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
  
 private:
  capture_sequence captures_;
};

} // namespace testing
} // namespace e

#endif // escapement_e_testing_invocation_hpp
