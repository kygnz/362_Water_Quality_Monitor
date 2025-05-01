/*

  Jessie Nouna, jnoun2, jnoun2@uic.edu 
  Kyla Gonzalez, kgonza39, kgonza39@uic.edu
  Fariha Siyadath, fsiya2, fsiya2@uic.edu

  Group #42
  Project name: AquaScout

  Abstract
  Our project is a water quality sensor that will monitor the PH, 
  the temperature, and the turbidity of water using the respective waterproof 
  sensors. Information regarding the water quality will be displayed 
  using buzzers, LEDs, an LCD screen. An accompanying UI, built with Processing,
  tracks metrics and sends alerts to view water quality over time. Through this 
  project, we provide an avenue to continuously collect and display data on water 
  quality for drinking water.

  File Summary
  This file instructs Arduino 2, which takes a message from Arduino 1 that contains
  information about the status and value reading of the pH sensor. It also reads information
  from the temperature and turbidity sensors, and constructs a single message containing
  status and value readings of all 3 sensors to send to Arduino 3. It also updates
  its LED to provide a visual representation of the temperature and turbidity status.

*/

#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>

// sensor pins
#define ONE_WIRE_BUS 2 
#define TURBIDITY_PIN A0

// sensor configuration
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempsensors(&oneWire);

// define serial connections
SoftwareSerial mySerial(6, 5); // RX, TX

// LED pins
int redPin= 11;
int greenPin = 10;
int bluePin = 9;

// Timing variables
const long interval = 100;
unsigned long previousMillis = 0;

void setup() {
    // serial output for debugging
    Serial.begin(9600);   
    tempsensors.begin();

    // prepare led ppins for output
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);

    // serial connection for recieving/transmitting
    mySerial.begin(9600); 
}



void loop() {

    if (mySerial.available()) {

      // pH data from arduino 1
      String fromA1 = mySerial.readStringUntil('\n');

      // get temperature sensor reading
      tempsensors.requestTemperatures();
      float tempcelcius = tempsensors.getTempCByIndex(0);
      float tempF = (tempcelcius * (9.0/5.0)) + 32.0;

      // get turbidity sensor reading
      int turbVal = analogRead(TURBIDITY_PIN);
    

      //  Determine temperature status based on threshold
      String tempStatus;
      if (tempF >= 75) {
        tempStatus = "SAFE";
      } 
      else if ((tempF >= 60 && tempF < 74)) {
        tempStatus = "RISKY";
      } 
      else {
        tempStatus = "DANGEROUS";
      }


      // Determine turbidity status based on threshold
      String turbStatus;
      if(turbVal >= 500){
          if (turbVal >= 620 && turbVal < 665) {
              turbStatus = "RISKY";
          } 
          else{
            turbStatus = "SAFE";
          }
      }
      else { 
          turbStatus = "DANGEROUS";
      }

      // Set LED based on status of temperature and turbidity readings
      // If both are safe, LED is green
      if ((tempStatus == "SAFE") && turbStatus == "SAFE") {
        analogWrite(redPin, 0);
        analogWrite(greenPin, 255);
        analogWrite(bluePin, 0);
      }
      // If at least one is dangerous, LED is red
      else if (tempStatus == "DANGEROUS" || turbStatus == "DANGEROUS") {
        analogWrite(redPin, 255);
        analogWrite(greenPin, 0);
        analogWrite(bluePin, 0);
      }
      // If at least one is risky and none are dangerous, LED is yellow
      else if (tempStatus == "RISKY" || turbStatus == "RISKY") {
        analogWrite(redPin, 255);
        analogWrite(greenPin, 255);
        analogWrite(bluePin, 0);
      }
      
      // convert turbidity reading to voltage
      float voltage = turbVal * (5.0 / 1023.0);

      // Append status of pH, temperature and turbidity status and reading into one message
      String turbMessage = "," + turbStatus + "," + String(voltage) + ",";
      String tempMessage = tempStatus + "," + String(tempF);
      String fullMessage = fromA1 + turbMessage + tempMessage;

      Serial.println(fullMessage); // Serial output for debugging

      // Send status of all 3 input sensors to arduino 3
      mySerial.println(fullMessage);
    }

  }


