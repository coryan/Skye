#include <e/mocking/common/detail/validator.hpp>

#include <boost/test/unit_test.hpp>

#include <string>
#include <tuple>
#include <utility>
#include <vector>

typedef std::tuple<int,std::string> arguments;
typedef std::vector<arguments> capture_sequence;

using namespace e::mocking::common::detail;

BOOST_AUTO_TEST_CASE( basic_tests ) {
  capture_sequence seq;

  exactly_validator<capture_sequence,false> v1(0);
  validation_result expected{true,false,""};
  BOOST_CHECK_EQUAL(v1.validate(seq), expected);

  exactly_validator<capture_sequence,true> v2(0);
  expected = validation_result{true,true,""};
  BOOST_CHECK_EQUAL(v2.validate(seq), expected);

  seq.push_back(std::make_tuple(1, "foo"));
  seq.push_back(std::make_tuple(1, "foo"));
  seq.push_back(std::make_tuple(2, "bar"));
  seq.push_back(std::make_tuple(1, "foo"));

  at_least_validator<capture_sequence> v3(4);
  expected = validation_result{true,false,""};
  BOOST_CHECK_EQUAL(v3.validate(seq), expected);

  at_least_validator<capture_sequence> v4(7);
  expected = validation_result{false,false,""};
  BOOST_CHECK_EQUAL(v4.validate(seq).pass, expected.pass);

}
