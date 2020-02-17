# Get the base Ubuntu image from Docker Hub
FROM ubuntu:18.04

# Install curl
RUN apt-get update && apt-get install -y curl

#install Python
RUN apt-get install -y python3
RUN apt-get install -y python
RUN apt-get install -y python-pip
RUN pip install pyserial

# Install Arduino
RUN curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | BINDIR=/usr/bin sh

# Install Arduino ESP8266
RUN arduino-cli core update-index --additional-urls https://arduino.esp8266.com/stable/package_esp8266com_index.json
RUN arduino-cli core install esp8266:esp8266 --additional-urls http://arduino.esp8266.com/stable/package_esp8266com_index.json

# Install Arduino ESP32
RUN arduino-cli core update-index --additional-urls https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
RUN arduino-cli core install esp32:esp32 --additional-urls https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json


RUN mkdir -p /root/Arduino/libraries/Thing.CoAP
WORKDIR /root/Arduino/libraries/Thing.CoAP