# Get the base Ubuntu image from Docker Hub
FROM ubuntu:18.04

# Update apps on the base image
RUN apt-get update -y

# Install build utilities
RUN apt-get update && apt-get install build-essential -y --fix-missing
RUN apt-get install autoconf -y
RUN apt-get install automake 
RUN apt-get install libtool -y

WORKDIR /Thing.CoAP

CMD autoreconf -i && cd build-aux/ && ../configure && make