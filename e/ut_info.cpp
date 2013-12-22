#define BOOST_TEST_MODULE e_info
#include <boost/test/unit_test.hpp>
#include <e/info.hpp>
#include <e/e-config.hpp>


BOOST_AUTO_TEST_CASE(info_version) {
  BOOST_CHECK_EQUAL(e::version(), std::string(VERSION));
}
