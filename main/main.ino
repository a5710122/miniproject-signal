#include "DHT.h"
#define DHTPIN 2 // what pin we're connected to
#define DHTTYPE DHT22 // DHT 22 (AM2302)
// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor
// ---------------------------------------------------------------------------------

#include <SPI.h>
#include <SD.h>
// This example shows how to read and write data to and from an SD card file
// The circuit:
// * SD card attached to SPI bus as follows:
// ** MOSI - pin 11
// ** MISO - pin 12
// ** CLK - pin 13
// ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)
// ----------------------------------------------------------------------------------


DHT dht(DHTPIN, DHTTYPE); //DHT
File myFile; //SD Card

void setup() {
  Serial.begin(9600); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Start!");
  dht.begin();

  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  
}

void loop() {

   unsigned long time;

   time = millis();
   float count_time = time/1000;

   int count_time_int = int(count_time);

  switch (count_time_int) {
    case 600:
      log_data();

    }
  
}

void log_data(){
   float humidity; 
   float temperature;
   String dataString;
   if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Failed to read from DHT");
   } else {
      humidity = dht.readHumidity();
      temperature = dht.readTemperature();
      dataString = String(humidity) + "," + String(temperature); // convert to CSV
      save_data(dataString);
   }
}

void save_data(String dataString){

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("log_data.csv", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to log_data.csv...");
    myFile.println(dataString);
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening log_data.csv");
  }

  // re-open the file for reading:
  myFile = SD.open("log_data.csv");
  if (myFile) {
    Serial.println("log_data.csv:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening log_data.csv");
  }
}
