# Face Tracker

----

## Requirements:

+ ESP32
+ Webcam
+ 2 Servos
+ 2.4GHz Network

> Python requirements in requirements.txt

> Arduino Requirements are at the top of esp32.ino

----

## Setup:

1) In esp32.ino, set ```ssid``` and ```password``` to network SSID and password
2) Flash ino to esp32
3) Connect esp32 to two servos with data pins on 18 and 19
  + Ensure that pin 18 is the servo that handles horizontal movement
  + Ensure that pin 19 handles vertical movement
4) Attach some rod or "arm" implement to the vertical rotation servo
5) Attach webcam to end of vertical rotation servo
6) Connect webcam via USB into computer
7) On Computer, create ```.env``` file
8) In ```.env``` file, create environment variable titled as follows: ESP32_IP
  + Give it the value of the ip address printed by the esp32 in the serial monitor
9) Run main.py
10) :)
    
