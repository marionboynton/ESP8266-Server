# README #

WiFi connectivity system for IoT batteries. 
Enables wifi connection between an ESP8266 micro-controller and a device to enable a device to connect through wifi network if a wifi router is available or directly to the battery (hotspot) if one is not. Includes simple web page and mobile app that displays real time information from the ESP8266, and ultimately the battery, when the device is connected. Includes a web page and mobile app that displays real time information from the ESP8266 (retrieved from the battery), when the device is connected. 


folder structure: 


ESP8266Server folder: 

1.	ESP8266Server.ino 

2.	data folder: (script.js; style.css; Chart.js; chartjs-plugin-datalabels.js)


Steps to run:

1.	compile ESP8266Server.ino 

2.	upload ESP8266Server.ino to ESP8266 with serial port open 

3.	close serial port and upload sketch data folder 

4.	once spiffs files have completed upload open serial port and upload ESP8266Server.ino 

5.	open wifi and connect to ESP8266 

6.	open web browser and go to http://192.168.4.1