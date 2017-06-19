/*
 *    TDA7418 basic volume example
 * 
 *    Reads a potentiometer value every 50ms, maps them to the desired volume range 
 *    then set the volume accordingly and print the value on the serial monitor.
 */

#include "TDA7418.h"

TDA7418 TDA;


byte volumePin = 0;                 // Potentiometer wiper connected to pin A0
int volume = 0;

byte readInterval = 50;             // Amount of time between readings
unsigned long previousMillis = 0;   // Keeps last time we read the potentiometer


void setup() {

  Serial.begin(57600);              // Start serial communications at 57600 bps.
  Serial.println("Hello");

  TDA.begin();                      // Initialize TDA7418 with default values

  TDA.attenuator(0);                // Set all 5 attenuators to 0dB
  TDA.source(INPUT_SE3);            // Set source to input 3
  TDA.softMute(disable);            // Self-explanatory

}


void loop() {

  unsigned long currentMillis = millis();
  

  if ((unsigned long)(currentMillis - previousMillis) >= readInterval) {

    int reading = analogRead(volumePin);              // Read volume potentiometer position

    volume = map(reading, 0, 1023, -80, 0);           // Map values for volume control between -80dB and 0dB

    TDA.volume(volume);                               // Set volume
    
    Serial.println(volume);                           // Print mapped values

    previousMillis = currentMillis;                   
  }

}
