check_LTLIBRARIES = libgtest.la libgmock.la libgtest_main.la libgmock_main.la

libgtest_la_SOURCES = ../thirdparty/googletest/googletest/src/gtest-all.cc
libgtest_la_CPPFLAGS = -I$(top_srcdir)/thirdparty/googletest/googletest/include -I$(top_srcdir)/thirdparty/googletest/googletest
libgtest_la_LDFLAGS = -pthread

libgmock_la_SOURCES = ../thirdparty/googletest/googlemock/src/gmock-all.cc
libgmock_la_CPPFLAGS = -I$(top_srcdir)/thirdparty/googletest/googlemock/include -I$(top_srcdir)/thirdparty/googletest/googletest/include -I$(top_srcdir)/thirdparty/googletest/googlemock
libgmock_la_LDFLAGS = -pthread

libgtest_main_la_SOURCES = ../thirdparty/googletest/googletest/src/gtest_main.cc
libgtest_main_la_CPPFLAGS = -I$(top_srcdir)/thirdparty/googletest/googletest/include -I$(top_srcdir)/thirdparty/googletest/googletest
libgtest_main_la_LIBADD = libgtest.la
libgtest_main_la_LDFLAGS = -pthread

libgmock_main_la_SOURCES = ../thirdparty/googletest/googlemock/src/gmock_main.cc
libgmock_main_la_CPPFLAGS = -I$(top_srcdir)/thirdparty/googletest/googlemock/include -I$(top_srcdir)/thirdparty/googletest/googletest/include -I$(top_srcdir)/thirdparty/googletest/googlemock
libgmock_main_la_LIBADD = libgmock.la
libgmock_main_la_LDFLAGS = -pthread

check_PROGRAMS = ClientTest ObserverTest OptionsTest PacketTest ResourceBaseTest ServerTest StatusTest WebLinkTest
ClientTest_SOURCES = ClientTest.cpp
ClientTest_LDADD = libgtest.la libgmock.la libgtest_main.la libgmock_main.la ../Thing.CoAP/libthingcoap.la
ClientTest_LDFLAGS = -pthread
ClientTest_CPPFLAGS = -I$(top_srcdir)/thirdparty/googletest/googletest/include -I$(top_srcdir)/thirdparty/googletest/googlemock/include -I$(top_srcdir)/thirdparty/googletest/googletest -pthread -I$(top_srcdir)

ObserverTest_SOURCES = ObserverTest.cpp
ObserverTest_LDADD = libgtest.la libgmock.la libgtest_main.la libgmock_main.la ../Thing.CoAP/libthingcoap.la
ObserverTest_LDFLAGS = -pthread
ObserverTest_CPPFLAGS = -I$(top_srcdir)/thirdparty/googletest/googletest/include -I$(top_srcdir)/thirdparty/googletest/googlemock/include -I$(top_srcdir)/thirdparty/googletest/googletest -pthread -I$(top_srcdir)

OptionsTest_SOURCES = OptionsTest.cpp
OptionsTest_LDADD = libgtest.la libgmock.la libgtest_main.la libgmock_main.la ../Thing.CoAP/libthingcoap.la
OptionsTest_LDFLAGS = -pthread
OptionsTest_CPPFLAGS = -I$(top_srcdir)/thirdparty/googletest/googletest/include -I$(top_srcdir)/thirdparty/googletest/googlemock/include -I$(top_srcdir)/thirdparty/googletest/googletest -pthread -I$(top_srcdir)

PacketTest_SOURCES = PacketTest.cpp
PacketTest_LDADD = libgtest.la libgmock.la libgtest_main.la libgmock_main.la ../Thing.CoAP/libthingcoap.la
PacketTest_LDFLAGS = -pthread
PacketTest_CPPFLAGS = -I$(top_srcdir)/thirdparty/googletest/googletest/include -I$(top_srcdir)/thirdparty/googletest/googlemock/include -I$(top_srcdir)/thirdparty/googletest/googletest -pthread -I$(top_srcdir)

ResourceBaseTest_SOURCES = ResourceBaseTest.cpp
ResourceBaseTest_LDADD = libgtest.la libgmock.la libgtest_main.la libgmock_main.la ../Thing.CoAP/libthingcoap.la
ResourceBaseTest_LDFLAGS = -pthread
ResourceBaseTest_CPPFLAGS = -I$(top_srcdir)/thirdparty/googletest/googletest/include -I$(top_srcdir)/thirdparty/googletest/googlemock/include -I$(top_srcdir)/thirdparty/googletest/googletest -pthread -I$(top_srcdir)

ServerTest_SOURCES = ServerTest.cpp
ServerTest_LDADD = libgtest.la libgmock.la libgtest_main.la libgmock_main.la ../Thing.CoAP/libthingcoap.la
ServerTest_LDFLAGS = -pthread
ServerTest_CPPFLAGS = -I$(top_srcdir)/thirdparty/googletest/googletest/include -I$(top_srcdir)/thirdparty/googletest/googlemock/include -I$(top_srcdir)/thirdparty/googletest/googletest -pthread -I$(top_srcdir)

StatusTest_SOURCES = StatusTest.cpp
StatusTest_LDADD = libgtest.la libgmock.la libgtest_main.la libgmock_main.la ../Thing.CoAP/libthingcoap.la
StatusTest_LDFLAGS = -pthread
StatusTest_CPPFLAGS = -I$(top_srcdir)/thirdparty/googletest/googletest/include -I$(top_srcdir)/thirdparty/googletest/googlemock/include -I$(top_srcdir)/thirdparty/googletest/googletest -pthread -I$(top_srcdir)

WebLinkTest_SOURCES = WebLinkTest.cpp
WebLinkTest_LDADD = libgtest.la libgmock.la libgtest_main.la libgmock_main.la ../Thing.CoAP/libthingcoap.la
WebLinkTest_LDFLAGS = -pthread
WebLinkTest_CPPFLAGS = -I$(top_srcdir)/thirdparty/googletest/googletest/include -I$(top_srcdir)/thirdparty/googletest/googlemock/include -I$(top_srcdir)/thirdparty/googletest/googletest -pthread -I$(top_srcdir)

TESTS = $(check_PROGRAMS)