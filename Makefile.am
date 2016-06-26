# Skye uses a non-recursive make.
# Skye does not use shared libraries so 


noinst_LIBRARIES = \
  skye/lib_skye.a \
  skye/detail/lib_skye.a \
  skye/asio/lib_skye.a \
  skye/asio/detail/lib_skye.a
unit_tests = \
  skye/detail/ut_argument_capture_by_value \
  skye/detail/ut_argument_wrapper \
  skye/detail/ut_unknown_argument_capture_by_value \
  skye/detail/ut_validator \
  skye/ut_conditional_returns \
  skye/ut_conditional_returns_template \
  skye/ut_mock_function \
  skye/ut_mock_template_function
unit_tests_asio = \
  skye/asio/detail/ut_async_function_argument_capture \
  skye/asio/ut_async_io_member_function \
  skye/asio/ut_iterator

examples = \
  examples/tutorials/calculator \
  examples/tutorials/greetings

noinst_PROGRAMS = $(examples)

check_PROGRAMS = $(unit_tests) $(unit_tests_asio)
TESTS = $(check_PROGRAMS)

AM_CXXFLAGS = $(BOOST_CPPFLAGS)
AM_LDFLAGS = $(BOOST_LDFLAGS)

# Common configuration for all unit tests
UT_CPPFLAGS = \
  -DBOOST_TEST_DYN_LINK \
  -DBOOST_TEST_MAIN \
  -DSKYE_USE_BOOST_UNIT_TEST_FRAMEWORK \
  $(CPPFLAGS)
skye_ut_libs = \
  $(BOOST_UNIT_TEST_FRAMEWORK_LIB)
skye_ut_asio_libs = \
  $(BOOST_ASIO_LIB) $(BOOST_SYSTEM_LIB) \
  $(skye_ut_libs)

################################################################
# skye/
################################################################

skye_lib_skye_adir = $(includedir)/skye
skye_lib_skye_a_HEADERS = \
  skye/mock_function.hpp \
  skye/mock_template_function.hpp
skye_lib_skye_a_SOURCES = 
skye_lib_skye_a_LIBADD =

skye_detail_lib_skye_adir = $(includedir)/skye/detail
skye_detail_lib_skye_a_HEADERS = \
  skye/detail/argument_wrapper.hpp \
  skye/detail/assertion_reporting.hpp \
  skye/detail/boost_assertion_reporting.hpp \
  skye/detail/default_return.hpp \
  skye/detail/function_assertion.hpp \
  skye/detail/iostream_assertion_reporting.hpp \
  skye/detail/set_action_proxy.hpp \
  skye/detail/tuple_streaming.hpp \
  skye/detail/unknown_arguments_capture_by_value.hpp \
  skye/detail/validator.hpp 
skye_detail_lib_skye_a_SOURCES = 
skye_detail_lib_skye_a_LIBADD =

skye_ut_conditional_returns_SOURCES = \
  skye/ut_conditional_returns.cpp
skye_ut_conditional_returns_CPPFLAGS = \
  $(UT_CPPFLAGS) \
  -DBOOST_TEST_MODULE=skye_ut_conditional_returns
skye_ut_conditional_returns_LDADD = \
  $(skye_ut_libs)

skye_ut_conditional_returns_template_SOURCES = \
  skye/ut_conditional_returns_template.cpp
skye_ut_conditional_returns_template_CPPFLAGS = \
  $(UT_CPPFLAGS) \
  -DBOOST_TEST_MODULE=skye_ut_conditional_returns_template
skye_ut_conditional_returns_template_LDADD = \
  $(skye_ut_libs)

skye_ut_mock_function_SOURCES = \
  skye/ut_mock_function.cpp
skye_ut_mock_function_CPPFLAGS = \
  $(UT_CPPFLAGS) \
  -DBOOST_TEST_MODULE=skye_ut_mock_function
skye_ut_mock_function_LDADD = \
  $(skye_ut_libs)

skye_ut_mock_template_function_SOURCES = \
  skye/ut_mock_template_function.cpp
skye_ut_mock_template_function_CPPFLAGS = \
  $(UT_CPPFLAGS) \
  -DBOOST_TEST_MODULE=skye_ut_mock_template_function
skye_ut_mock_template_function_LDADD = \
  $(skye_ut_libs)

skye_detail_ut_argument_capture_by_value_SOURCES = \
  skye/detail/ut_argument_capture_by_value.cpp
skye_detail_ut_argument_capture_by_value_CPPFLAGS = \
  $(UT_CPPFLAGS) \
  -DBOOST_TEST_MODULE=skye_detail_ut_argument_capture_by_value
skye_detail_ut_argument_capture_by_value_LDADD = \
  $(skye_ut_libs)

skye_detail_ut_argument_wrapper_SOURCES = \
  skye/detail/ut_argument_wrapper.cpp
skye_detail_ut_argument_wrapper_CPPFLAGS = \
  $(UT_CPPFLAGS) \
  -DBOOST_TEST_MODULE=skye_detail_ut_argument_wrapper
skye_detail_ut_argument_wrapper_LDADD = \
  $(skye_ut_libs)

skye_detail_ut_unknown_argument_capture_by_value_SOURCES = \
  skye/detail/ut_unknown_argument_capture_by_value.cpp
skye_detail_ut_unknown_argument_capture_by_value_CPPFLAGS = \
  $(UT_CPPFLAGS) \
  -DBOOST_TEST_MODULE=skye_detail_ut_unknown_argument_capture_by_value
skye_detail_ut_unknown_argument_capture_by_value_LDADD = \
  $(skye_ut_libs)

skye_detail_ut_validator_SOURCES = \
  skye/detail/ut_validator.cpp
skye_detail_ut_validator_CPPFLAGS = \
  $(UT_CPPFLAGS) \
  -DBOOST_TEST_MODULE=skye_detail_ut_validator
skye_detail_ut_validator_LDADD = \
  $(skye_ut_libs)

################################################################
# skye/asio
################################################################

skye_asio_lib_skye_adir = $(includedir)/skye/asio
skye_asio_lib_skye_a_HEADERS = \
  skye/asio/acceptor.hpp \
  skye/asio/async_accept_member_function.hpp \
  skye/asio/async_connect_member_function.hpp \
  skye/asio/async_io_member_function.hpp \
  skye/asio/endpoint.hpp \
  skye/asio/iterator.hpp \
  skye/asio/protocol.hpp \
  skye/asio/resolver.hpp \
  skye/asio/service.hpp \
  skye/asio/socket.hpp
skye_asio_lib_skye_a_SOURCES =
skye_asio_lib_skye_a_LIBADD = 

skye_asio_detail_lib_skye_asio_adir = $(includedir)/skye/asio/detail
skye_asio_detail_lib_skye_asio_a_HEADERS = \
  skye/asio/detail/async_function_argument_capture.hpp \
  skye/asio/detail/test_service_singleton.hpp
skye_asio_detail_lib_skye_a_SOURCES =
skye_asio_detail_lib_skye_a_LIBADD =

skye_asio_detail_ut_async_function_argument_capture_SOURCES = \
  skye/asio/detail/ut_async_function_argument_capture.cpp
skye_asio_detail_ut_async_function_argument_capture_CPPFLAGS = \
  $(UT_CPPFLAGS) \
  -DBOOST_TEST_MODULE=skye_asio_detail_ut_async_function_argument_capture
skye_asio_detail_ut_async_function_argument_capture_LDADD = \
  $(skye_ut_asio_libs)

skye_asio_ut_async_io_member_function_SOURCES = \
  skye/asio/ut_async_io_member_function.cpp
skye_asio_ut_async_io_member_function_CPPFLAGS = \
  $(UT_CPPFLAGS) \
  -DBOOST_TEST_MODULE=skye_asio_ut_async_io_member_function
skye_asio_ut_async_io_member_function_LDADD = \
  $(skye_ut_asio_libs)

skye_asio_ut_iterator_SOURCES = \
  skye/asio/ut_iterator.cpp
skye_asio_ut_iterator_CPPFLAGS = \
  $(UT_CPPFLAGS) \
  -DBOOST_TEST_MODULE=skye_asio_ut_iterator
skye_asio_ut_iterator_LDADD = \
  $(skye_ut_asio_libs)

################################################################
# examples
################################################################

examples_tutorials_calculator_SOURCES = \
  examples/tutorials/calculator.cpp
examples_tutorials_calculator_CPPFLAGS =
examples_tutorials_calculator_LDADD =

examples_tutorials_greetings_SOURCES = \
  examples/tutorials/greetings.cpp
examples_tutorials_greetings_CPPFLAGS =
examples_tutorials_greetings_LDADD =
