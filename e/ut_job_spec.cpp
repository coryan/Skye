#define BOOST_TEST_MODULE e_job_spec
#include <boost/test/unit_test.hpp>
#include <e/job_spec.hpp>


BOOST_AUTO_TEST_CASE(job_spec_base)
{
  e::job_spec j("a_program");
  BOOST_CHECK_EQUAL(j.executable(), "a_program");
}
