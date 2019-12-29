//
// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// FirebaseRoom_ESP8266 is a sample that demo using multiple sensors
// and actuactor with the FirebaseArduino library.

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "nodemcu-23ab2.firebaseio.com"
#define FIREBASE_AUTH "hIdSc019D7lC2821l1vRSBsbnnZdrWcv1tr6ehBR"
#define WIFI_SSID "VIVO-D90B"
#define WIFI_PASSWORD "1065629Derivaldonet"

//const int grovePowerPin = 2;
//const int vibratorPin = 5;
const int lightSensorPin = A0;
const int ledPin = 2;
const int buttonPin = 0;
//const int fanPin = 13;

void setup() {
  Serial.begin(9600);



  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.set("LDR", 0);
}



void loop() {

  int sensorValue = analogRead(A0);   // Ler o pino Analógico A0 onde está o LDR
  float voltagem = sensorValue * (3.3 / 1024.0);   // Converter a leitura analógica (que vai de 0 - 1023) para uma voltagem (0 - 3.3V), quanto de acordo com a intensidade de luz no LDR a voltagem diminui.
  Serial.println(voltagem);   // Mostrar valor da voltagem no monitor serial
  Firebase.setFloat("LDR", voltagem); // Envia o dado da variavel voltagem para a variavel LDR no Firebase
  delay(5000); // Espera 5 segundos
  


  

}
