#include <e/mocking/common/detail/validator.hpp>

#include <boost/test/unit_test.hpp>

#include <string>
#include <tuple>
#include <utility>
#include <vector>

typedef std::tuple<int,std::string> arguments;
typedef std::vector<arguments> capture_sequence;

using namespace e::mocking::common::detail;


BOOST_AUTO_TEST_CASE( test_equality_filter ) {
  capture_sequence seq{
    std::make_tuple(1, "foo"),
    std::make_tuple(1, "foo"),
    std::make_tuple(1, "bar"),
    std::make_tuple(2, "bar"),
    std::make_tuple(3, "bar"),
    std::make_tuple(4, "foo"),
    std::make_tuple(5, "foo"),
    std::make_tuple(1, "foo")};

  equality_filter<capture_sequence> filter1(std::make_tuple(2, "bar"));
  capture_sequence tmp = seq;
  filter1.filter(tmp);
  BOOST_CHECK_EQUAL(tmp.size(), 1);

  equality_filter<capture_sequence> filter2(std::make_tuple(1, "foo"));
  tmp = seq;
  filter2.filter(tmp);
  BOOST_CHECK_EQUAL(tmp.size(), 3);


}

BOOST_AUTO_TEST_CASE( test_at_least_validator ) {
  capture_sequence seq{
    std::make_tuple(1, "foo"),
    std::make_tuple(1, "foo"),
    std::make_tuple(2, "bar"),
    std::make_tuple(1, "foo")};

  at_least_validator<capture_sequence> v_pass(4);
  auto expected = validation_result{true,false,""};
  BOOST_CHECK_EQUAL(v_pass.validate(seq), expected);

  at_least_validator<capture_sequence> v_fail(7);
  expected = validation_result{false,false,""};
  BOOST_CHECK_EQUAL(v_fail.validate(seq).pass, expected.pass);

  v_pass.filter(seq);
  BOOST_CHECK_EQUAL(seq.size(), 4);
}

BOOST_AUTO_TEST_CASE( test_at_most_validator ) {
  capture_sequence seq{
    std::make_tuple(1, "foo"),
    std::make_tuple(1, "foo"),
    std::make_tuple(2, "bar"),
    std::make_tuple(1, "foo")};

  at_most_validator<capture_sequence> v_pass(4);
  auto expected = validation_result{true,false,""};
  BOOST_CHECK_EQUAL(v_pass.validate(seq), expected);

  at_most_validator<capture_sequence> v_fail(2);
  expected = validation_result{false,false,""};
  BOOST_CHECK_EQUAL(v_fail.validate(seq).pass, expected.pass);

  v_pass.filter(seq);
  BOOST_CHECK_EQUAL(seq.size(), 4);
}

BOOST_AUTO_TEST_CASE( test_exactly_validator ) {
  capture_sequence seq;

  exactly_validator<capture_sequence,false> v_pass(0);
  auto expected = validation_result{true,false,""};
  BOOST_CHECK_EQUAL(v_pass.validate(seq), expected);

  exactly_validator<capture_sequence,true> v_pass_sc(0);
  expected = validation_result{true,true,""};
  BOOST_CHECK_EQUAL(v_pass_sc.validate(seq), expected);

  seq.push_back(std::make_tuple(1, "foo"));
  seq.push_back(std::make_tuple(1, "foo"));
  seq.push_back(std::make_tuple(2, "bar"));
  seq.push_back(std::make_tuple(1, "foo"));

  exactly_validator<capture_sequence,false> v_pass_nsc(4);
  expected = validation_result{true,false,""};
  BOOST_CHECK_EQUAL(v_pass_nsc.validate(seq), expected);

  exactly_validator<capture_sequence,false> v_fail_nsc(7);
  expected = validation_result{false,false,""};
  BOOST_CHECK_EQUAL(v_fail_nsc.validate(seq).pass, expected.pass);

  v_pass.filter(seq);
  BOOST_CHECK_EQUAL(seq.size(), 4);
}


