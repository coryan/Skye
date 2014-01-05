#ifndef escapement_e_mocking_common_validator_hpp
#define escapement_e_mocking_common_validator_hpp

#include <memory>
#include <string>
#include <sstream>

#include <boost/test/unit_test.hpp>

namespace e {
namespace mocking {
namespace common {
namespace detail {

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
inline std::ostream & operator<<(std::ostream & os, location const & x) {
  return os << x.function << "@[" << x.file << ":" << x.line << "]";
}
#define ESCAPEMENT_MOCK_LOCATION \
  e::mocking::common::detail::location(__func__, __FILE__, __LINE__)

template<typename sequence_type>
class validator {
 public:
  explicit validator(sequence_type * sequence)
      : sequence_(sequence)
  {}

  virtual bool pass(std::string & msg) const = 0;

  sequence_type * sequence_;
};

template<typename sequence_type>
class trivial_validator : public validator<sequence_type> {
 public:
  explicit trivial_validator(sequence_type * sequence)
      : validator<sequence_type>(sequence)
  {}

  virtual bool pass(std::string &) const {
    return true;
  }
};

template<typename sequence_type>
class at_least_validator : public validator<sequence_type> {
 public:
  explicit at_least_validator(sequence_type * sequence, std::size_t expected)
      : validator<sequence_type>(sequence)
      , expected_(expected)
  {}

  virtual bool pass(std::string & msg) const {
    if (this->sequence_->size() >= expected_) {
      return true;
    }
    std::ostringstream os;
    os << "failed validation, expected at least " 
       << expected_ << " invocations, but only "
       << this->sequence_->size() << " where present.";
    msg = os.str();
    return false;
  }

 private:
  std::size_t expected_;
};

template<typename sequence_type>
class report_with_check {
 public:
  report_with_check(sequence_type * sequence, location const & where)
      : validator_(new trivial_validator<sequence_type>(sequence))
      , where_(where)
  {
  }
  ~report_with_check() {
    std::string msg;
    bool result = validator_->pass(msg);
    if (result) {
      boost::unit_test::unit_test_log.set_checkpoint(
          where_.file, where_.line);
      return;
    }
    boost::unit_test::framework::assertion_result( false );
    boost::unit_test::unit_test_log
        << boost::unit_test::log::begin(
            where_.file, where_.line)
        << boost::unit_test::log_all_errors
        << msg
        << boost::unit_test::log::end();
  }

  report_with_check & at_least(std::size_t expected) {
    validator_.reset(
        new at_least_validator<sequence_type>(
            validator_->sequence_, expected));
    return *this;
  }

 private:
  std::shared_ptr<validator<sequence_type>> validator_;
  location where_;
};


} // namespace detail
} // namespace common
} // namespace mocking
} // namespace e

#endif // escapement_e_mocking_common_validator_hpp
