 /*
 
   * 0

 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Esp32   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101             Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          

             
             
             
             
             
             
             
             
             
             
             
             
             
             
             
             
             22         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            5        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   23        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   19        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   18        D13        ICSP-3           15
 *
 * Night Mode Light Pin       3
 * Light Sensor   A2 0
 * Electrical Lock pins       4
 * Obsticle Avoidance Sensor    A1
 */

#include <SPI.h>
#include <MFRC522.h>
//


#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above
const int IN1 = 7;
const int IN2 = 6;
const int IN3 = 4;
const int IN4 = 2;
const int ENA = 5 ;
const int ENB = 3 ;
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
int lightSensor = A0; 
//int obsticleSensor = A1;// select the input pin for the potentiometer
//int out_light = 2;
int lock_pin = 8;
//int auto_light = 4 ;
int light_sensor_Value = 0;
//int obst_sensor_Value = 0;
void setup() {
 Serial.begin(9600);   // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  delay(4);       // Optional delay. Some board do need more time after init to be ready, see Readme
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
 pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (ENA, OUTPUT);
   pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);
  pinMode (ENB, OUTPUT);
  pinMode(lock_pin, OUTPUT);
  pinMode(lightSensor, INPUT);
//  pinMode(obsticleSensor, INPUT);
//   pinMode(out_light, OUTPUT);
//    pinMode(auto_light, OUTPUT);
   digitalWrite(lock_pin, HIGH);
    

}

void loop() {


//  SPI.begin();      // Init SPI bus
//  mfrc522.PCD_Init();   // Init MFRC522
//  delay(4);       // Optional delay. Some board do need more time after init to be ready, see Readme
//  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
////  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));

  
// light_sensor_Value = analogRead(lightSensor);
//obst_sensor_Value = analogRead(obsticleSensor);
 if(analogRead(A0)>=400){
    analogWrite(ENB, 150);
   
//Serial.println(analogRead(A0));

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
    }else{
   
//Serial.println("LIGHT Off");   
//    analogWrite(ENB, 130);

        digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

      }

//    if(obst_sensor_Value<=45){
//  digitalWrite(auto_light, HIGH);
//  
//  }else{
//     digitalWrite(auto_light, LOW);
//    }
  
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

 String content= "";
  byte letter;
for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "D9 A1 D1 8C" ) //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println("Welcome Ali");
    dooropen();
    delay(2000);
  doorclose();
  }else if (content.substring(1) == "47 09 71 B3") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println("Welcome 19-Cit-Evening");
   dooropen();
    delay(2000);
  doorclose();
  }
 
 else   {
    Serial.println(" Access denied");
    Serial.print("Please Try Again");


  }




 

  // Dump debug info about the card; PICC_HaltA() is automatically called
//  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
}

//  This function is Used to Automatically Open the Door
void dooropen(){
   digitalWrite(lock_pin, LOW);
     delay(800);
      analogWrite(ENA, 150);
   
//control direction 


  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  
  Serial.println(" Opening door ");
  delay(600);
  
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  
  }
//   This function is used to Closed the Door Automaticaly
void doorclose(){
    
   analogWrite(ENA, 150);
   
//control direction 

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  Serial.println(" Closing door ");
  delay(600);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  delay(800);
   digitalWrite(lock_pin, HIGH);
  
  }
