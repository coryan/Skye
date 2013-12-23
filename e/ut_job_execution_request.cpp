#define BOOST_TEST_MODULE e_job_execution_request
#include <boost/test/unit_test.hpp>
#include <e/job_execution_request.hpp>

using namespace e;

BOOST_AUTO_TEST_CASE(basics) {
  job_spec job("/foo/bar/baz");
  job.set_args({"a", "b"}).set_env({{"Z", ""}, {"U", "1:2:3"}});

  job_execution_request request("foo:1234/bar:2345", job);
  BOOST_CHECK_EQUAL(request, request);
  BOOST_CHECK_EQUAL(request.id(), "foo:1234/bar:2345");
  BOOST_CHECK_EQUAL(request.job().executable(), "/foo/bar/baz");

  job_execution_request other("foo:1234/bar:4567", job);
  BOOST_CHECK_LT(request, other);
}
