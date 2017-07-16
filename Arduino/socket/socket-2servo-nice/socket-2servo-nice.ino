#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <ESP8266mDNS.h>
#include <Hash.h>
#include <Servo.h>

//#define DEBUG

//adjust this <<<<<<<<<<--------------
#define SERVO1   4
#define SERVO2   16
const bool invert_servo1 = true;
const bool invert_servo2 = false;

const char *ssid = "TromontanaS";
const char *password = "zotkazotka";

uint32_t failsaveS=5;
const uint8_t failSaveServo1 = 40;
const uint8_t failSaveServo2 = 80;
//end of adjusting, more is at the botom



#ifdef DEBUG
#define USE_SERIAL Serial
#endif
const long interval = 1000;
int same=0;


//range styling: http://twiggle-web-design.com/tutorials/Custom-Vertical-Input-Range-CSS3.html

const char skripta[] PROGMEM = "<html><head><title>Miharix WiFi RC</title>"
"<script>var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);connection.onopen = function () {  connection.send('Connect ' + new Date()); }; connection.onerror = function (error) {    console.log('WebSocket Error ', error);};connection.onmessage = function (e) {  console.log('Server: ', e.data);};function sendServo() {  var g = parseInt(document.getElementById('g').value).toString(16);  var s = parseInt(document.getElementById('s').value).toString(16);  if(g.length < 2) { g = '0' + g; }   if(s.length < 2) { s = '0' + s; }    var rgb = '#'+s+g;    console.log('RGB: ' + rgb); connection.send(rgb); }</script>"
"<style>"
"html { background-color: Black; color: #555; height: 100%; padding: 0px; margin: 0px; overflow:hidden; text-align:center;}"
"body { font-family: Arial, Calibri; width: 100%; margin: 0px; background-color: Black; padding: 0px; height: 100%;} "
"input[type='range'].range {position: fixed; cursor: grabbing; width: 50% !important; -webkit-appearance: none; width:50%; border: 0px; background-color: #e6e6e6; background-image: -webkit-gradient(linear, 0% 0%, 0% 100%, from(#e6e6e6), to(#d2d2d2)); background-image: -webkit-linear-gradient(right, #222222, #AAAAAA,#222222); background-image: -moz-linear-gradient(right, #222222, #AAAAAA,#222222); background-image: -ms-linear-gradient(right, #222222, #AAAAAA,#222222); background-image: -o-linear-gradient(right, #222222, #AAAAAA,#222222); }"
"input[type='range'].range:focus{border: 0 !imporant; outline: none !important;}"
"input[type='range'].range::-webkit-slider-thumb{-webkit-appearance: none;width: 100px;height: 100px; background-color: #555;background-image: -webkit-gradient(linear, 0% 0%, 0% 100%, from(#4DFFFF), to(#00CCFF));background-image: -webkit-linear-gradient(right,#000000, #00cc00,#00cc00,#000000);background-image: -moz-linear-gradient(right,#000000, #00cc00,#00cc00,#000000);background-image: -ms-linear-gradient(right,#000000, #00cc00,#00cc00,#000000);background-image: -o-linear-gradient(right,#000000, #00cc00,#00cc00,#000000);}"
"input[type='range'].round {-webkit-border-radius: 20px;-moz-border-radius: 20px;border-radius: 20px;}"
"input[type='range'].round::-webkit-slider-thumb{-webkit-border-radius: 20px;  -moz-border-radius: 20px; -o-border-radius: 20px;}"
".horizontal-lowest-first{-webkit-transform:rotate(0deg);-moz-transform:rotate(0deg);-o-transform:rotate(0deg);-ms-transform:rotate(0deg);transform:rotate(0deg);}"
".vertical-heighest-first{-webkit-transform:rotate(270deg);-moz-transform:rotate(270deg);-o-transform:rotate(270deg);-ms-transform:rotate(270deg);transform:rotate(270deg);}"
".servo1{top: 40%; left: 2%;}"
".servo2{top: 40%; left: 55%;}"
"</style>"
"<meta name='viewport' content='width=device-width, initial-scale=1, maximum-scale=1, user-scalable=0'/>"
"</head><body>"
"Miharix WiFi RC"

//set servo range and init value !!!! <<<<<<<<<<<<<<<<---------------<<<<<<<<<<<<<<<<---------------
"<input id='s' class='range horizontal-lowest-first round servo1' type='range' oninput='sendServo();' min='10' max='170' step='1' value='90'>"
"<input id='g' class='range vertical-heighest-first round servo2' type='range' oninput='sendServo();' min='10' max='170' step='1' value='90'>"
"</body></html>";



Servo servo1;
Servo servo2;
unsigned long previousMillis = 0;
uint32_t rgb2 = 0;
uint32_t rgb = 1;
ESP8266WiFiMulti WiFiMulti;

ESP8266WebServer server = ESP8266WebServer(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

    switch(type) {
        case WStype_DISCONNECTED:

            #ifdef DEBUG
            USE_SERIAL.printf("[%u] Disconnected!\n", num);
            #endif

            digitalWrite(2, LOW);
            
            break;
        case WStype_CONNECTED: {
            IPAddress ip = webSocket.remoteIP(num);

            #ifdef DEBUG
            USE_SERIAL.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
            #endif
            
            // send message to client
            webSocket.sendTXT(num, "Connected");
        }
            break;
        case WStype_TEXT:

            #ifdef DEBUG
            USE_SERIAL.printf("[%u] get Text: %s\n", num, payload);
            #endif
            
            if(payload[0] == '#') {
                // we get RGB data

                // decode rgb data
                
                 rgb = (uint32_t) strtol((const char *) &payload[1], NULL, 16);
                 #ifdef DEBUG
                  USE_SERIAL.printf("rgb=%u",rgb);
                  USE_SERIAL.printf("rgb2=%u",rgb2);
                 #endif
              //  analogWrite(LED_RED,    ((rgb >> 16) & 0xFF));
               // analogWrite(LED_GREEN,  ((rgb >> 8) & 0xFF));
               // analogWrite(LED_BLUE,   ((rgb >> 0) & 0xFF));

               
               
               if(invert_servo1){
                servo1.write(180-(rgb >> 8) & 0xFF);
               }else{
                 servo1.write((rgb >> 8) & 0xFF);
               }

               
               if(invert_servo2){
                  servo2.write(180-(rgb >> 0) & 0xFF);
               }else{
                 servo2.write((rgb >> 0) & 0xFF);
               }

               
            }

            break;
    }

}

void setup() {
    
    #ifdef DEBUG
    USE_SERIAL.begin(115200);

    //USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }
    #endif



    WiFi.softAP(ssid, password);

    webSocket.begin();
    webSocket.onEvent(webSocketEvent);

    if(MDNS.begin("esp8266")) {
        #ifdef DEBUG
        USE_SERIAL.println("MDNS responder started");
        #endif
    }
    

    // handle index
    server.on("/", []() {
    server.send(200, "text/html", skripta);
    digitalWrite(2, LOW);
    delay(50); 
    digitalWrite(2, HIGH);  
    });

    server.begin();

    // Add service to MDNS
    MDNS.addService("http", "tcp", 80);
    MDNS.addService("ws", "tcp", 81);

//set servo init value <<<<<<<<<<<<<<<<---------------
  servo1.attach(SERVO1);
  servo1.write(90);
  servo2.attach(SERVO2);
  servo2.write(90);
//
  pinMode(2, OUTPUT);

  for(uint8_t t = 4; t > 0; t--) {
    digitalWrite(2, HIGH);
    delay(50); 
    digitalWrite(2, LOW);
    delay(50); 
  }

}

void loop() {
    webSocket.loop();
    
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
       #ifdef DEBUG
      USE_SERIAL.printf("rgb2=%u\n",rgb2);
      USE_SERIAL.printf("rgb=%u\n",rgb);
      #endif
      previousMillis = currentMillis;
      if (rgb == rgb2) {
        same++;
       if(same>=failsaveS){
          servo1.write(failSaveServo1);
          servo2.write(failSaveServo2);
          digitalWrite(2, LOW);
       }
      }else{
       digitalWrite(2, HIGH);
       same=0;
      }
       rgb2=rgb;
    }

    
    server.handleClient();
}

