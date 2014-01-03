#include <e/config_files_location.hpp>
#include <e/config.hpp>

#define BOOST_TEST_MODULE e_config_file_location
#include <boost/test/unit_test.hpp>
#include <turtle/mock.hpp>

namespace fs = boost::filesystem;

MOCK_CLASS( mock_getenv )
{
  MOCK_STATIC_METHOD( get, 1, char const*(char const*), get )

public:
  char const * operator()(char const * arg) {
    return mock_getenv::get(arg);
  }
};

MOCK_FUNCTION( mock_validator, 1, bool(fs::path const&), mock_validator )


BOOST_AUTO_TEST_CASE(test_base_function_program_root) {
  mock_getenv env;

  MOCK_RESET( mock_validator );
  MOCK_RESET( env.get );

  MOCK_EXPECT( env.get ).with( "TEST_ROOT" )
      .returns( "/test/path" );
  MOCK_EXPECT( env.get ).with( "ESCAPEMENT_ROOT" )
      .returns( "/install/path" );
  MOCK_EXPECT( mock_validator ).returns( true );

  fs::path etc =
      fs::path(e::sysconfdir()).filename();
  fs::path test_etc = "/test/path" / etc;
  fs::path install_etc = "/install/path" / etc;

  e::config_files_location_base<mock_getenv> locator;

  // Test the case where the TEST_ROOT path is valid ...
  fs::path location = locator(
      "/foo/bar/baz/program", "TEST_ROOT",
      mock_validator);
  BOOST_CHECK_EQUAL(location, test_etc);

  // ... and also when it is invalid ...
  MOCK_RESET( mock_validator );
  MOCK_EXPECT( mock_validator ).with( test_etc ).returns( false );
  MOCK_EXPECT( mock_validator ).with( install_etc ).returns( true );
  location = locator(
      "/foo/bar/baz/program", "TEST_ROOT",
      mock_validator);
  BOOST_CHECK_EQUAL(location, install_etc);
}

BOOST_AUTO_TEST_CASE(test_base_function_escapement_root) {
  mock_getenv env;

  MOCK_RESET( mock_validator );
  MOCK_RESET( env.get );

  MOCK_EXPECT( env.get ).with( "TEST_ROOT" )
      .returns( nullptr );
  MOCK_EXPECT( env.get ).with( "ESCAPEMENT_ROOT" )
      .returns( "/install/path" );
  MOCK_EXPECT( mock_validator ).returns( true );

  fs::path etc =
      fs::path(e::sysconfdir()).filename();
  fs::path install_etc = "/install/path" / etc;

  e::config_files_location_base<mock_getenv> locator;

  // Test the case where the ESCAPEMENT_ROOT path is valid ...
  fs::path location = locator(
      "/foo/bar/baz/program", "TEST_ROOT",
      mock_validator);
  BOOST_CHECK_EQUAL(location, install_etc);

  // ... and also when it is invalid ...
  MOCK_RESET( mock_validator );
  MOCK_EXPECT( mock_validator ).with( install_etc ).returns( false );
  MOCK_EXPECT( mock_validator ).with( e::sysconfdir() ).returns( true );
  location = locator(
      "/foo/bar/baz/program", "TEST_ROOT",
      mock_validator);
  BOOST_CHECK_EQUAL(location, e::sysconfdir() );
}

BOOST_AUTO_TEST_CASE(test_base_function_program) {
  mock_getenv env;

  MOCK_RESET( mock_validator );
  MOCK_RESET( env.get );

  MOCK_EXPECT( env.get ).with( "TEST_ROOT" )
      .returns( nullptr );
  MOCK_EXPECT( env.get ).with( "ESCAPEMENT_ROOT" )
      .returns( nullptr );

  fs::path etc =
      fs::path(e::sysconfdir()).filename();
  fs::path bin =
      fs::path(e::bindir()).filename();
  fs::path program = "/foo/bar/" / bin / "program";
  fs::path program_etc = "/foo/bar/" / etc;

  BOOST_CHECK_MESSAGE(true, "program path = " << program);
  BOOST_CHECK_MESSAGE(true, "program etc path = " << program_etc);

  e::config_files_location_base<mock_getenv> locator;

  // Test the case where the program path is valid ...
  MOCK_RESET( mock_validator );
  MOCK_EXPECT( mock_validator ).with( program_etc ).returns( true );
  MOCK_EXPECT( mock_validator ).with( e::sysconfdir() ).returns( false );
  fs::path location = locator(
      program.string().c_str(), "TEST_ROOT",
      mock_validator);
  BOOST_CHECK_EQUAL(location, program_etc);

  // ... and also when it is invalid, which means all paths are invalid ...
  MOCK_RESET( mock_validator );
  MOCK_EXPECT( mock_validator ).returns( false );
  BOOST_CHECK_THROW(
      location = locator(
          "/foo/bar/baz/program", "TEST_ROOT",
          mock_validator),
      std::runtime_error);
}

BOOST_AUTO_TEST_CASE(test_impl_function) {
  fs::path location;

  location = e::config_files_location_impl(
      "foo/bar/baz", nullptr, "xyz",
      [](fs::path const&) {return true; });
  BOOST_CHECK_MESSAGE(
      location.string().find("xyz") == 0,
      "The location (" << location << ") should start with xyz");

  location = e::config_files_location_impl(
      "foo/bar/baz", "abc", nullptr,
      [](fs::path const&) { return true; });
  BOOST_CHECK_MESSAGE(
      location.string().find("abc") == 0,
      "The location (" << location << ") should start with abc");

  // TODO(ES-24)
  location = e::config_files_location_impl(
      "foo/bar/bin/baz", nullptr, nullptr,
      [](fs::path const&) { return true; });
  BOOST_CHECK_EQUAL(location.string(), e::sysconfdir());

  BOOST_CHECK_THROW(
      location = e::config_files_location_impl(
          "./foo/bar", nullptr, nullptr,
          [](fs::path const&) { return false; }),
      std::runtime_error);
}

