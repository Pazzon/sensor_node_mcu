

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>



// Set these to run example.
#define FIREBASE_HOST "nodemcu-23ab2.firebaseio.com"
#define FIREBASE_AUTH "hIdSc019D7lC2821l1vRSBsbnnZdrWcv1tr6ehBR"



ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
}

const int lightSensorPin = A0;
const int buttonPin = 0;
int sensorValue;
long voltagem;
long graf_list[5];
int cont1[]={3,7,11,15,19};




void EEPROMWriteStr(int address, String value); //n Bytes

String EEPROMReadStr(int address);


StaticJsonBuffer<500> jsonBuffer;
 

void setup() {
/*
 graf_list[0]=EEPROMReadLong(3);
 graf_list[1]=EEPROMReadLong(7);
 graf_list[2]=EEPROMReadLong(11);
 graf_list[3]=EEPROMReadLong(15);
 graf_list[4]=EEPROMReadLong(19);

*/
  
 Serial.begin(115200);
 

 //pinMode(ledPin, OUTPUT);
 //digitalWrite(ledPin, 0);

 


  
 Serial.println();
 Serial.print("Configuring access point...");
 WiFi.softAP("ESPap");

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }

  //delay(5000);
 //WiFi.softAPdisconnect(true);
  
  WiFi.begin(EEPROMReadStr(100),EEPROMReadStr(1000));

  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi... ");

   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(500);
  //server.begin();
  //Serial.println(F("Server started"));
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
 
}

 
void loop() {
server.handleClient();
  /*
  if(digitalRead(buttonPin)==0){
    
  JsonObject& root = jsonBuffer.createObject();
  JsonArray& analogValues = root.createNestedArray("analog");
  
  sensorValue = analogRead(A0);   // Ler o pino Analógico A0
  voltagem = sensorValue * (3.3 / 1024.0);   // Converter a leitura analógica (que vai de 0 - 1023) para uma voltagem (0 - 3.3V)
  Serial.println(voltagem);   // Mostrar valor da voltagem no monitor serial
  Firebase.setFloat("SENSOR", voltagem); // Envia o dado da variavel voltagem para a variavel
  
  for (byte i = 0; i < 5; i = i + 1) {
    
    if (i<4){
       graf_list[i]=graf_list[i+1];}
    if(i==4){
        graf_list[i]= sensorValue;
       }
       Serial.println(graf_list[i]);
 }

          
   for (int pin = 0; pin < 5; pin++) {
   // Read the analog input
   // Add the value at the end of the array
   EEPROMWriteLong(cont1[pin], graf_list[pin]);     //Posicao 3, 4, 5 e 6 da EEPROM
   EEPROM.commit();//Salva o dado na EEPROM.
   analogValues.add(graf_list[pin]);
  }
  root.printTo(Serial);
  Firebase.set("Graf_Sensor",root);
  

  Serial.println();

  if (sensorValue < 341){ 
    Firebase.set("umidade", "seco");
  }
  if (342< sensorValue && sensorValue< 682) {
    Firebase.set("umidade", "umido");
  }
  if (sensorValue > 684){
    Firebase.set("umidade", "molhado");
    
    }


   
  
  }
  
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
  
  }


void EEPROMWriteInt(int address, int value) {
   byte hiByte = highByte(value);
   byte loByte = lowByte(value);

   EEPROM.write(address, hiByte);
   EEPROM.write(address + 1, loByte);   
}

int EEPROMReadInt(int address) {
   byte hiByte = EEPROM.read(address);
   byte loByte = EEPROM.read(address + 1);
   
   return word(hiByte, loByte); 
}

void EEPROMWriteLong(int address, long value) {
   byte four = (value & 0xFF);
   byte three = ((value >> 8) & 0xFF);
   byte two = ((value >> 16) & 0xFF);
   byte one = ((value >> 24) & 0xFF);

   EEPROM.write(address, four);
   EEPROM.commit();//Salva o dado na EEPROM.
   EEPROM.write(address + 1, three);
     EEPROM.commit();//Salva o dado na EEPROM.

   EEPROM.write(address + 2, two);
     EEPROM.commit();//Salva o dado na EEPROM.

   EEPROM.write(address + 3, one);
     EEPROM.commit();//Salva o dado na EEPROM.

   

   
}

long EEPROMReadLong(int address) {
   long four = EEPROM.read(address);
   long three = EEPROM.read(address + 1);
   long two = EEPROM.read(address + 2);
   long one = EEPROM.read(address + 3);

   return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

void EEPROMWriteStr(int address, String value) {
int nL;

   for (nL=0; nL < value.length(); nL++)  {
      EEPROM.write(address + nL, value.charAt(nL) );

      if (nL >= 1000) {
         break;
      }
   }

   if (nL < 1000) {
      EEPROM.write(address + nL, (char)0 );
   }
}

void EEPROMConcatStr(int address, String value) {
int nL;
char readByte;
bool nullFound = false;
int strPos = 0;

   for (nL=0; nL < 1000; nL++)  {

      if (!nullFound) {
         readByte = EEPROM.read(address + nL);

         if (readByte == (char)0) {
            nullFound = true;
         }
      } 
      
      if (nullFound) {
         EEPROM.write(address + nL, value.charAt(strPos) );          
         strPos++;

         if ((strPos + 1) > value.length()) {
            EEPROM.write(address + nL + 1, (char)0 );
            break;
         }
      }
   }     
}

String EEPROMReadStr(int address) {
String readStr = "";
char readByte;
int readAddress = address;

   do {
      readByte = EEPROM.read(readAddress);
      readStr += readByte;
      readAddress++;
   } while ( (readByte != (char)0) && (readAddress < (address + 1000)) );
   
   return readStr; 
}









  
