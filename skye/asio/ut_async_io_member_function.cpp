#include <skye/asio/async_io_member_function.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/asio/streambuf.hpp>


// We normally do not write test code for test code, but these mock
// classes are complicated, and we want them to at least compile
// before we use them in real tests.

using namespace e::mocking::asio;

BOOST_AUTO_TEST_CASE( async_write_member_function_ctor ) {
  async_write_member_function async_write_some;
  BOOST_CHECK(not async_write_some.has_calls());
}

BOOST_AUTO_TEST_CASE( async_write_member_function_with_calls ) {
  char const msg[] = "TEST\n";

  async_write_member_function amf;
  int counter = 0;
  std::size_t total_bt = 0;

  amf(
      boost::asio::buffer(msg, sizeof(msg)-1),
      [&counter,&total_bt](
          boost::system::error_code const & ec, std::size_t bt) {
        counter++;
        total_bt += bt;
      });

  BOOST_CHECK_EQUAL(amf.has_calls(), 1);
  BOOST_CHECK_EQUAL(
      amf.at(0)->get_buffer_size(), sizeof(msg) - 1);
  BOOST_CHECK_EQUAL(
      amf.at(0)->get_buffer_data(), static_cast<void const*>(msg));

  amf.at(0)->call_functor(boost::system::error_code(), 2);
  BOOST_CHECK_EQUAL(counter, 1);
  BOOST_CHECK_EQUAL(total_bt, 2);
}

BOOST_AUTO_TEST_CASE( async_read_member_function_ctor ) {
  async_read_member_function async_read_some;
  BOOST_CHECK(not async_read_some.has_calls());
}

BOOST_AUTO_TEST_CASE( async_read_member_function_with_calls ) {
  std::size_t const bufsize = 1024;
  char raw[bufsize];

  async_read_member_function amf;
  int counter = 0;
  std::size_t total_bt = 0;

  amf(
      boost::asio::buffer(raw, bufsize),
      [&counter,&total_bt](
          boost::system::error_code const & ec, std::size_t bt) {
        counter++;
        total_bt += bt;
      });

  BOOST_CHECK_EQUAL(amf.has_calls(), 1);
  BOOST_CHECK_EQUAL(amf.at(0)->get_buffer_size(), bufsize);
  BOOST_CHECK_EQUAL(amf.at(0)->get_buffer_data(), raw);

  amf.at(0)->call_functor(boost::system::error_code(), 2);
  BOOST_CHECK_EQUAL(counter, 1);
  BOOST_CHECK_EQUAL(total_bt, 2);

  char const msg[] = "TEST\n";
  amf.at(0)->set_buffer_data(msg, sizeof(msg));

  BOOST_CHECK_EQUAL(std::string(raw), std::string(msg));
}

