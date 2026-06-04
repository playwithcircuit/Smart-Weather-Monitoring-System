# Smart-Weather-Monitoring-System

Weather monitoring plays an important role in agriculture, environmental studies, industrial automation, and smart home applications. This project demonstrates how multiple sensors can be interfaced directly with an ESP32-S3 Smart Display to continuously monitor environmental parameters such as temperature, humidity, atmospheric pressure, altitude, rainfall, and ambient light intensity and display the information on a modern graphical dashboard.

The graphical user interface is built using the LVGL library, providing an interactive and visually appealing experience.

---

## Features

* Real-time Temperature Monitoring
* Relative Humidity Measurement
* Atmospheric Pressure Measurement
* Altitude Estimation
* Rain Detection
* Automatic Day/Night Detection
* LVGL-Based Graphical Dashboard
* ESP-IDF Framework

---

## Hardware Used

| Component              | Purpose                                        |
| ---------------------- | ---------------------------------------------- |
| ESP32-S3 Smart Display | Main Controller and Display                    |
| BME280 Sensor          | Temperature, Humidity and Pressure Measurement |
| Rain Sensor            | Rainfall Detection                             |
| LDR Sensor             | Day/Night Detection                            |
| USB Cable              | Power Supply and Programming                   |

---

## Parameters Monitored

* Temperature (°C)
* Humidity (%)
* Atmospheric Pressure (hPa)
* Altitude (m)
* Rain Status
* Day/Night Status

---

## Working Principle

The Smart Weather Monitoring System continuously acquires environmental data using multiple sensors.

The BME280 sensor measures temperature, humidity, and atmospheric pressure through the I2C interface. Altitude is estimated from atmospheric pressure using the barometric formula.

The Rain Sensor and LDR sensor provide analog voltage outputs that are converted into digital values using the ESP32-S3 ADC module. Based on predefined threshold values, the software determines whether it is raining and whether the surrounding environment corresponds to day or night.

The sensor data is processed by the ESP32-S3 and displayed on a graphical dashboard developed using the LVGL library.

---

## Software Requirements

* Visual Studio Code
* ESP-IDF Extension
* ESP-IDF Framework
* LVGL Library
* BME280 Driver Library

---

## Circuit Connections
![Wiring Connections](https://playwithcircuit.com/wp-content/uploads/2026/06/Circuit-Diagram-for-ESP32-based-Weather-Monitoring-System.webp)

### BME280 Sensor

| BME280 Pin | ESP32-S3 GPIO |
| ---------- | ------------- |
| VCC        | 3.3V          |
| GND        | GND           |
| SDA        | GPIO9         |
| SCL        | GPIO10        |

### Rain Sensor

| Rain Sensor Pin | ESP32-S3 GPIO |
| --------------- | ------------- |
| VCC             | 3.3V          |
| GND             | GND           |
| AO              | GPIO7         |

### LDR Sensor

| LDR Pin | ESP32-S3 GPIO |
| ------- | ------------- |
| VCC     | 3.3V          |
| GND     | GND           |
| AO      | GPIO6         |

---

## Software Architecture

The software is divided into two major modules:

### main.cpp

Responsible for:

* Display initialization
* LVGL GUI creation
* Dashboard card creation
* Updating sensor values
* Managing graphical objects

### sensors.c

Responsible for:

* I2C initialization
* BME280 communication
* ADC configuration
* Rain detection
* Day/Night detection
* Environmental data acquisition

---

## How It Works

1. ESP32-S3 initializes the display and sensors.
2. BME280 measures temperature, humidity, and pressure.
3. Altitude is calculated from atmospheric pressure.
4. Rain sensor determines whether it is raining.
5. LDR sensor determines day or night conditions.
6. Sensor values are continuously updated on the LVGL dashboard.

---

## Applications

* Home Weather Station
* Smart Agriculture
* Greenhouse Monitoring
* Environmental Monitoring
* Educational Projects
* IoT Applications
* Industrial Monitoring
* Research and Development

---

## Copyright Notice

Copyright © 2026 playwithcircuit.com

This project is provided for educational and personal use only.

Redistribution, commercial use, reproduction of source files, diagrams, images, or article content without written permission from PlayWithCircuit.com is prohibited.

## Original Article

For detailed explanation, circuit diagrams, code walkthrough, troubleshooting guide, and complete theory, visit:

[https://playwithcircuit.com](https://playwithcircuit.com/how-to-build-a-weather-monitoring-system-using-esp32-s3-smart-display-and-lvgl/)
