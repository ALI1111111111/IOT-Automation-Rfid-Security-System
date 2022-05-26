#include <WiFi.h>
#include <HTTPClient.h>
#include <StringSplitter.h>
#include "DHT.h"
#define trigPin 4
#define echoPin 16
#define DHTPIN 26     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);
const char* ssid = "PTCL-BB";
const char* password = "16227583@";

String water = "https://iotproj.azurewebsites.net/water/store";
String serverName1 = "https://iotproj.azurewebsites.net/ard/create";

String data1,data2,data3,data4;
int b =0 ;

void setup() {
  Serial.begin(115200); 
Serial.println(F("DHTxx test!"));
pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(12, OUTPUT);
      pinMode(17, OUTPUT);
      pinMode(14, OUTPUT);
    pinMode(27, OUTPUT);
  dht.begin();
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
 
    if(WiFi.status()== WL_CONNECTED){
     


 pin1();
 ultrasonic();

     
    }
    else {
      Serial.println("WiFi Disconnected");
    }
 
  
}
void pin1(){
   
      HTTPClient http;

//      String serverPath = serverName + "/ard/1";
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverName1);
      
      // Send HTTP GET request
      int httpCode = http.GET();
 String data = http.getString();
    Serial.println(httpCode);
//  Serial.println("***************        Data Read        ***************");
//  Serial.println(data);
  StringSplitter *splitter = new StringSplitter(data, '/', 4);  // new StringSplitter(string_to_split, delimiter, limit)
  int itemCount = splitter->getItemCount();
//  Serial.println("***************        Data Splited        ***************");
//  Serial.println("Item count: " + String(itemCount));

  for(int i = 0; i < itemCount; i++)
  {
    String item = splitter->getItemAtIndex(i);
//    Serial.println("Item @ index " + String(i) + ": " + String(item));
    if(i == 0)
    {
     data1 = item;
    }
    if(i == 1)
    {
     data2 = item;
    }
    if(i == 2)
    {
    data3 = item;
    }if(i == 3)
    {
    data4 = item;
    }
  }
  data1.remove(0,7);
    data1.remove(data1.length()-3,data1.length());
    data2.remove(0,18);
  data2.remove(data2.length()-2,data2.length());
 data3.remove(0,18);
   data3.remove(data3.length()-2,data3.length());
   data4.remove(0,18);
   data4.remove(data4.length()-12,data4.length());

//Serial.println("***************        Data Filtered        ***************");
   Serial.println(data1.toInt());
   if(data1.toInt()== 1){
    
        digitalWrite(14, HIGH);
       
        }else if(data1.toInt()== 0){
          digitalWrite(14, LOW);
          }
          if(data2.toInt()== 1){
    
        digitalWrite(12, HIGH);
       
        }else if(data2.toInt()== 0){
          digitalWrite(12, LOW);
          }if(data3.toInt()== 1){
    
        digitalWrite(17, HIGH);
       
        }else if(data3.toInt()== 0){
          digitalWrite(17, LOW);
          }
 if(data4.toInt()== 1){
    
        digitalWrite(27, HIGH);
       
        }else if(data4.toInt()== 0){
          digitalWrite(27, LOW);
          }
   
//   Serial.println(data2);
//        Serial.println(data3);
//        Serial.println(data4);
//        Serial.print("[HTTP] connection closed or file end.\n");
     
   

    http.end();
  
    }

    void ultrasonic(){
      HTTPClient http;
       long duration, distance;
  digitalWrite(trigPin, LOW);        
  delayMicroseconds(2);              
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);           
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  Serial.print(distance);
//  if (distance >= 200 || distance <= 0)
//  {
//    Serial.println("Out of range");
//  }
//  else 
//  {
 float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
 Serial.println(h);
 Serial.println(t);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) ) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
//  int date = 5;

 String serverPath = water + "?distance="+distance +"&temparature="+t+"&humidity="+h;
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      
    Serial.print(distance);
    Serial.println(" cm");
    delay(500);

//  }

      
      }
   
   
  
 
