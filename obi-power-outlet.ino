#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>

const char* ssid = "<SSID>";
const char* password = "<PASSWORD>";

WiFiClient espClient;
ESP8266WebServer server(80);

bool relay_status = false;
bool inp_status = 1;

void setRelay(bool new_status){
  relay_status = new_status;
    
  if (relay_status){
    digitalWrite(12, 0);
    delay(50);
    digitalWrite(12, 1);
  } else {
    digitalWrite(5, 0);
    delay(50);
    digitalWrite(5, 1);
  }

  Serial.print("relay ");
  Serial.println(relay_status?"on":"off");
}

void on() {
  Serial.println("Relay on via HTTP");
  setRelay(true);
  server.send(200, "text/plain", "on\n");
}

void off() {
  Serial.println("Relay off via HTTP");
  setRelay(false);
  server.send(200, "text/plain", "off\n");
}

void toggle() {
  Serial.println("Relay toggle via HTTP");
  if(relay_status) {
    setRelay(false);
    server.send(200, "text/plain", "off\n");
  } else {
    setRelay(true);
    server.send(200, "text/plain", "on\n");
  }
}

void status() {
  Serial.println("Relay status via HTTP");
  if(relay_status)
    server.send(200, "text/plain", "on\n");
  else
    server.send(200, "text/plain", "off\n");
}

void setup(void){
  Serial.begin(115200); 
  delay(2000);

  pinMode(14, INPUT); // Push button
  pinMode(4, OUTPUT); // Blue LED
  pinMode(5, OUTPUT); // Relay off
  pinMode(12, OUTPUT); // Relay on
  setRelay(true);
  setRelay(false);
  
  Serial.println();
  Serial.println("Connecting to WiFi");

  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("WiFi connected with ip ");
  Serial.println(WiFi.localIP());

  server.on("/on", on);
  server.on("/off", off);
  server.on("/toggle", toggle);
  server.on("/status", status);
  server.begin();
  
  digitalWrite(4, 1);
 }

void loop(void){
  bool inp = digitalRead(14);
  if (inp == 0 and inp != inp_status) {
    Serial.println("Relay toggle via Button");
    setRelay(!relay_status);
  }
  inp_status = inp;

  server.handleClient();
  
  delay(50);
}
