

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <EEPROM.h>



// Set these to run example.
#define FIREBASE_HOST "nodemcu-23ab2.firebaseio.com"
#define FIREBASE_AUTH "hIdSc019D7lC2821l1vRSBsbnnZdrWcv1tr6ehBR"






const int lightSensorPin = A0;
const int buttonPin = 0;
const int ledPin = 2;
int sensorValue;
float voltagem;
float graf_list[5];
int cont1[]={3,7,11,15,19};
String login="";
int i2=1;





void EEPROMWriteStr(int address, String value); //n Bytes

String EEPROMReadStr(int address);
void PontoDeAcesso();
void Estacao();


//WiFiServer server(80);
void setup() {
/*
 graf_list[0]=EEPROMReadLong(3);
 graf_list[1]=EEPROMReadLong(7);
 graf_list[2]=EEPROMReadLong(11);
 graf_list[3]=EEPROMReadLong(15);
 graf_list[4]=EEPROMReadLong(19);

*/  
 Serial.begin(115200);
 //EEPROM.begin(4000);
 pinMode(ledPin, OUTPUT);
 digitalWrite(ledPin, 0); 
Estacao();
//PontoDeAcesso();

 //delay(5000);
 //WiFi.softAPdisconnect(true);
 //server.begin();
 //Serial.println(F("Server started"));
 Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

}
void loop() {
  /*
while (i2){

 WiFiClient client = server.available();
  if (!client) {
    return;
  }
  Serial.println(F("new client"));

  client.setTimeout(5000); // default is 1000

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(F("request: "));
  Serial.println(req);
  //GET /gpio/1 HTTP/1.1

  // Match the request
  int val;
  if (req.indexOf(F("/gpio/0")) != -1) {
    val = 1;
  } else if (req.indexOf(F("/gpio/1")) != -1) {
    val = 0;
  } else {
    Serial.println(F("invalid request"));
    val = digitalRead(ledPin);
  }
login="";
  for (int i=0; i <= req.length();i++){
    char a[] = "H";    
    int i1= req.length();
    i1 =i1-9;
    if (i == a[0] || i == i1 ){
      
      break;
    }
    
    if(i>4){
      login = login + req[i];
    }
  }
  Serial.print("login = ");
  if(login == "favicon.ico"){
      Serial.println("aqui nao");

  }else{  Serial.println(login);}
  Serial.println("---------------------------------");
  if(login == "sair"){
    i2=0;
    break;
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
  Serial.println(F("Disconnecting from client"));
}*/
  
  if(digitalRead(buttonPin)==0){
  StaticJsonBuffer<500> jsonBuffer;   
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
        graf_list[i]= voltagem;
       }
       Serial.println(graf_list[i]);
 }

          
   for (int pin = 0; pin < 5; pin++) {
   // Read the analog input
   // Add the value at the end of the array
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








  }
void Estacao(){
  int cont=0;
  WiFi.begin(EEPROMReadStr(100),EEPROMReadStr(1000));

  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi... ");

   while (WiFi.status() != WL_CONNECTED) {
    cont=cont+1;
    delay(500);
    Serial.print(F("."));
    if (cont == 30){
      WiFi.disconnect();
      break;
    }
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(500);

  
}
void PontoDeAcesso(){
 Serial.println();
 Serial.print("Configuring access point...");
 WiFi.softAP("ESPap");

 IPAddress myIP = WiFi.softAPIP();
 Serial.print("AP IP address: ");
 Serial.println(myIP);
 //server.begin();
 Serial.println("HTTP server started");
 while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }



  
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

void EEPROMWriteStr(int offset, String s) {


    for (byte b = 0; b <= s.length(); b++) {
    EEPROM.write(offset + b, s[b]);
  }
    
}



String EEPROMReadStr(int offset) {
String s = "";
 while (offset < 4000) {
    if (EEPROM.read(offset) == 0) {
      break;
    }
    s += char(EEPROM.read(offset));
    offset++;
  }   
   
   return s; 
}








  
