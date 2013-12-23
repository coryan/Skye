#define BOOST_TEST_MODULE e_job_spec
#include <boost/test/unit_test.hpp>
#include <e/job_spec.hpp>


BOOST_AUTO_TEST_CASE(job_spec_base)
{
  e::job_spec j("a_program");
  BOOST_CHECK_EQUAL(j.executable(), "a_program");
}

BOOST_AUTO_TEST_CASE(job_spec_args)
{
  e::job_spec j("a_program");

  BOOST_CHECK_EQUAL(j.args().empty(), true);
  j.set_args({"foo", "bar", "baz"});
  BOOST_REQUIRE_EQUAL(j.args().size(), 3);
  BOOST_CHECK_EQUAL(j.args()[0], "foo");
  BOOST_CHECK_EQUAL(j.args()[1], "bar");
  BOOST_CHECK_EQUAL(j.args()[2], "baz");
  e::job_spec::arguments args1({"foo", "bar", "baz"});
  e::job_spec::arguments args2({"qux", "quux"});
  j.set_args(args1).set_args(args2);
  BOOST_REQUIRE_EQUAL(j.args().size(), 2);
  BOOST_CHECK_EQUAL(j.args()[0], "qux");
  BOOST_CHECK_EQUAL(j.args()[1], "quux");
}

BOOST_AUTO_TEST_CASE(job_spec_environment)
{
  e::job_spec j("a_program");

  BOOST_CHECK_EQUAL(j.env().empty(), true);
  j.set_env({{"X", "1"}, {"Y", "2"}, {"bar", "baz"}});
  BOOST_REQUIRE_EQUAL(j.env().count("X"), 1);
  BOOST_REQUIRE_EQUAL(j.env().count("Y"), 1);
  BOOST_REQUIRE_EQUAL(j.env().count("bar"), 1);
  BOOST_CHECK_EQUAL(j.env().at("X"), "1");
  BOOST_CHECK_EQUAL(j.env().at("Y"), "2");
  BOOST_CHECK_EQUAL(j.env().at("bar"), "baz");

  e::job_spec::environment env0({{"foo", "bar"}, {"baz", "qux"}});
  e::job_spec::environment env1({{"X", "2"}, {"Y", "3"}});
  
  j.set_env(env0).set_env(env1);
  BOOST_REQUIRE_EQUAL(j.env().count("foo"), 0);
  BOOST_REQUIRE_EQUAL(j.env().count("X"), 1);
  BOOST_CHECK_EQUAL(j.env().at("X"), "2");
  BOOST_REQUIRE_EQUAL(j.env().count("Y"), 1);
  BOOST_CHECK_EQUAL(j.env().at("Y"), "3");

}

BOOST_AUTO_TEST_CASE(job_spec_setters)
{
  e::job_spec j("a_program");
  BOOST_CHECK_EQUAL(j.stdout(), "");
  BOOST_CHECK_EQUAL(j.stderr(), "");
  BOOST_CHECK_EQUAL(j.wd(), "");
  BOOST_CHECK_EQUAL(j.username(), "");
  BOOST_CHECK_EQUAL(j.groupname(), "");

  j.set_wd("foo").set_stdout("bar")
    .set_stderr("baz")
    .set_username("qux")
    .set_groupname("quux")
    ;

  BOOST_CHECK_EQUAL(j.wd(), "foo");
  BOOST_CHECK_EQUAL(j.stdout(), "bar");
  BOOST_CHECK_EQUAL(j.stderr(), "baz");
  BOOST_CHECK_EQUAL(j.username(), "qux");
  BOOST_CHECK_EQUAL(j.groupname(), "quux");
}
