#ifndef escapement_e_mocking_common_detail_validator_hpp
#define escapement_e_mocking_common_detail_validator_hpp

#include <e/mocking/common/detail/tuple_streaming.hpp>

#include <algorithm>
#include <string>
#include <sstream>
#include <iostream>
#include <tuple>

namespace e {
namespace mocking {
namespace common {
namespace detail {

/**
 * Wrap function name, filename and line number into a simple struct.
 *
 * This is a simple wrapper to collect all of __func__, __FILE__ and
 * __LINE__ into a single object.  It is used by the validators to
 * pass this information to the underlying unit testing framework.
 */
struct location {
  location(
      char const * fu,
      char const * fi,
      int li)
      : function(fu)
      , file(fi)
      , line(li)
  {}

  char const * function;
  char const * file;
  int line;
};

/**
 * Define a streaming operator.
 */
inline std::ostream & operator<<(std::ostream & os, location const & x) {
  return os << x.function << "@[" << x.file << ":" << x.line << "]";
}

/**
 * Capture the location macros into an object.
 */
#define ESCAPEMENT_MOCK_LOCATION \
  e::mocking::common::detail::location(__func__, __FILE__, __LINE__)

/**
 * A simple object to hold the validation results
 */
struct validation_result {
  bool pass;
  bool short_circuit;
  std::string msg;

  //@{
  /**
   * @name comparison operators for testing
   */
  bool operator==(validation_result const & rhs) const {
    return (pass == rhs.pass and short_circuit == rhs.short_circuit);
  }
  bool operator!=(validation_result const & rhs) const {
    return !(*this == rhs);
  }
  //@}
};

/**
 * Define a streaming operator (mostly for unit testing).
 */
inline std::ostream & operator<<(
    std::ostream & os, validation_result const & x) {
  return os << std::boolalpha
            << "validation_result{pass=" << x.pass
            << ",short_circuit=" << x.short_circuit
            << ",msg='" << x.msg << "'}";
}

/**
 * Define the interface for validator objects.
 *
 * Validators are asked to filter the sequence of argument list
 * captures, and -- once all the filter are applied -- verify that the
 * resulting set meets expectations.
 *
 * Typically validators are created by a user of a mock function and
 * chained together, for example:
 *
 * @code
 * mock_function<void(int,std::string const&)> x;
 * x(1, "foo");
 * x(2, "bar");
 *
 * x.check_called().once().with(1, "foo");
 * x.check_called().exactly(2);
 * @endcode
 */
template<typename sequence_type>
class validator {
 public:
  validator() {}
  virtual ~validator() {}

  /**
   * Apply any filtering required by the validator
   */
  virtual void filter(sequence_type & sequence) const = 0;

  /**
   * Verify that the sequence after all filtering meets the validator
   * requirements.
   */
  virtual validation_result validate(sequence_type const &) const = 0;
};

/**
 * Verify that the argument capture after all filters has at least a
 * prescribed number of calls.
 */
template<typename sequence_type>
class at_least_validator : public validator<sequence_type> {
 public:
  at_least_validator(std::size_t min)
      : min_(min)
  {}

  void filter(sequence_type & sequence) const override {
  }
  validation_result validate(
      sequence_type const & sequence) const override {
    std::size_t size = sequence.size();
    if (size >= min_) {
      std::ostringstream os;
      os << ".at_least( " << min_ << " )";
      return validation_result{true, false, os.str()};
    }
    std::ostringstream os;
    os << "failed validation, expected at least " 
       << min_ << " invocations, but only "
       << size << " where present.";
    return validation_result{false, false, os.str()};
  }

 private:
  std::size_t min_;
};

/**
 * Verify that the argument capture after all filters has at most a
 * prescribed number of calls.
 */
template<typename sequence_type>
class at_most_validator : public validator<sequence_type> {
 public:
  at_most_validator(std::size_t max)
      : max_(max)
  {}

  void filter(sequence_type & sequence) const override {
  }
  validation_result validate(
      sequence_type const & sequence) const override {
    std::size_t size = sequence.size();
    if (size <= max_) {
      std::ostringstream os;
      os << ".at_most( " << max_ << " )";
      return validation_result{true, false, os.str()};
    }
    std::ostringstream os;
    os << "failed validation, expected at most " 
       << max_ << " invocations, but "
       << size << " where present.";
    return validation_result{false, false, os.str()};
  }

 private:
  std::size_t max_;
};

/**
 * Verify that the argument capture after all filters has exactly a
 * prescribed number of calls.
 *
 * @param short_circuit define if the validation should short circuit
 * the chain.  This is desirable for the never() validator.
 */
template<typename sequence_type, bool short_circuit>
class exactly_validator : public validator<sequence_type> {
 public:
  exactly_validator(std::size_t expected)
      : expected_(expected)
  {}

  void filter(sequence_type & sequence) const override {
  }
  validation_result validate(
      sequence_type const & sequence) const override {
    std::size_t size = sequence.size();
    if (size == expected_) {
      std::ostringstream os;
      if (short_circuit && expected_ == 0) {
        os << ".never()";
      } else {
        os << ".exactly( " << expected_ << " )";
      }
      return validation_result{true, short_circuit, os.str()};
    }
    std::ostringstream os;
    os << "failed validation, expected exactly " 
       << expected_ << " invocations, but "
       << size << " where present.";
    return validation_result{false, short_circuit, os.str()};
  }

 private:
  std::size_t expected_;
};

/**
 * Implement a filter that only accepts captures matching an specific value.
 */
template<class sequence_type>
class equality_filter : public validator<sequence_type> {
 public:
  typedef typename sequence_type::value_type value_type;

  equality_filter(value_type && match)
      : match_(match)
  {}
  equality_filter(value_type const & match)
      : match_(match)
  {}

  void filter(sequence_type & sequence) const override {
    sequence.erase(
        std::remove_if(
            sequence.begin(), sequence.end(),
            [this](value_type const & x) {
              return x != match_;
            }),
        sequence.end());
  }
  validation_result validate(
      sequence_type const & ) const override {
    std::ostringstream os;
    os << ".with( " << match_ << " )";
    return validation_result{true, false, os.str()};
  }
  
 private:
  value_type match_;
};

} // namespace detail
} // namespace common
} // namespace mocking
} // namespace e

#endif // escapement_e_mocking_common_detail_validator_hpp
