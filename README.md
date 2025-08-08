# Optimization-of-overloading-and-underloading-of-wagon-using-IoT
An IoT-based system designed to enhance the efficiency and safety of railway wagon loading by measuring real-time weight distribution using load cells and ESP32 modules. This project helps prevent overloading and ensures better resource utilization through smart monitoring and alerts.

📌 Project Overview
Traditional methods of monitoring railway wagon loading are inefficient and prone to human error. This project aims to automate and optimize the loading process using IoT technologies.
It consists of a master-slave architecture where:
          1. Each slave ESP32 collects load data from multiple load cells installed under wagon bases.
          2. A master ESP32 polls the slaves and aggregates data.
          3. If the load exceeds a defined threshold, the system triggers an overload warning via a buzzer and displays the message on an OLED screen.

🔧 Features
        📦 Real-time weight monitoring from load cells
        🔁 Master-slave communication using ESP-NOW
        🚨 Automatic overloading alerts via buzzer
        📺 OLED display showing live weight and warnings
        🧠 Scalable architecture for multiple wagons
        ⚡ Low power consumption using ESP32

📡 Protocols Used
The following protocols are implemented in this project for seamless communication and data handling:

Protocol	                                             Purpose
ESP-NOW	                          Wireless peer-to-peer communication between ESP32 master and slaves.
I2C	                              Communication between master ESP32 and OLED display (SSD1306).
Digital GPIO	                    Used for triggering buzzer and reading digital output from HX711 modules.
HX711 Serial Protocol	            2-wire protocol to read data from load cells via HX711.
