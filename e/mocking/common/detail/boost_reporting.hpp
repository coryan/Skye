#ifndef escapement_e_mocking_common_detail_boost_reporting_hpp
#define escapement_e_mocking_common_detail_boost_reporting_hpp

#include <e/mocking/common/detail/validator.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <boost/test/unit_test.hpp>

#include <list>
#include <memory>
#include <string>
#include <sstream>
#include <type_traits>

namespace e {
namespace mocking {
namespace common {
namespace detail {

/**
 * The root for a validation chain, reports to Boost using
 * BOOST_CHECK_* semantics.
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
 * After some minimal pre-processor macro, the check_called() function
 * returns an object of @c report_with_check class.  This produces the
 * following list of validators:
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
 */
template<typename sequence_type>
class report_with_check {
 public:
  typedef typename sequence_type::value_type value_type;
  typedef std::shared_ptr<validator<sequence_type>> pointer;

  report_with_check(
      sequence_type const & sequence, location const & where)
      : validators_()
      , sequence_(sequence)
      , where_(where) {
  }
  ~report_with_check() {
    validate();
  }

  //@{
  /**
   * @name Validator factory functions.
   */
  /// Requires at least (inclusive) this many calls after filtering.
  report_with_check & at_least(std::size_t min) {
    add_validator(pointer(new at_least_validator<sequence_type>(min)));
    return *this;
  }

  /// Requires at most (inclusive) this many calls after filtering.
  report_with_check & at_most(std::size_t max) {
    add_validator(pointer(new at_most_validator<sequence_type>(max)));
    return *this;
  }

  /// Requires exactly this many calls after filtering.
  report_with_check & exactly(std::size_t expected) {
    add_validator(pointer(
        new exactly_validator<sequence_type,false>(expected)));
    return *this;
  }

  /// Requires exactly one call after filtering.
  report_with_check & once() {
    return exactly(1);
  }

  /// Requires no calls after filtering.
  report_with_check & never() {
    add_validator(pointer(
        new exactly_validator<sequence_type,true>(0)));
    return *this;
  }

  /// Requires between @a min and @a max calls, both inclusive, after
  /// filtering.
  report_with_check & between(std::size_t min, std::size_t max) {
    return at_least(min).at_most(max);
  }

  /// Filters to only the calls with the given value.
  template<typename... arg_types>
  report_with_check & with(arg_types... args) {
    auto match = detail::wrap_args_as_tuple(args...);
    static_assert(
        std::is_convertible<decltype(match),value_type>::value,
        "Match expression is not convertible to argument capture");

    add_validator(pointer(
        new equality_filter<sequence_type>(std::move(match))));
    return *this;
  }
  report_with_check & with(value_type && match) {
    return add_validator(pointer(
        new equality_filter<sequence_type>(match)));
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

    validation_result r;
    for (auto i : validators_) {
      r = i->validate(sequence);
      if (not r.pass or r.short_circuit) {
        break;
      }
    }
    if (r.pass) {
      boost::unit_test::unit_test_log.set_checkpoint(
          where_.file, where_.line);
    } else {
      boost::unit_test::framework::assertion_result( false );
      boost::unit_test::unit_test_log
          << boost::unit_test::log::begin(
              where_.file, where_.line)
          << boost::unit_test::log_all_errors
          << r.msg
          << boost::unit_test::log::end();
    }
  }

 private:
  std::list<pointer> validators_;
  sequence_type sequence_;
  location where_;
};

} // namespace detail
} // namespace common
} // namespace mocking
} // namespace e

#endif // escapement_e_mocking_common_detail_boost_reporting_hpp
