# Smart Cattle Shelter Automation System (ESP32 + Web Dashboard)

## Overview
This project implements a smart automation system for cattle shelters using the ESP32.  
The system automatically controls ventilation, lighting, and roofing based on environmental conditions.  
It also provides a real-time monitoring dashboard accessible through any web browser over Wi-Fi.

## Features
- **Temperature Monitoring (DHT11)** – Automatically controls the fan and relay.
- **Light Monitoring (LDR)** – Turns lights on/off based on light intensity.
- **Rain Detection (Rain Sensor)** – Closes or opens the roof using a servo motor.
- **Web Dashboard** – Displays real-time data (temperature, light levels, rain status).
- **Wi-Fi Connectivity** – ESP32 hosts a local web server for control and monitoring.

## Hardware Components
- ESP32-S2 or ESP32 Development Board  
- DHT11 Temperature & Humidity Sensor  
- LDR (Light Dependent Resistor)  
- Rain Sensor Module  
- Servo Motor (for automated roof control)  
- Relay Module (for fan control)  
- LEDs (for fan and light simulation)  
- Power Supply(External would be better) and Connecting Wires  

## Software Requirements
- Arduino IDE (with ESP32 board support installed)  
- Required Libraries:
 - DHT sensor library
 - WebServer
 - ArduinoJson
 - Esp32Servo

## Installation
1. Clone this repository:
   ```bash
   git clone https://github.com/<your-username>/smart-cattle-shelter.git
Open the project in Arduino IDE.

Install the required libraries via Library Manager or manually from GitHub.

Select your ESP32-S2 (or ESP32 Dev Board) under Tools > Board.

Update your Wi-Fi SSID and Password inside the code (const char* ssid and const char* password).

Upload the code to your ESP32.

## Usage
Power up the ESP32 with the connected sensors and actuators.

Open the Serial Monitor to confirm system initialization.

Connect your computer/phone to the same Wi-Fi network as ESP32.

Open a browser and enter the IP address shown in the Serial Monitor.

The web dashboard will display live sensor readings and system status.


## Future Improvements

Cloud-based data logging (Google Sheets or Firebase).

Mobile app integration for remote monitoring.

Advanced analytics with environmental history.

# Author

Adarsha K M

Electronics & Communication Engineering, 
G M Institute of Technology, VTU
