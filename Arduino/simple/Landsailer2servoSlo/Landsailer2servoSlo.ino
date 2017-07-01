#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

#include <Servo.h> 
Servo S4; 
Servo S5;  

/* Set these to your desired credentials. */
const char *ssid = "Tramontana";
const char *password = "zotkazotka";

ESP8266WebServer server(80);

/* Go to http://192.168.4.1 */
String glava ="<body bgcolor=\"#000000\" text=\"#AAAAAA\" link=\"#00FF00\" vlink=\"#00FF00\"><center><font size=\"6\"><b>Tramontana</b></font><br><font size=\"4\">Tabor ZOTKS - poletje 2017</font><br><font size=\"12\"><a href=\"l5\">L50%</a>&emsp;&emsp;&emsp;<a href=\"n\">Naprej</a>&emsp;&emsp;&emsp;<a href=\"d5\">D50%</a><br><br><a href=\"l\">Levo</a>&emsp;&emsp;&emsp;<a href=\"d\">Desno</a><br><br><a href=\"z\">Zategni</a>&emsp;&emsp;&emsp;<a href=\"s\">Sredina</a>&emsp;&emsp;&emsp;<a href=\"c\">Sprosti</a><br><font size=\"5\">Zadnji izveden ukaz: ";

//ob root oz priklopu telefona
void Root() { 
  S4.write(90);
  S5.write(90);
  digitalWrite(2, LOW);
  server.send(200, "text/html",glava+"Vklop");
  digitalWrite(2, HIGH);
}

//kolesa
void Levo() {
  S4.write(20);
  digitalWrite(2, LOW);
  server.send(200, "text/html",glava+"Levo");
  digitalWrite(2, HIGH);
  
}

void Desno() {
  S4.write(160);
  digitalWrite(2, LOW);
  server.send(200, "text/html",glava+"Desno");
  digitalWrite(2, HIGH);
}

void L50() {
  S4.write(45);
  digitalWrite(2, LOW);
  server.send(200, "text/html",glava+"L50%");
  digitalWrite(2, HIGH);
  
}

void D50() {
  S4.write(135);
  digitalWrite(2, LOW);
  server.send(200, "text/html",glava+"D50%");
  digitalWrite(2, HIGH);
}

void Naprej() {
  S4.write(90);
  digitalWrite(2, LOW);
  server.send(200, "text/html",glava+"Desno");
  digitalWrite(2, HIGH);
}


//jadro
void Sprosti() {
   S5.write(180);
  digitalWrite(2, LOW);
  server.send(200, "text/html",glava+"Sprosti");
  digitalWrite(2, HIGH);
 
}
void Zategni() {
  S5.write(0);
  digitalWrite(2, LOW);
  server.send(200, "text/html",glava+"Zategni");
  digitalWrite(2, HIGH);
}
void Sredina() {
  S5.write(90);
  digitalWrite(2, LOW);
  server.send(200, "text/html",glava+"Zategni");
  digitalWrite(2, HIGH);
}

void setup() {

  delay(50); 
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  server.on("/",Root);
 server.on("/l", Levo);
 server.on("/d", Desno);
 server.on("/n", Naprej);
  server.on("/d5", D50);
 server.on("/l5", L50);
 
 server.on("/c", Sprosti);
 server.on("/z", Zategni);
 server.on("/s", Sredina);
  server.begin();
 
 S4.attach(4); //smer
S4.write(90);

S5.attach(16); //jadro
S5.write(90);

  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
}

void loop() {
  server.handleClient();
}
