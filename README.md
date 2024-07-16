# ESP32 Relay and External LED Control

This project demonstrates how to control a relay and an external LED using an ESP32 via a web server. The ESP32 connects to a WiFi network and hosts a web server that allows users to turn the relay on and off. When the relay is activated, an external LED connected to GPIO 33 is also turned on. The project includes basic logging functionality and ensures responsive design for mobile access.

## Features
- Control relay via web interface
- External LED indication when relay is active
- Responsive design for mobile devices
- Simple logging mechanism for connection and control status

## Components Used
- ESP32
- Relay module
- External LED
- 220 ohm resistor
- Breadboard and jumper wires

## How to Use
1. Connect the ESP32 to the relay module and external LED as per the provided schematic.
2. Upload the code to the ESP32.
3. Connect to the WiFi network specified in the code.
4. Access the web server hosted by the ESP32 to control the relay and LED.

## Schematic
- ESP32 GPIO 33 --> 220 ohm Resistor --> Anode of LED
- Cathode of LED --> GND
- ESP32 GPIO 27 --> Relay module IN
- Relay module VCC --> 5V
- Relay module GND --> GND
