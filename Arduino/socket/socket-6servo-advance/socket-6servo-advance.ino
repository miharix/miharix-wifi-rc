

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
//#include <ESP8266WiFiMulti.h> // za več wifijev
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h> //for parsing/saving json file
#include <FS.h>   // Include the SPIFFS library
#include <Servo.h>


//#define DEBUG
//#define DEBUG1

#define PRINTLN(var) Serial.print(#var ": "); Serial.println(var)

//ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80
WebSocketsServer webSocket = WebSocketsServer(81);    // create a websocket server on port 81

#define SERVO1   16
#define SERVO2   5
#define SERVO3   14
#define SERVO4   4
#define SERVO5   12
#define SERVO6   13


#define HARD_RESET_PIN 0

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;

unsigned long previousMillis = 0;
unsigned long previousMillisR1 = 0;
unsigned long previousMillisR2 = 0;
unsigned long previousMillisR3 = 0;
unsigned long previousMillisR4 = 0;
unsigned long previousMillisR5 = 0;
unsigned long previousMillisR6 = 0;

const long interval = 1000; //interval for checking failsave state
bool S1_6_flag = false;
word SameStateCount = 0; //counter of non changed states - failsave
char S1_buff[3];//servo socket buffer
char S2_buff[3];
char S3_buff[3];
char S4_buff[3];
char S5_buff[3];
char S6_buff[3];

//int rate_interval = 0;

byte Servo1_target = 90;
byte Servo2_target = 90;
byte Servo3_target = 90;
byte Servo4_target = 90;
byte Servo5_target = 90;
byte Servo6_target = 90;
unsigned long currentMillis = millis();

bool invert_servo1 = false;
bool invert_servo2 = false;
bool invert_servo3 = false;
bool invert_servo4 = false;
bool invert_servo5 = false;
bool invert_servo6 = false;

word failSaveServo1 = 90; //failsave position 0-180
word failSaveServo2 = 90;
word failSaveServo3 = 90;
word failSaveServo4 = 90;
word failSaveServo5 = 90;
word failSaveServo6 = 90;

word maxRangeServo1 = 0;
word maxRangeServo2 = 0;
word maxRangeServo3 = 0;
word maxRangeServo4 = 0;
word maxRangeServo5 = 0;
word maxRangeServo6 = 0;

word minRangeServo1 = 0;
word minRangeServo2 = 0;
word minRangeServo3 = 0;
word minRangeServo4 = 0;
word minRangeServo5 = 0;
word minRangeServo6 = 0;

short TrimServo1 = 0;
short TrimServo2 = 0;
short TrimServo3 = 0;
short TrimServo4 = 0;
short TrimServo5 = 0;
short TrimServo6 = 0;

byte ExpoServo1 = 0;
byte ExpoServo2 = 0;
byte ExpoServo3 = 0;
byte ExpoServo4 = 0;
byte ExpoServo5 = 0;
byte ExpoServo6 = 0;


byte RateServo1 = 0;
byte RateServo2 = 0;
byte RateServo3 = 0;
byte RateServo4 = 0;
byte RateServo5 = 0;
byte RateServo6 = 0;


//float expor=0.20;
double CalcExpoServo1 = 0; //=pow(4,expor);
double CalcExpoServo2 = 0;
double CalcExpoServo3 = 0;
double CalcExpoServo4 = 0;
double CalcExpoServo5 = 0;
double CalcExpoServo6 = 0;


byte RCmode = 0;
byte FailSaveTime = 5;

char  RCname[64] = "";
char  RCSSID[64] = "SSID";//WiFi.macAddress().ToString()+" (miharix.eu)";
char  RCPASS[64] = "miharix.eu";
//char  RCSSID[64] = "S55MX 2,4Ghz"; //WiFi.macAddress()+" (miharix.eu)";
//char  RCPASS[64] = "mihamiha";
bool WiFiAP = true; //make WiFi AP 1 or connect to wifi 0
bool factory_boot = false;

bool ResetToFactoryFlag = false;

String getContentType(String filename); // convert the file extension to the MIME type
bool handleFileRead(String path);       // send the right file to the client (if it exists)

void setup() {
  pinMode(HARD_RESET_PIN, INPUT);
#ifdef DEBUG1
  Serial.begin(9600);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');
#endif
  SPIFFS.begin();                           // Start the SPI Flash Files System
  loadConfig();
  delay(10);
  // WiFi.disconnect();  //erese old ssid password

  if (!WiFiAP) { //Connect to wifi
    WiFi.begin(RCSSID, RCPASS);
#ifdef DEBUG1
    Serial.print("Connecting to ");
    Serial.print(RCSSID); Serial.println(" ...");
#endif
    int NC_count = 0;
    while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
      delay(1000);
#ifdef DEBUG1
      Serial.print('.');
#endif
      if (NC_count > 300) {
        ResetToFactoryFlag = true;
        ResetToFactory();
      }
      NC_count++;

    }
#ifdef DEBUG1
    Serial.println('\n');
    Serial.print("Connected to ");
    Serial.println(WiFi.SSID());              // Tell us what network we're connected to
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer
#endif
  } else { //go AP wifi mode
    WiFi.softAP(RCSSID, RCPASS);             // Start the access point
#ifdef DEBUG1
    Serial.print(RCSSID);
    Serial.print("Access Point \"");
    Serial.println("\" started");
    Serial.print("IP address:\t");
    Serial.println(WiFi.softAPIP());         // Send the IP address of the ESP8266 to the computer
#endif

  }

  /* wifiMulti.addAP("S55MX 2,4Ghz", "mihamiha");   // add Wi-Fi networks you want to connect to
    wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2");
    wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3");*/

  // Serial.println("Connecting ...");
  // int i = 0;
  /* while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
     delay(250);
     Serial.print('.');
    }*/


  if (MDNS.begin("rc")) {              // Start the mDNS responder for rc.local
#ifdef DEBUG1
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
#endif
  }


  // server.on("/setup.html", HTTP_GET, get_response);


  server.onNotFound([]() {                              // If the client requests any URI
    if (!handleFileRead(server.uri()))                  // send it if it exists
      server.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
  });

  server.begin();                           // Actually start the server
#ifdef DEBUG
  Serial.println("HTTP server started");
#endif
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
#ifdef DEBUG
  Serial.println("websocket server started");
#endif
  servos_attach();
  CalcExpoRates();

  pinMode(2, OUTPUT);
  for (uint8_t t = 4; t > 0; t--) { //blink on boot
    digitalWrite(2, HIGH);
    delay(50);
    digitalWrite(2, LOW);
    delay(50);
  }

  // Add service to MDNS
  MDNS.addService("http", "tcp", 80);
  MDNS.addService("ws", "tcp", 81);
}

void servos_attach() {
  //set servo init value <<<<<<<<<<<<<<<<--------------------------------------
  servo1.attach(SERVO1, minRangeServo1, maxRangeServo1);
  servo1.writeMicroseconds(failSaveServo1);
  servo2.attach(SERVO2, minRangeServo2, maxRangeServo2);
  servo2.writeMicroseconds(failSaveServo2);
  servo3.attach(SERVO3, minRangeServo3, maxRangeServo3);
  servo3.writeMicroseconds(failSaveServo3);
  servo4.attach(SERVO4, minRangeServo4, maxRangeServo4);
  servo4.writeMicroseconds(failSaveServo4);
  servo5.attach(SERVO5, minRangeServo5, maxRangeServo5);
  servo5.writeMicroseconds(failSaveServo5);
  servo6.attach(SERVO6, minRangeServo6, maxRangeServo6);
  servo6.writeMicroseconds(failSaveServo6);
}
void servos_dettach() {
  servo1.detach();
  servo2.detach();
  servo3.detach();
  servo4.detach();
  servo5.detach();
  servo6.detach();
}

void CalcExpoRates() {
#ifdef DEBUG1
  Serial.println("ExpoServo1");
  Serial.println(ExpoServo1);
#endif
  float tempR = 2;
  tempR = ExpoServo1 / 100.0;
#ifdef DEBUG1
  Serial.println("tempR");
  Serial.println(tempR);
#endif
  CalcExpoServo1 = pow(4, tempR);
#ifdef DEBUG1
  Serial.println("CalcExpoServo1");
  Serial.println(CalcExpoServo1);

  Serial.println("ExpoServo2");
  Serial.println(ExpoServo2);
#endif
  tempR = ExpoServo2 / 100.0;
#ifdef DEBUG1
  Serial.println("tempR2");
  Serial.println(tempR);
#endif
  CalcExpoServo2 = pow(4, tempR);
#ifdef DEBUG1
  Serial.println("CalcExpoServo2");
  Serial.println(CalcExpoServo2);
#endif
  tempR = ExpoServo3 / 100.0;
  CalcExpoServo3 = pow(4, tempR);

  tempR = ExpoServo4 / 100.0;
  CalcExpoServo4 = pow(4, tempR);

  tempR = ExpoServo5 / 100.0;
  CalcExpoServo5 = pow(4, tempR);

    tempR = ExpoServo6 / 100.0;
  CalcExpoServo6 = pow(4, tempR);

}


int exponentRC(float input, double calcrate, int servotrim) {
  if (input > 180) {
    input = 180;
  }
  if (input < 0) {
    input = 0;
  }
  input = input - 90;
  input = input / 100;

  if (input > 0) {
    input = (100 * pow(input, calcrate) + 90) + servotrim;
    if (input > 180) {
      return 180;
    }
    return input;
  }

  if (input < 0) {
    input = (100 * - pow(-input, calcrate) + 90) + servotrim;
    if (input < 0) {
      return 0;
    }
    return input;
  }

  return 90 + servotrim;
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:

#ifdef DEBUG
      Serial.printf("[%u] Disconnected!\n", num);
#endif

      digitalWrite(2, LOW);

      break;
    case WStype_CONNECTED: {
        IPAddress ip = webSocket.remoteIP(num);

#ifdef DEBUG
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
#endif

        // send message to client
        webSocket.sendTXT(num, "Connected");
      }
      break;
    case WStype_TEXT:

#ifdef DEBUG
      Serial.printf("[%u] get Text: %s\n", num, payload);
#endif

      if ((payload[0] == 'S') && (payload[13] == '\n')) {

        memcpy(S1_buff, &payload[1], 2 );
        // S1_buff[3] = '\0';


        memcpy(S2_buff, &payload[3], 2 );
        //     S2_buff[3] = '\0';

        memcpy(S3_buff, &payload[5], 2 );
        //   S3_buff[3] = '\0';

        memcpy(S4_buff, &payload[7], 2 );
        //  S4_buff[3] = '\0';

        memcpy(S5_buff, &payload[9], 2 );
        //S5_buff[3] = '\0';

        memcpy(S6_buff, &payload[11], 2 );
        //S6_buff[3] = '\0';

        S1_6_flag = true;

        if (S1_buff[0] == '-') { //prevent buffer owerflow
          S1_buff[0] = '0';
          S1_buff[1] = '0';
        }
        if (S2_buff[0] == '-') {
          S2_buff[0] = '0';
          S2_buff[1] = '0';
        }
        if (S3_buff[0] == '-') {
          S3_buff[0] = '0';
          S3_buff[1] = '0';
        }
        if (S4_buff[0] == '-') {
          S4_buff[0] = '0';
          S4_buff[1] = '0';
        }
        if (S5_buff[0] == '-') {
          S5_buff[0] = '0';
          S5_buff[1] = '0';
        }
        if (S6_buff[0] == '-') {
          S6_buff[0] = '0';
          S6_buff[1] = '0';
        }

#ifdef DEBUG
        Serial.printf(" S1_6_flag=%d", S1_6_flag);
        Serial.printf("\nS1_buff=%u", strtol(S1_buff, NULL, 16));
        Serial.printf("\nS1_buff+trim1=%d", TrimServo1);
        Serial.printf("\nS1_buff+trim1=%u", strtol(S1_buff, NULL, 16) + TrimServo1);

        Serial.printf("\nS2_buff=%u", strtol(S2_buff, NULL, 16));
        Serial.printf("\nS3_buff=%u", strtol(S3_buff, NULL, 16));
        Serial.printf("\nS4_buff=%u", strtol(S4_buff, NULL, 16));
        Serial.printf("\nS5_buff=%u", strtol(S5_buff, NULL, 16));
        Serial.printf("\nS6_buff=%u", strtol(S6_buff, NULL, 16));
#endif


        if (invert_servo1) {
          //servo1.write(180 - (strtol(S1_buff, NULL, 16)) + TrimServo1);
          Servo1_target = 180 - strtol(S1_buff, NULL, 16);
        } else {
          //servo1.write(strtol(S1_buff, NULL, 16) + TrimServo1);
          Servo1_target = strtol(S1_buff, NULL, 16);
        }
        Servo1_target = exponentRC(Servo1_target, CalcExpoServo1, TrimServo1);

        if (invert_servo2) {
          Servo2_target = 180 - strtol(S2_buff, NULL, 16);
          //servo2.write(180 - (strtol(S2_buff, NULL, 16)) + TrimServo2);
        } else {
          Servo2_target = strtol(S2_buff, NULL, 16);
          //servo2.write(strtol(S2_buff, NULL, 16) + TrimServo2);
        }
#ifdef DEBUG1
        Serial.printf("\nS2_target1=%u", Servo2_target);
        Serial.println();
        Serial.println(CalcExpoServo2);
#endif
        Servo2_target = exponentRC(Servo2_target, CalcExpoServo2, TrimServo2);

        //pow(Servo2_target/10,pow(4,-50/100))*10;//expo
        // Serial.printf("S2_target2=%u", Servo2_target);
        //(((1*2)-1)*(abs(x)^(4^(80/100))))
        //((((abs(x)==x)*2)-1)*(abs(x)^(4^(80/100))))
#ifdef DEBUG1
        Serial.printf("\nS2_target=%u", Servo2_target);
#endif
        if (invert_servo3) {
          Servo3_target = 180 - strtol(S3_buff, NULL, 16);
          //servo3.write(180 - (strtol(S3_buff, NULL, 16)) + TrimServo3);
        } else {
          //servo3.write(strtol(S3_buff, NULL, 16) + TrimServo3);
          Servo3_target = strtol(S3_buff, NULL, 16);
        }

        Servo3_target = exponentRC(Servo3_target, CalcExpoServo3, TrimServo3);

        if (invert_servo4) {
          Servo4_target = 180 - strtol(S4_buff, NULL, 16);
          //servo4.write(180 - (strtol(S4_buff, NULL, 16)) + TrimServo4);
        } else {
          Servo4_target = strtol(S4_buff, NULL, 16);
          //servo4.write(strtol(S4_buff, NULL, 16) + TrimServo4);
        }
        Servo4_target = exponentRC(Servo4_target, CalcExpoServo4, TrimServo4);

        //Serial.printf("\nS4_target=%u", Servo4_target);
        if (invert_servo5) {
          Servo5_target = 180 - strtol(S5_buff, NULL, 16);
          //servo5.write(180 - (strtol(S5_buff, NULL, 16)));
        } else {
          Servo5_target = strtol(S5_buff, NULL, 16);
          //servo5.write(strtol(S5_buff, NULL, 16));
        }
        Servo5_target = exponentRC(Servo5_target, CalcExpoServo5, TrimServo5);


        if (invert_servo6) {
          //   servo6.write(180 - (strtol(S6_buff, NULL, 16)));
          Servo6_target = 180 - strtol(S6_buff, NULL, 16);
        } else {
          Servo6_target = strtol(S6_buff, NULL, 16);
          //servo6.write(strtol(S6_buff, NULL, 16));
        }
#ifdef DEBUG1
        Serial.printf("\nS6_target=%u", Servo6_target);
#endif
        Servo6_target = exponentRC(Servo6_target, CalcExpoServo6, TrimServo6);
#ifdef DEBUG1
        Serial.printf("\nS6_target2=%u", Servo6_target);
#endif
      }

      break;
  }

}


void servo_go() {

  if (currentMillis - previousMillisR1 >= RateServo1) {
    previousMillisR1 = currentMillis;
    if (servo1.read() != Servo1_target) {
      if (servo1.read() < Servo1_target) {
        servo1.write(servo1.read() + 1);
      } else {
        servo1.write(servo1.read() - 1);
      }
    }
  }

  if (currentMillis - previousMillisR2 >= RateServo2) {
    previousMillisR2 = currentMillis;
    if (servo2.read() != Servo2_target) {
      if (servo2.read() < Servo2_target) {
        servo2.write(servo2.read() + 1);
      } else {
        servo2.write(servo2.read() - 1);
      }
    }
  }

  if (currentMillis - previousMillisR3 >= RateServo3) {
    previousMillisR3 = currentMillis;
    if (servo3.read() != Servo3_target) {
      if (servo3.read() < Servo3_target) {
        servo3.write(servo3.read() + 1);
      } else {
        servo3.write(servo3.read() - 1);
      }
    }
  }

  if (currentMillis - previousMillisR4 >= RateServo4) {
    previousMillisR4 = currentMillis;
    if (servo4.read() != Servo4_target) {
      if (servo4.read() < Servo4_target) {
        servo4.write(servo4.read() + 1);
      } else {
        servo4.write(servo4.read() - 1);
      }
    }
  }

  if (currentMillis - previousMillisR5 >= RateServo5) {
    previousMillisR5 = currentMillis;
    if (servo5.read() != Servo5_target) {
      if (servo5.read() < Servo5_target) {
        servo5.write(servo5.read() + 1);
      } else {
        servo5.write(servo5.read() - 1);
      }
    }
  }

  if (currentMillis - previousMillisR6 >= RateServo6) {
    previousMillisR6 = currentMillis;
    if (servo6.read() != Servo6_target) {
      if (servo6.read() < Servo6_target) {
        servo6.write(servo6.read() + 1);
      } else {
        servo6.write(servo6.read() - 1);
      }
    }
  }
}
//ResetToFactoryTime
//ResetToFactoryCounter

void ResetToFactory() {
  //Serial.println("HARD_RESET_PIN: "+String(digitalRead(HARD_RESET_PIN)));
  if (!digitalRead(HARD_RESET_PIN) || ResetToFactoryFlag) {
    digitalWrite(2, LOW);
    delay(50);
    for (uint8_t t = 50; t > 0; t--) { //blink before hard reset
      digitalWrite(2, HIGH);
      delay(t * 10);
      digitalWrite(2, LOW);
      delay(t * 10 / 2);
    }
    if (!digitalRead(HARD_RESET_PIN) || ResetToFactoryFlag) {
#ifdef DEBUG1
      Serial.println("GO HARD RESET");
#endif
      for (uint8_t t = 5; t > 0; t--) { //blink before hard reset
        digitalWrite(2, HIGH);
        delay(100);
        digitalWrite(2, LOW);
        delay(100);
      }

      //----------
      File RestoreFile = SPIFFS.open("/config_restore.json", "r");
      File RewriteFile = SPIFFS.open("/config.json", "w");
      while (RestoreFile.available()) {
        digitalWrite(2, HIGH);
        RewriteFile.write(RestoreFile.read());
        digitalWrite(2, LOW);
      }
      RestoreFile.close();
      RewriteFile.close();
#ifdef DEBUG1
      Serial.println("GO HARD RESET done");
#endif
      for (uint8_t t = 20; t > 0; t--) { //blink before hard reset
        digitalWrite(2, HIGH);
        delay(50);
        digitalWrite(2, LOW);
        delay(50);
      }
      delay(2000);
      ESP.restart();

    }
  }

}

void loop(void) {

  ResetToFactory();
  webSocket.loop();

  currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
#ifdef DEBUG
    Serial.printf("S1_6_flag=%d\n", S1_6_flag);
#endif
    previousMillis = currentMillis;
    if (S1_6_flag == false) {
      SameStateCount++;
      if (SameStateCount >= FailSaveTime) {
        servo1.write(failSaveServo1);
        servo2.write(failSaveServo2);
        servo3.write(failSaveServo3);
        servo4.write(failSaveServo4);
        servo5.write(failSaveServo5);
        servo6.write(failSaveServo6);
        digitalWrite(2, LOW);
      }
    } else {
      digitalWrite(2, HIGH);
      SameStateCount = 0;
      S1_6_flag = false;
    }
  }

  if (SameStateCount < FailSaveTime) {
    servo_go();
  }


  server.handleClient();
}

void print_variables() {
  PRINTLN(invert_servo1);
  PRINTLN(invert_servo2);
  PRINTLN(invert_servo3);
  PRINTLN(invert_servo4);
  PRINTLN(invert_servo5);
  PRINTLN(invert_servo6);

  PRINTLN(failSaveServo1);
  PRINTLN(failSaveServo2);
  PRINTLN(failSaveServo3);
  PRINTLN(failSaveServo4);
  PRINTLN(failSaveServo5);
  PRINTLN(failSaveServo6);

  PRINTLN(maxRangeServo1);
  PRINTLN(maxRangeServo2);
  PRINTLN(maxRangeServo3);
  PRINTLN(maxRangeServo4);
  PRINTLN(maxRangeServo5);
  PRINTLN(maxRangeServo6);

  PRINTLN(minRangeServo1);
  PRINTLN(minRangeServo2);
  PRINTLN(minRangeServo3);
  PRINTLN(minRangeServo4);
  PRINTLN(minRangeServo5);
  PRINTLN(minRangeServo6);

  PRINTLN(ExpoServo1);
  PRINTLN(ExpoServo2);
  PRINTLN(ExpoServo3);
  PRINTLN(ExpoServo4);
  PRINTLN(ExpoServo5);
  PRINTLN(ExpoServo6);

  PRINTLN(TrimServo1);
  PRINTLN(TrimServo2);
  PRINTLN(TrimServo3);
  PRINTLN(TrimServo4);
  PRINTLN(TrimServo5);
  PRINTLN(TrimServo6);

  PRINTLN(RateServo1);
  PRINTLN(RateServo2);
  PRINTLN(RateServo3);
  PRINTLN(RateServo4);
  PRINTLN(RateServo5);
  PRINTLN(RateServo6);

  PRINTLN(RCmode);
  PRINTLN(FailSaveTime);
  PRINTLN(RCname);
  PRINTLN(RCSSID);
  PRINTLN(RCPASS);
  PRINTLN(WiFiAP);
}


String getContentType(String filename) { // convert the file extension to the MIME type

  if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".json")) return "application/json";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".svg")) return "image/svg+xml";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/html";
}

bool saveConfig() {
  print_variables();
  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    return false;
  }
  StaticJsonBuffer<1124> jsonBuffer;
  JsonObject &json = jsonBuffer.createObject();

  if (!factory_boot) {
    json["SSID"] = RCSSID;
  } else {
    json["SSID"] = "SSID";
  }
  json["Pass"] = RCPASS;
  json["WiFiAP"] = WiFiAP;
  json["RCname"] = RCname;
  json["Mode"] = RCmode;
  json["Ftime"] = FailSaveTime;

  JsonObject& Servo1 = json.createNestedObject("Servo1");
  JsonObject& Servo2 = json.createNestedObject("Servo2");
  JsonObject& Servo3 = json.createNestedObject("Servo3");
  JsonObject& Servo4 = json.createNestedObject("Servo4");
  JsonObject& Servo5 = json.createNestedObject("Servo5");
  JsonObject& Servo6 = json.createNestedObject("Servo6");

  Servo1["max"] = maxRangeServo1;
  Servo2["max"] = maxRangeServo2;
  Servo3["max"] = maxRangeServo3;
  Servo4["max"] = maxRangeServo4;
  Servo5["max"] = maxRangeServo5;
  Servo6["max"] = maxRangeServo6;

  Servo1["min"] = minRangeServo1;
  Servo2["min"] = minRangeServo2;
  Servo3["min"] = minRangeServo3;
  Servo4["min"] = minRangeServo4;
  Servo5["min"] = minRangeServo5;
  Servo6["min"] = minRangeServo6;

  Servo1["trim"] = TrimServo1;
  Servo2["trim"] = TrimServo2;
  Servo3["trim"] = TrimServo3;
  Servo4["trim"] = TrimServo4;
  Servo5["trim"] = TrimServo5;
  Servo6["trim"] = TrimServo6;

  Servo1["failsave"] = failSaveServo1;
  Servo2["failsave"] = failSaveServo2;
  Servo3["failsave"] = failSaveServo3;
  Servo4["failsave"] = failSaveServo4;
  Servo5["failsave"] = failSaveServo5;
  Servo6["failsave"] = failSaveServo6;

  Servo1["expo"] = ExpoServo1;
  Servo2["expo"] = ExpoServo2;
  Servo3["expo"] = ExpoServo3;
  Servo4["expo"] = ExpoServo4;
  Servo5["expo"] = ExpoServo5;
  Servo6["expo"] = ExpoServo6;

  Servo1["rate"] = RateServo1;
  Servo2["rate"] = RateServo2;
  Servo3["rate"] = RateServo3;
  Servo4["rate"] = RateServo4;
  Servo5["rate"] = RateServo5;
  Servo6["rate"] = RateServo6;

  Servo1["reverse"] = invert_servo1;
  Servo2["reverse"] = invert_servo2;
  Servo3["reverse"] = invert_servo3;
  Servo4["reverse"] = invert_servo4;
  Servo5["reverse"] = invert_servo5;
  Servo6["reverse"] = invert_servo6;

  if (json.prettyPrintTo(configFile) == 0) {
#ifdef DEBUG1
    Serial.println(F("Failed to write to file"));
#endif
  }

  // Close the file (File's destructor doesn't close the file)
  configFile.close();
  return true;
}

bool loadConfig() {
    
  File configFile = SPIFFS.open("/config.json", "r"); 
  if (!configFile) {
    return false;
  }
   
  size_t size = configFile.size();

#ifdef DEBUG1
  Serial.println(size);
#endif
  if (size > 1124) {
#ifdef DEBUG1
    Serial.println("Config to big");
#endif


    return false;
  }
        
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
#ifdef DEBUG
  Serial.println(buf.get());
#endif
  StaticJsonBuffer<1124> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());
  if (!json.success()) {
#ifdef DEBUG1
    Serial.println("parser error");
#endif
   return false;
  }
  configFile.close();//close the file

  invert_servo1 = json["Servo1"]["reverse"];
  invert_servo2 = json["Servo2"]["reverse"];
  invert_servo3 = json["Servo3"]["reverse"];
  invert_servo4 = json["Servo4"]["reverse"];
  invert_servo5 = json["Servo5"]["reverse"];
  invert_servo6 = json["Servo6"]["reverse"];

  failSaveServo1 = json["Servo1"]["failsave"];
  failSaveServo2 = json["Servo2"]["failsave"];
  failSaveServo3 = json["Servo3"]["failsave"];
  failSaveServo4 = json["Servo4"]["failsave"];
  failSaveServo5 = json["Servo5"]["failsave"];
  failSaveServo6 = json["Servo6"]["failsave"];

  maxRangeServo1 = json["Servo1"]["max"];
  maxRangeServo2 = json["Servo2"]["max"];
  maxRangeServo3 = json["Servo3"]["max"];
  maxRangeServo4 = json["Servo4"]["max"];
  maxRangeServo5 = json["Servo5"]["max"];
  maxRangeServo6 = json["Servo6"]["max"];

  minRangeServo1 = json["Servo1"]["min"];
  minRangeServo2 = json["Servo2"]["min"];
  minRangeServo3 = json["Servo3"]["min"];
  minRangeServo4 = json["Servo4"]["min"];
  minRangeServo5 = json["Servo5"]["min"];
  minRangeServo6 = json["Servo6"]["min"];

  TrimServo1 = json["Servo1"]["trim"];
  TrimServo2 = json["Servo2"]["trim"];
  TrimServo3 = json["Servo3"]["trim"];
  TrimServo4 = json["Servo4"]["trim"];
  TrimServo5 = json["Servo5"]["trim"];
  TrimServo6 = json["Servo6"]["trim"];

  ExpoServo1 = json["Servo1"]["expo"];
  ExpoServo2 = json["Servo2"]["expo"];
  ExpoServo3 = json["Servo3"]["expo"];
  ExpoServo4 = json["Servo4"]["expo"];
  ExpoServo5 = json["Servo5"]["expo"];
  ExpoServo6 = json["Servo6"]["expo"];

  RateServo1 = json["Servo1"]["rate"];
  RateServo2 = json["Servo2"]["rate"];
  RateServo3 = json["Servo3"]["rate"];
  RateServo4 = json["Servo4"]["rate"];
  RateServo5 = json["Servo5"]["rate"];
  RateServo6 = json["Servo6"]["rate"];

  RCmode = json["Mode"];
  FailSaveTime = json["Ftime"];

  strlcpy(RCname, json["RCname"], sizeof(RCname));
#ifdef DEBUG1
  Serial.println(RCSSID);
#endif
  if (json["SSID"] != "SSID") {
    strlcpy(RCSSID, json["SSID"], sizeof(RCSSID));
    strlcpy(RCPASS, json["Pass"], sizeof(RCPASS));
    WiFiAP = json["WiFiAP"];
  } else {//ssid not set go AP use mac and display pass
    unsigned char masrc[6];
    WiFi.macAddress(masrc);
    String macRC = String(masrc[0], 16) + ":" + String(masrc[1], 16) + ":" + String(masrc[2], 16) + ":" + String(masrc[3], 16) + ":" + String(masrc[4], 16) + ":" + String(masrc[5], 16) + " (miharix.eu)";
#ifdef DEBUG1
    Serial.println(macRC);
#endif
    macRC.toCharArray(RCSSID, sizeof(RCSSID));
    strlcpy(RCPASS, "miharix.eu", sizeof(RCPASS));
    WiFiAP = true;
    factory_boot = true; //first time boot -> on root go to FAQ.html
  }


  print_variables();

  return true;

}

void rebooESP(String filename) {
  //server.send(200, "text/html", "<html><head><meta http-equiv='refresh' content='60;url=/setup.html' /></head><body><h1>Auto <a href='/setup.html'>reload</a> in 60 seconds...</h1></body></html>");
  
  File file = SPIFFS.open(filename, "r");                    // Open the file
  size_t sent = server.streamFile(file, "text/html");    // Send it to the client
  file.close();                                          // Close the file again
  
  delay(2000);
  ESP.restart();
}

void handleargs() { //handle http_get arguments
  for (int i = 0; i < server.args(); i++) { //for debug print all arguments
#ifdef DEBUG1
    Serial.println("Arg " + server.argName(i) + "=" + String(server.arg(i)));
#endif
  }
  //update temp variables

  if (server.hasArg("s1r")) {
    invert_servo1 = server.arg("s1r");
  } else {
    invert_servo1 = false;
  }
  if (server.hasArg("s2r")) {
    invert_servo2 = server.arg("s2r");
  } else {
    invert_servo2 = false;
  }
  if (server.hasArg("s3r")) {
    invert_servo3 = server.arg("s3r");
  } else {
    invert_servo3 = false;
  }
  if (server.hasArg("s4r")) {
    invert_servo4 = server.arg("s4r");
  } else {
    invert_servo4 = false;
  }
  if (server.hasArg("s5r")) {
    invert_servo5 = server.arg("s5r");
  } else {
    invert_servo5 = false;
  }
  if (server.hasArg("s6r")) {
    invert_servo6 = server.arg("s6r");
  } else {
    invert_servo6 = false;
  }

  if (server.hasArg("s1f")) {
    failSaveServo1 = server.arg("s1f").toInt();
  }
  if (server.hasArg("s2f")) {
    failSaveServo2 = server.arg("s2f").toInt();
  }
  if (server.hasArg("s3f")) {
    failSaveServo3 = server.arg("s3f").toInt();
  }
  if (server.hasArg("s4f")) {
    failSaveServo4 = server.arg("s4f").toInt();
  }
  if (server.hasArg("s5f")) {
    failSaveServo5 = server.arg("s5f").toInt();
  }
  if (server.hasArg("s6f")) {
    failSaveServo6 = server.arg("s6f").toInt();
  }

  if (server.hasArg("s1x")) {
    maxRangeServo1 = server.arg("s1x").toInt();
  }
  if (server.hasArg("s2x")) {
    maxRangeServo2 = server.arg("s2x").toInt();
  }
  if (server.hasArg("s3x")) {
    maxRangeServo3 = server.arg("s3x").toInt();
  }
  if (server.hasArg("s4x")) {
    maxRangeServo4 = server.arg("s4x").toInt();
  }
  if (server.hasArg("s5x")) {
    maxRangeServo5 = server.arg("s5x").toInt();
  }
  if (server.hasArg("s6x")) {
    maxRangeServo6 = server.arg("s6x").toInt();
  }

  if (server.hasArg("s1n")) {
    minRangeServo1 = server.arg("s1n").toInt();
  }
  if (server.hasArg("s2n")) {
    minRangeServo2 = server.arg("s2n").toInt();
  }
  if (server.hasArg("s3n")) {
    minRangeServo3 = server.arg("s3n").toInt();
  }
  if (server.hasArg("s4n")) {
    minRangeServo4 = server.arg("s4n").toInt();
  }
  if (server.hasArg("s5n")) {
    minRangeServo5 = server.arg("s5n").toInt();
  }
  if (server.hasArg("s6n")) {
    minRangeServo6 = server.arg("s6n").toInt();
  }

  if (server.hasArg("s1t")) {
    TrimServo1 = server.arg("s1t").toInt();
  }
  if (server.hasArg("s2t")) {
    TrimServo2 = server.arg("s2t").toInt();
  }
  if (server.hasArg("s3t")) {
    TrimServo3 = server.arg("s3t").toInt();
  }
  if (server.hasArg("s4t")) {
    TrimServo4 = server.arg("s4t").toInt();
  }
  if (server.hasArg("s5t")) {
    TrimServo5 = server.arg("s5t").toInt();
  }
  if (server.hasArg("s6t")) {
    TrimServo6 = server.arg("s6t").toInt();
  }

  if (server.hasArg("s1e")) {
    ExpoServo1 = server.arg("s1e").toInt();
  }
  if (server.hasArg("s2e")) {
    ExpoServo2 = server.arg("s2e").toInt();
  }
  if (server.hasArg("s3e")) {
    ExpoServo3 = server.arg("s3e").toInt();
  }
  if (server.hasArg("s4e")) {
    ExpoServo4 = server.arg("s4e").toInt();
  }
  if (server.hasArg("s5e")) {
    ExpoServo5 = server.arg("s5e").toInt();
  }
  if (server.hasArg("s6e")) {
    ExpoServo6 = server.arg("s6e").toInt();
  }

  if (server.hasArg("s1s")) {
    RateServo1 = server.arg("s1s").toInt();
  }
  if (server.hasArg("s2s")) {
    RateServo2 = server.arg("s2s").toInt();
  }
  if (server.hasArg("s3s")) {
    RateServo3 = server.arg("s3s").toInt();
  }
  if (server.hasArg("s4s")) {
    RateServo4 = server.arg("s4s").toInt();
  }
  if (server.hasArg("s5s")) {
    RateServo5 = server.arg("s5s").toInt();
  }
  if (server.hasArg("s6s")) {
    RateServo6 = server.arg("s6s").toInt();
  }

  if (server.hasArg("Mode")) {
    RCmode = server.arg("Mode").toInt();
  }
  if (server.hasArg("Ftime")) {
    FailSaveTime = server.arg("Ftime").toInt();
  }
  if (server.hasArg("rcname")) {
    server.arg("rcname").toCharArray(RCname, sizeof(RCname));
    //RCname = server.arg("rcname").c_str();
  }
  if (server.hasArg("RCSSID")) {
    char  RCSSID_temp[64] = "SSID_temp";
    server.arg("RCSSID").toCharArray(RCSSID_temp, sizeof(RCSSID_temp));
    // RCSSID = server.arg("RCSSID").c_str();
    if (RCSSID_temp != "SSID") {
      factory_boot = false;
      server.arg("RCSSID").toCharArray(RCSSID, sizeof(RCSSID));

      if (server.hasArg("RCPASS")) {
        server.arg("RCPASS").toCharArray(RCPASS, sizeof(RCPASS));
        //RCPASS = server.arg("RCPASS").c_str();
      }
      if (server.hasArg("WiFiAP")) {
        WiFiAP = server.arg("WiFiAP");
        //RCPASS = server.arg("RCPASS").c_str();
      } else {
        WiFiAP = false;
      }
    }
  }

  if (server.hasArg("save")) { //detect button click
    switch (server.arg("save").toInt()) {
      case 1:
#ifdef DEBUG1
        Serial.println("RAM");//use only temp varible
#endif
        servos_dettach();
        delay(500);
        servos_attach();
        CalcExpoRates();
        print_variables();
        break;
      case 2:
        if (saveConfig())
#ifdef DEBUG1
          Serial.println("EEPROM saved");//save temp variables to json file
#endif
        break;
      case 3:
#ifdef DEBUG1
        Serial.println("REBOOT");
#endif
        rebooESP("/reboot.html");
        break;
      case 4:
        rebooESP("/restore.html");
        ResetToFactoryFlag = true;
        ResetToFactory();
#ifdef DEBUG1
        Serial.println("RESET ALL");//back to "factory defoults"
#endif
        break;
      case 5:
        if (saveConfig()) {
#ifdef DEBUG1
          Serial.println("EEPROM saved");//save temp variables to json file
          Serial.println("SET WiFi restarting");
#endif
          rebooESP("/SSID_pass.html");
        }else{
        rebooESP("/reboot.html");
        }

        break;
    }

  }


}

bool handleFileRead(String path) { // send the right file to the client (if it exists)
  if (server.args() != 0) {
    handleargs();
  }
#ifdef DEBUG1
  Serial.println("handleFileRead: " + path);
#endif
  if (path.endsWith("/")) {
    if (factory_boot) { //factory boot show faq
      path += "faq";
    } else {
      path += "control";          // If a folder is requested, send the index file
    }
  }
  if (path.endsWith("/conf") || path.endsWith("/config") || path.endsWith("/edit") || path.endsWith("/nastavi") || path.endsWith("/uredi") || path.endsWith("/root") || path.endsWith("/admin") || path.endsWith("/administrator")) {
    path = "/setup";
  }
  String contentType = getContentType(path);             // Get the MIME type
  String pathWithGz = path + ".gz";
  String pathWithHtml = path + ".html";
  String pathWithRCmode = path + String(RCmode) + ".html";


  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path) || SPIFFS.exists(pathWithHtml) || SPIFFS.exists(pathWithRCmode)) { // If the file exists, either as a compressed archive, or normal
    if (SPIFFS.exists(pathWithGz)) {                        // If there's a compressed version available
      path += ".gz";
    } else {                         // Use the compressed version
      if (SPIFFS.exists(pathWithHtml)) {
        path += ".html";
      } else {
        if (SPIFFS.exists(pathWithRCmode))
          path += String(RCmode) + ".html";
      }
    }
    File file = SPIFFS.open(path, "r");                    // Open the file
    size_t sent = server.streamFile(file, contentType);    // Send it to the client
    file.close();                                          // Close the file again
#ifdef DEBUG1
    Serial.println(String("\tSent file: ") + path);
#endif
    return true;
  }
#ifdef DEBUG1
  Serial.println(String("\tFile Not Found: ") + path);
#endif
  return false;                                          // If the file doesn't exist, return false
}
