/*
   Copyright (c) 2015, Majenko Technologies
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

 * * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

 * * Redistributions in binary form must reproduce the above copyright notice, this
     list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.

 * * Neither the name of Majenko Technologies nor the names of its
     contributors may be used to endorse or promote products derived from
     this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
   ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>




// Set these to run example.
#define FIREBASE_HOST "nodemcu-23ab2.firebaseio.com"
#define FIREBASE_AUTH "hIdSc019D7lC2821l1vRSBsbnnZdrWcv1tr6ehBR"
#ifndef STASSID
#define STASSID "VIVO-D90B"
#define STAPSK  "1065629Derivaldonet"
#endif
const char* ssid     = STASSID;
const char* password = STAPSK;

//WiFiServer server(80);
//ESP8266WebServer server(80);
ESP8266WiFiMulti WiFiMulti;

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/


const int lightSensorPin = A0;
//const int ledPin = 2;
const int buttonPin = 0;
float graf_list[]={2, 4, 8, 3, 6};
void setup() {
  delay(1000);
  Serial.begin(115200);
  //pinMode(ledPin, OUTPUT);
  //digitalWrite(ledPin, 0);

  
 // Serial.println();
 // Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
 // WiFi.softAP("ESPap");

  //IPAddress myIP = WiFi.softAPIP();
  //Serial.print("AP IP address: ");
  //Serial.println(myIP);
  
  //Serial.println("HTTP server started");

  //delay(5000);
 // WiFi.softAPdisconnect(true);


  // We start by connecting to a WiFi network
  
  WiFiMulti.addAP(ssid, password);

  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi... ");

  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(500);
  //server.begin();
  //Serial.println(F("Server started"));
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.set("Graf_Sensor",graf_list[0]);
  
}
 int cont=0;
 int sensorValue;
 float voltagem;
 
void loop() {
   StaticJsonBuffer<500> jsonBuffer;
   JsonObject& root = jsonBuffer.createObject();
   JsonArray& analogValues = root.createNestedArray("analog");
   JsonArray& digitalValues = root.createNestedArray("digital");



  if(digitalRead(buttonPin)==0){
  sensorValue = analogRead(A0);   // Ler o pino Analógico A0 onde está o LDR
  voltagem = sensorValue * (3.3 / 1024.0);   // Converter a leitura analógica (que vai de 0 - 1023) para uma voltagem (0 - 3.3V), quanto de acordo com a intensidade de luz no LDR a voltagem diminui.
  Serial.println(voltagem);   // Mostrar valor da voltagem no monitor serial
  Firebase.setFloat("LDR", voltagem); // Envia o dado da variavel voltagem para a variavel LDR no Firebase
  if (cont <= 5){
    for (byte i = 0; i < 5; i = i + 1) {
       Serial.println(graf_list[i]);
       if (i<4){
       graf_list[i]=graf_list[i+1];}
       if(i==4){
        graf_list[i]= voltagem;
       }
          }

          
               Firebase.set("Graf_Sensor","33");
               Serial.println(cont);
               cont += 1;
    

    
    
    
  }
  if (cont>5){
    cont= 0;
  }
  
 
  if (sensorValue < 341){ 
    Firebase.set("umidade", "seco");
  }
  if (342< sensorValue && sensorValue< 682) {
    Firebase.set("umidade", "umido");
  }
  if (sensorValue > 684){
    Firebase.set("umidade", "molhado");
    
    }


    for (int pin = 0; pin < 6; pin++) {
    // Read the analog input
    int value = analogRead(pin);

    // Add the value at the end of the array
    analogValues.add(value);
  }
    for (int pin = 0; pin < 14; pin++) {
    // Read the digital input
    int value = digitalRead(pin);

    // Add the value at the end of the array
    digitalValues.add(value);
  }

   root.printTo(Serial);
   Firebase.set("Graf_Sensor",root);
   Serial.println();

     
    
  
// Check if a client has connected
 /* WiFiClient client = server.available();
  if (!client) {
    return;
  }
  Serial.println(F("new client"));

  

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(F("request: "));
  Serial.println(req);

  // Match the request
  int val;
  if (req.indexOf(F("/gpio/0")) != -1) {
    val = 0;
  } else if (req.indexOf(F("/gpio/1")) != -1) {
    val = 1;
  } else {
    Serial.println(F("invalid request"));
    val = digitalRead(ledPin);
  }

  // Set LED according to the request
  digitalWrite(ledPin, val);

  // read/ignore the rest of the request
  // do not client.flush(): it is for output only, see below
  while (client.available()) {
    // byte by byte is not very efficient
    client.read();
  }

  // Send the response to the client
  // it is OK for multiple small client.print/write,
  // because nagle algorithm will group them into one single packet
  client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now "));
  client.print((val) ? F("high") : F("low"));
  client.print(F("<br><br>Click <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/1'>here</a> to switch LED GPIO on, or <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/0'>here</a> to switch LED GPIO off.</html>"));

  // The client will actually be *flushed* then disconnected
  // when the function returns and 'client' object is destroyed (out-of-scope)
  // flush = ensure written data are received by the other side
  Serial.println(F("Disconnecting from client"));8*/
  
  }}
