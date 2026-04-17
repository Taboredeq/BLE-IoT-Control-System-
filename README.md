# ESP32 BLE & IoT Control System

## Overview
This project is an embedded system based on ESP32 that combines **BLE communication, WiFi IoT control (Blynk), and hardware interaction** to enable remote control of a PC and connected peripherals.

The system allows:
- Remote PC power control via relay
- Wireless command transmission using BLE HID (keyboard emulation)
- Audio output switching (headphones / speakers)
- IoT-based control using the Blynk platform

---

## Features

### BLE Communication (HID)
- ESP32 acts as a Bluetooth Low Energy keyboard (BLE HID)
- Sends keyboard shortcuts to PC (e.g., media control, system commands)
- Real-time command execution from hardware inputs

### IoT Control (Blynk + WiFi)
- Remote control via Blynk mobile application
- WiFi-based communication with ESP32
- Virtual pins used for system control logic

### Hardware Integration
- Relay-based PC power control
- Physical input detection (e.g., headphone state switch)
- GPIO-based event handling

### System Reliability
- Watchdog timer implementation (ESP32 task watchdog)
- Event-driven architecture for stable operation

---

## Hardware Used
- ESP32 Dev Board
- Relay module
- Push buttons / input sensors
- PC (BLE HID receiver)

---

## Software Stack
- Arduino framework (ESP32)
- BLE Keyboard library
- Blynk IoT platform
- FreeRTOS / ESP-IDF components (watchdog)

---

## Example Use Cases
- Remote PC power on/off
- Switching audio output (headphones ↔ speakers)
- Sending system shortcuts via BLE
- Remote IoT device control via smartphone


---

## What I learned
- BLE HID communication on ESP32
- IoT integration using Blynk platform
- Embedded system reliability techniques
- Real-time event-driven firmware design

---

## Future Improvements
- Replace Blynk with custom MQTT backend
- Add encrypted BLE communication
- Improve state machine architecture
- Expand support for multiple devices

---

## Author
Norbert Boroń
Embedded Systems & Automation Engineering Student
