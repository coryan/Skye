# Skye uses a non-recursive make.
# Skye does not use shared libraries so 


noinst_LIBRARIES = \
  skye/common/lib_skye.a \
  skye/asio/lib_skye_asio.a
unit_tests_common = \
  skye/common/detail/ut_argument_capture_by_value \
  skye/common/detail/ut_argument_wrapper \
  skye/common/detail/ut_unknown_argument_capture_by_value \
  skye/common/detail/ut_validator \
  skye/common/ut_mock_function \
  skye/common/ut_mock_template_function
unit_tests_asio = \
  skye/asio/detail/ut_async_function_argument_capture \
  skye/asio/ut_async_io_member_function \
  skye/asio/ut_iterator

check_PROGRAMS = $(unit_tests_common) $(unit_tests_asio)
TESTS = $(check_PROGRAMS)

AM_CXXFLAGS = $(BOOST_CPPFLAGS)
AM_LDFLAGS = $(BOOST_LDFLAGS)

# Common configuration for all unit tests
UT_CPPFLAGS = -DBOOST_TEST_DYN_LINK -DBOOST_TEST_MAIN
skye_ut_libs = \
  skye/common/lib_skye.a \
  $(BOOST_UNIT_TEST_FRAMEWORK_LIB)
skye_ut_asio_libs = \
  skye/asio/lib_skye_asio.a \
  $(BOOST_ASIO_LIB) $(BOOST_SYSTEM_LIB) \
  $(skye_ut_libs)

################################################################
# skye/common
################################################################

skye_common_lib_skye_adir = $(includedir)/skye
skye_common_lib_skye_a_HEADERS = \
  skye/common/mock_function.hpp \
  skye/common/mock_template_function.hpp \
  skye/common/detail/argument_wrapper.hpp \
  skye/common/detail/mock_returner.hpp \
  skye/common/detail/tuple_streaming.hpp \
  skye/common/detail/validator.hpp 
skye_common_lib_skye_a_SOURCES = 
skye_common_lib_skye_a_LIBADD =

skye_common_detail_ut_argument_capture_by_value_SOURCES = \
  skye/common/detail/ut_argument_capture_by_value.cpp
skye_common_detail_ut_argument_capture_by_value_CPPFLAGS = \
  $(UT_CPPFLAGS) \
  -DBOOST_TEST_MODULE=skye_common_detail_ut_argument_capture_by_value
skye_common_detail_ut_argument_capture_by_value_LDADD = \
  $(skye_ut_libs)

skye_common_detail_ut_argument_wrapper_SOURCES = \
  skye/common/detail/ut_argument_wrapper.cpp
skye_common_detail_ut_argument_wrapper_CPPFLAGS = \
  $(UT_CPPFLAGS) \
  -DBOOST_TEST_MODULE=skye_common_detail_ut_argument_wrapper
skye_common_detail_ut_argument_wrapper_LDADD = \
  $(skye_ut_libs)

skye_common_detail_ut_unknown_argument_capture_by_value_SOURCES = \
  skye/common/detail/ut_unknown_argument_capture_by_value.cpp
skye_common_detail_ut_unknown_argument_capture_by_value_CPPFLAGS = \
  $(UT_CPPFLAGS) \
  -DBOOST_TEST_MODULE=skye_common_detail_ut_unknown_argument_capture_by_value
skye_common_detail_ut_unknown_argument_capture_by_value_LDADD = \
  $(skye_ut_libs)

skye_common_detail_ut_validator_SOURCES = \
  skye/common/detail/ut_validator.cpp
skye_common_detail_ut_validator_CPPFLAGS = \
  $(UT_CPPFLAGS) \
  -DBOOST_TEST_MODULE=skye_common_detail_ut_validator
skye_common_detail_ut_validator_LDADD = \
  $(skye_ut_libs)

skye_common_ut_mock_function_SOURCES = \
  skye/common/ut_mock_function.cpp
skye_common_ut_mock_function_CPPFLAGS = \
  $(UT_CPPFLAGS) \
  -DBOOST_TEST_MODULE=skye_common_ut_mock_function
skye_common_ut_mock_function_LDADD = \
  $(skye_ut_libs)

skye_common_ut_mock_template_function_SOURCES = \
  skye/common/ut_mock_template_function.cpp
skye_common_ut_mock_template_function_CPPFLAGS = \
  $(UT_CPPFLAGS) \
  -DBOOST_TEST_MODULE=skye_common_ut_mock_template_function
skye_common_ut_mock_template_function_LDADD = \
  $(skye_ut_libs)

################################################################
# skye/asio
################################################################

skye_asio_lib_skye_asio_adir = $(includedir)/e
skye_asio_lib_skye_asio_a_HEADERS = \
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
skye_asio_lib_skye_asio_a_SOURCES = \
  skye/asio/acceptor.cpp \
  skye/asio/service.cpp \
  skye/asio/socket.cpp
skye_asio_lib_skye_asio_a_LIBADD = \
  $(BOOST_ASIO_LIB)

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
