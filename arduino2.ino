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
using buzzers, LEDs, an LCD screen and Processing UI. Through this 
project, we provide an avenue to continuously collect and display data on water 
quality.


*/


#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>


#define ONE_WIRE_BUS 2 // Data wire is connected to pin D2
#define TURBIDITY_PIN A0

SoftwareSerial mySerial(6, 5); // RX, TX
int redPin= 11;
int greenPin = 10;
int bluePin = 9;

const long interval = 100;
unsigned long previousMillis = 0;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempsensors(&oneWire);

// Turbidity thresholds (values from analogRead)
const int SAFE_TURB_LOW  = 600;
const int SAFE_TURB_HIGH = 700;
const int WARN_TURB_LOW  = 500; 

// Temperature thresholds (in °F)
const float SAFE_TEMP_LOW  = 72.0;
const float SAFE_TEMP_HIGH = 79.0;
const float WARN_TEMP_LOW  = 68.0;
const float WARN_TEMP_HIGH = 82.0;

void setup() {
  Serial.begin(9600);
  tempsensors.begin();
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  mySerial.begin(9600);  // RX from Arduino 2, TX to Arduino 1

  // WIFI CONNECTION CODE SETUP HERE //
}

// the loop routine runs over and over again forever:
void loop() {

  // if (Serial1.available()) {
  //       String fromA1 = Serial1.readStringUntil('\n');
        // Serial.println(fromA1);
        
      // unsigned long currentMillis = millis();

      // if (currentMillis - previousMillis >= interval) {
      //     previousMillis = currentMillis;
          // Serial.println("***");
          if (mySerial.available()) {

            String fromA1 = mySerial.readStringUntil('\n');
            // Serial.println("***"+fromA1);

            tempsensors.requestTemperatures();
            float tempcelcius = tempsensors.getTempCByIndex(0);
            float tempF = (tempcelcius * (9.0/5.0)) + 32.0;
            // Serial.println(tempF);

            int turbVal = analogRead(TURBIDITY_PIN);
          
            // // turbidity status
            // String turbStatus;
            // if(turbVal >= 690){
            //   turbStatus = "SAFE";
            // }
            // else if (turbVal >= 600 && turbVal < 690) {
            // turbStatus = "RISKY";
            // } 
            // else { 
            //   turbStatus = "DANGEROUS";
            // }
            // turbidity status
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


            //  temperature status
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


            
            if ((tempStatus == "SAFE") && turbStatus == "SAFE") {
              analogWrite(redPin, 0);
              analogWrite(greenPin, 255);
              analogWrite(bluePin, 0);
            }
            else if (tempStatus == "DANGEROUS" || turbStatus == "DANGEROUS") {
              analogWrite(redPin, 255);
              analogWrite(greenPin, 0);
              analogWrite(bluePin, 0);
            }
            else if (tempStatus == "RISKY" || turbStatus == "RISKY") {
              analogWrite(redPin, 255);
              analogWrite(greenPin, 255);
              analogWrite(bluePin, 0);
            }
            
            float voltage = turbVal * (5.0 / 1023.0);
            // float NTU = -1120.4 * voltage * voltage + 5742.3 * voltage - 4352.9;
            // if (NTU < 0) NTU = 0;      // can’t have negative turbidity
            // if (NTU > 4000) NTU = 4000;
            // String turbMessage = "," + turbStatus + "," + String(turbVal) + ",";
            String turbMessage = "," + turbStatus + "," + String(voltage) + ",";
            String tempMessage = tempStatus + "," + String(tempF);

            String fullMessage = fromA1 + turbMessage + tempMessage;

            Serial.println(fullMessage);
            // Serial1.println(fullMessage); // Send to Arduino 3
            // mySerial.println(turbStatus + "," + String(turbVal) + "," + tempStatus + "," + String(tempF));
            mySerial.println(fullMessage);
          }
      // }


  }


  // unsigned long currentMillis = millis();

  // if (currentMillis - previousMillis >= interval) {
  //     previousMillis = currentMillis;

  //     if (Serial1.available()) {
  //       String fromA1 = Serial1.readStringUntil('\n');
  //       Serial.println(fromA1);

  //       tempsensors.requestTemperatures();
  //       float tempcelcius = tempsensors.getTempCByIndex(0);
  //       float tempF = (tempcelcius * (9.0/5.0)) + 32.0;

  //       int turbVal = analogRead(TURBIDITY_PIN);
      
  //       // turbidity status
  //       String turbStatus;
  //       if(turbVal >= SAFE_TURB_LOW){
  //         turbStatus = "SAFE";
  //       }
  //       else if (turbVal >= WARN_TURB_LOW && turbVal < SAFE_TURB_LOW) {
  //       turbStatus = "RISKY";
  //       } 
  //       else { 
  //         turbStatus = "DANGEROUS";
  //       }


  //       //  temperature status
  //       String tempStatus;
  //       if (tempF >= SAFE_TEMP_LOW && tempF <= SAFE_TEMP_HIGH) {
  //         tempStatus = "S";
  //       } 
  //       else if ((tempF >= WARN_TEMP_LOW && tempF < SAFE_TEMP_LOW) || 
  //               (tempF > SAFE_TEMP_HIGH && tempF <= WARN_TEMP_HIGH)) {
  //         tempStatus = "R";
  //       } 
  //       else {
  //         tempStatus = "D";
  //       }


  //       // Serial.print("Temperature: ");
  //       // Serial.print(tempF);
  //       // Serial.print(" °F - ");
  //       // Serial.println(tempStatus);
        
  //       String turbMessage = turbStatus + "," + String(turbVal);
  //       String tempMessage = tempStatus + "," + String(tempF);

  //       // Serial.print("Turbidity: ");
  //       // Serial.print(turbVal);
  //       // Serial.print(" - ");
  //       // Serial.println(turbStatus);

  //       // setLED(overallStatus);

  //       // String fullMessage = fromA1
  //       // delay(100); // Give buffer some time


  //       String fullMessage = fromA1 + turbMessage + tempMessage;
  //       Serial1.println(fullMessage); // Send to Arduino 1

  //     }
  // }



// }


