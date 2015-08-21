/*
University: Universita' degli Studi di Milano - Master in Computer Science
Address: Via Comelico 39, Milano
Thesis: Comelicottero Quadcopter
Built and programmed by
Andrea Toscano: andrea.toscano1@studenti.unimi.it   
*/

// GPS Data handler

#include <Wire.h>
#include "Ublox.h"
#define SERIAL_BAUD 115200
#define GPS_BAUD 115200
#define N_FLOATS 4

Ublox M8_Gps; // Init Ublox M8N receriver
volatile byte* arrayPointer;
const byte SLAVE_ADDRESS = 1;
byte lastMasterCommand = 99;
// Altitude - Latitude - Longitude - N Satellites
float gpsArray[N_FLOATS] = {0, 0, 0, 0};

void setup() {
   Serial.begin(SERIAL_BAUD);
   Serial1.begin(GPS_BAUD);
   Wire.begin(SLAVE_ADDRESS);
   Wire.onReceive(receiveCommand); 
   Wire.onRequest(requestEvent); 
}

void loop() {
   if(!Serial1.available())
		return;

  while(Serial1.available()){
        char c = Serial1.read();
         if (M8_Gps.encode(c)) {
          gpsArray[0] = M8_Gps.altitude;
          gpsArray[1] = M8_Gps.latitude;
          gpsArray[2] = M8_Gps.longitude; 
          gpsArray[3] = M8_Gps.sats_in_use;
        }
  }
/*  for(byte i = 0; i < N_FLOATS; i++) {
    Serial.print(gpsArray[i], 6);Serial.print(" ");
 }
  Serial.println("");
  Serial.println(""); */
}

// I2C rec command
void receiveCommand(int howMany){
    lastMasterCommand = Wire.read(); 
}

void requestEvent(){
  switch(lastMasterCommand) {
    case 1:
      writeI2C();
      break;
    // maybe there could be more cases 
  }
}

// I2C write command
// Sending data to Arduino Yun
void writeI2C() {
 byte buffer[4*N_FLOATS];
 arrayPointer = (byte*) &gpsArray;
 for(byte i = 0; i < 4*N_FLOATS; i++) 
     buffer[i] = arrayPointer[i];

 Wire.write(buffer,4*N_FLOATS);
}
