# Spinning Top – Smart Control System
This is the Git repository of my first project at the Technion – Spinning Top.

**Note:** This repository contains only the code components of the project (Android app and Arduino firmware). It does not include mechanical drawings, PCB design files, or the full project documentation.

The goal of this project was to combine several previous developments into a single integrated product: a controllable spinning top. We focused on the mechanical structure and an Android application that communicates with the spinning top in real-time using an ESP32 microcontroller.

## Project Summary
We designed a system that includes:

- An Android application for control and data display

- Arduino/ESP32 firmware for sensor reading and communication

- Supported by custom hardware and mechanics

## Android Application
- Developed using Android Studio in Java

- Connects to the ESP32 using WebSocket over Wi-Fi

- Displays graphs for acceleration, angle, and distance from the surface

- Handles connection status and error messages

## Arduino/ESP32 Firmware
- Written in C++ using the Arduino framework

- Uses VL53L0X and MPU6050 sensors

- Sends data to the app using WebSocket

- Designed for efficient memory usage and fast communication

## Sensors Used
- VL53L0X: Time-of-Flight sensor for ground distance measurement

- MPU6050: Measures acceleration and rotation

## Repository Structure
`/Android` - Android Studio project source code

`/Arduino` - ESP32 firmware code

## Achievements
- Real-time communication between Android app and ESP32

- Live graphical display of sensor values

- Fully integrated hardware and software system for a working spinning top

