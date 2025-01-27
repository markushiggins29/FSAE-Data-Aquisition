# FSAE-Data-Aquisition
This repository contains the software for my senior capstone project, a telemetry system for a Formula Electric Vehicle. The project contains event-driven software for two or more microcontrollers that follow a master-slave architecture.

The "master" is the monitor board, which collects data from the "slaves," or peripheral boards via CAN and stores the data locally in addition to broadcasting the information over WiFi. The monitor board contains an ESP32 microcontroller while
the peripheral boards utilize an Adafruit Feather M4 CAN microcontroller. Each peripheral board may accomadate up to 6 sensors that follow a standard power, signal, pin architecture and are powered by either 3.3V or 5V. Additional peripheral boards
can be easily integrated into the system up to the baudrate of the CAN protocol. 
