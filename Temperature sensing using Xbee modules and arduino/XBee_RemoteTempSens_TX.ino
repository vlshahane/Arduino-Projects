/*
 * Wireless Temperature Sensing and Alarm System
 *     Feature: Wireless Communication & Temperature Sensing
 *     Role: Transmitter
 *
 *     Hardware: XBee module, Temperature Sensor, Alarm
 */

#include "DHT.h"

#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht(DHTPIN, DHTTYPE);

float tempV;  // Voltage readings from the temperature sensor
float tempC;
float tempF;
int tempReading;
float alarmThreshold_F = 70.00;   // Temperature threshold (in F)
float alarmThreshold_C = 25.00;   // Temperature threshold (in C)

int alarmFreq = 1000;     //in Hz
int alarmDuration = 1000; //in ms

int pinSpeaker = 13;     // digital channel
int pinSwitch = 6;       // digital channel

unsigned long int buzzTime = 0;

void setup()
{
  // Buadrate is 9600 by default
  Serial.begin(9600);
  
  // Setup the DHT22
  dht.begin();
}

void loop()
{
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  // print out the temperature in Celsius
  tempC = t;
  Serial.println(tempC);  
  delay(1000);  //waiting a second
}





