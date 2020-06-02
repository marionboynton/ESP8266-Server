# README #
folder structure: 

ESP8266Server folder: 
1. ESP8266Server.ino 
2. data folder: (script.js; style.css; Chart.js; chartjs-plugin-datalabels.js)

1. compile ESP8266Server.ino 
2. upload ESP8266Server.ino to ESP8266 with serial port open 
3. close serial port and upload sketch data folder 
4. once spiffs files have completed upload open serial port and upload ESP8266Server.ino 
5. open wifi and connect to ESP8266 
6. open web browser and go to http://192.168.4.1