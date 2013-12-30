#ifndef escapement_e_testing_invocation_hpp
#define escapement_e_testing_invocation_hpp

#include <tuple>
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

  typedef std::tuple<args...> value_type;
  typedef std::vector<value_type> capture_sequence;

  void operator()(args... a) {
    value_type t(a...);
    captures_.push_back(std::move(t));
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

template<typename... arg_types>
class parametric_call : public parametric_call_base {
 public:
  typedef std::tuple<arg_types...> value_type;

  static parametric_call_base::pointer create(arg_types... a) {
    return parametric_call_base::pointer(new parametric_call(a...));
  }

  value_type const & args() const {
    return args_;
  }

 private:
  parametric_call(arg_types... a)
      : parametric_call_base()
      , args_(a...)
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
  void operator()(arg_types... a) {
    captures_.push_back(parametric_call<arg_types...>::create(a...));
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
