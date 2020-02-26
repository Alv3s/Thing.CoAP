bin_PROGRAMS=Server-InlineEndpoints Client Client-Discovery

Server_InlineEndpoints_SOURCES=Linux/Server-InlineEndpoints/Server-InlineEndpoints.cpp
Server_InlineEndpoints_CXXFLAGS=-std=c++11 -I$(top_srcdir)
Server_InlineEndpoints_LDADD= ../Thing.CoAP/libthingcoap.la
Server_InlineEndpoints_LDFLAGS=-pthread

Client_SOURCES=Linux/Client/Client.cpp
Client_CXXFLAGS=-std=c++11 -I$(top_srcdir)
Client_LDADD= ../Thing.CoAP/libthingcoap.la
Client_LDFLAGS=-pthread

Client_Discovery_SOURCES=Linux/Client-Discovery/Client-Discovery.cpp
Client_Discovery_CXXFLAGS=-std=c++11 -I$(top_srcdir)
Client_Discovery_LDADD= ../Thing.CoAP/libthingcoap.la
Client_Discovery_LDFLAGS=-pthread