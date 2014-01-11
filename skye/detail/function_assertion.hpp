#ifndef skye_detail_function_assertion_hpp
#define skye_detail_function_assertion_hpp

#include <skye/detail/validator.hpp>
#include <boost/range/adaptor/reversed.hpp>

#include <list>
#include <memory>
#include <string>
#include <sstream>
#include <type_traits>

namespace skye {
namespace detail {

/**
 * Build a validation check, executes it and then reports the results.
 *
 * Somebody more clever than me can figure out how to implement this
 * without lists and other additional data structures, seems like an
 * intrusive list with a sentinel should be able to handle it.  And
 * maybe use a form of the "Chain of Responsibility" pattern to
 * implement the iteration.  The current implementation works as
 * follows.  The user makes a series of chained calls like this:
 *
 * @code
 * mock_function<void(int,std::string const&)> f;
 * f(1, "abc");
 * f(2, "bcd");
 * f(2, "bcd");
 * f(2, "bcd");
 *
 * f.check_called().exactly(3).with(2, "bcd");
 * @endcode
 *
 * After some minimal pre-processor macro hidden inside that
 * check_called() member-function looking thing, the mock_function
 * constructs an instance of this class.  The chain of calls
 * exactly().with(), produces the following list of validators:
 *
 * @code
 * f.check_called().exactly(3).with(2, "bcd");
 * // with_validator(2, "bcd") -> exactly(3)
 * @endverbatim
 *
 * Naturally this sequence is constructor in steps, growing with each
 * call as shown here:
 *
 * @code
 * f.check_called();
 *
 * f.check_called().exactly(3);
 * // exactly(3)
 *
 * f.check_called().exactly(3).with(2, "bcd");
 * // with_validator(2, "bcd") -> exactly(3)
 *
 * @endcode
 *
 * The never() quantifier short-circuits validation, that means that
 * whatever results from subsequent validators are ignored.
 *
 * @tparam capture_strategy_T how was the underlying mock function
 * capturing its arguments.
 * @tparam reporting_strategy_T how are assertion results supposed to
 * be reported.
 */
template<typename capture_strategy_T,
         typename reporting_strategy_T>
class function_assertion {
 public:
  typedef capture_strategy_T capture_strategy;
  typedef reporting_strategy_T reporting_strategy;
  typedef typename capture_strategy::value_type value_type;
  typedef typename capture_strategy::capture_sequence sequence_type;
  typedef std::shared_ptr<validator<sequence_type>> pointer;

  function_assertion(
      sequence_type const & sequence, location const & where)
      : validators_()
      , sequence_(sequence)
      , where_(where) {
    reporting_strategy::checkpoint(where_);
  }
  ~function_assertion() {
    validate();
  }

  //@{
  /**
   * @name Validator factory functions.
   */
  /// Requires at least (inclusive) this many calls after filtering.
  function_assertion & at_least(std::size_t min) {
    add_validator(pointer(new at_least_validator<sequence_type>(min)));
    return *this;
  }

  /// Requires at most (inclusive) this many calls after filtering.
  function_assertion & at_most(std::size_t max) {
    add_validator(pointer(new at_most_validator<sequence_type>(max)));
    return *this;
  }

  /// Requires exactly this many calls after filtering.
  function_assertion & exactly(std::size_t expected) {
    add_validator(pointer(
        new exactly_validator<sequence_type,false>(expected)));
    return *this;
  }

  /// Requires exactly one call after filtering.
  function_assertion & once() {
    return exactly(1);
  }

  /// Requires no calls after filtering.
  function_assertion & never() {
    add_validator(pointer(
        new exactly_validator<sequence_type,true>(0)));
    return *this;
  }

  /// Requires between @a min and @a max calls, both inclusive, after
  /// filtering.
  function_assertion & between(std::size_t min, std::size_t max) {
    return at_least(min).at_most(max);
  }

  /// Filters to only the calls with the given value.
  template<typename... arg_types>
  function_assertion & with(arg_types&&... args) {
    auto match = capture_strategy::capture(std::forward<arg_types>(args)...);
    return with(std::move(match));
  }

  function_assertion & with(value_type && m) {
    value_type match(m);
    std::ostringstream os;
    capture_strategy::stream(os, match);
    add_validator(
        make_negative_filter<sequence_type>( os.str(), [match](value_type v) {
            return not capture_strategy::equals(match, v);
          }));
    return *this;
  }
                         
  //@}

 private:
  void add_validator(pointer v) {
    validators_.push_back(v);
  }

  void validate() {
    sequence_type sequence;
    sequence.swap(sequence_); // We no longer need the contents of sequence_

    for (auto i : boost::adaptors::reverse(validators_)) {
      i->filter(sequence);
    }

    validation_result r{true,false,std::string()};
    std::string msg = "check_called()";
    for (auto i : validators_) {
      r = i->validate(sequence);
      msg += r.msg;
      if (not r.pass or r.short_circuit) {
        break;
      }
    }
    if (r.pass) {
      reporting_strategy::report_success(where_, msg);
    } else {
      reporting_strategy::report_failure(where_, msg);
    }
  }

 private:
  std::list<pointer> validators_;
  sequence_type sequence_;
  location where_;
};

/**
 * Validate using BOOST_CHECK_* semantics, i.e., errors are not fatal.
 *
 * The use of a macro here is required to capture the file and line
 * number.  We are allowing a macro with a short name because this
 * is intended to be used in test code only, so any namespace
 * pollution can be kept under control.
 */
#define check_called() check(SKYE_LOCATION)

/**
 * Validate using BOOST_REQUIRE_* semantics, i.e., errors terminate
 * the test.
 *
 * The use of a macro here is required to capture the file and line
 * number.  We are allowing a macro with a short name because this
 * is intended to be used in test code only, so any namespace
 * pollution can be kept under control.
 */
#define require_called() require(SKYE_LOCATION)

} // namespace detail
} // namespace skye

#endif // skye_detail_function_assertion_hpp
