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
  This file instructs Arduino 3, which takes a message from Arduino 2
  that contains information about the status and value reading of 
  the pH, temperature, and turbidity sensors. It sends the information to Processing
  to set up the user interface, and it updates the LCD and LEDs to provide
  visual indicators of the sensor status.

*/

#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include "pitches.h"

// Define serial connections
SoftwareSerial mySerial(0, 13); // RX, TX

// Buzzer variables
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

// set up index and quote values
int index = 0;
String phStatus = "";
String tempStatus = "";
String turbStatus = "";
String ph = "";
String temp = "";
String turb = "";

// LCD pins
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Sensor data pins
int phPin = 6;
int tempPin = 10;
int turbPin = 9;
int buzzerPin = 8;

// buzzer variables
bool buzzerActive = false;
unsigned long buzzerStartTime = 0;
const unsigned long buzzerDuration = 2000;

// Timing variables
unsigned long prev = 0;  
const long interval = 3000;

// String to send to Processing
char buffer[40];
String toSend = "";

void setup() {

  Serial.begin(9600);   // Serial output for debugging
  Serial1.begin(9600);  // Serial connection for recieving

  // Set LED pins for output
  pinMode(phPin,OUTPUT);
  pinMode(tempPin,OUTPUT);
  pinMode(turbPin,OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Start LCD screen
  lcd.begin(16, 2);

}


void loop() {

  if (Serial1.available()) {
    // Read message from arduino 2
    String message = Serial1.readStringUntil('\n');
    
    // Send the message using Processing
    toSend = message;

    // Deconstruct message to extract pH status 
    int commaInd = message.indexOf(',');
    phStatus = message.substring(0,commaInd);
    message = message.substring(commaInd + 1);
   
    // Deconstruct message to extract pH value
    commaInd = message.indexOf(',');
    ph = message.substring(0,commaInd);
    ph.trim();
    message = message.substring(commaInd + 1);
    
    // Deconstruct message to extract temperature status 
    commaInd = message.indexOf(',');
    tempStatus = message.substring(0,commaInd);
    message = message.substring(commaInd + 1);

    // Deconstruct message to extract temperature value
    commaInd = message.indexOf(',');
    temp = message.substring(0,commaInd);
    temp.trim();
    message = message.substring(commaInd + 1);

    // Deconstruct message to extract turbidity status 
    commaInd = message.indexOf(',' );
    turbStatus = message.substring(0,commaInd);
    message = message.substring(commaInd + 1);

    // Leftover is the turbidity value
    turb = message;
    turb.trim();

    // Reset LCD
    lcd.setCursor(0,0); 
    lcd.print("                "); 
    lcd.setCursor(0,0);

    // Set first line of LCD to display the status based on the 3 sensor readings
    // If all readings are safe, overall status is safe
    if (phStatus == "SAFE" && tempStatus == "SAFE" && turbStatus == "SAFE"){
      sprintf(buffer, "ALL SAFE");
      lcd.print(buffer);
    }
    // If at least one reading is dangerous, then overall status is dangerous
    else if (phStatus == "DANGEROUS" || tempStatus == "DANGEROUS" || turbStatus == "DANGEROUS"){
      sprintf(buffer, "DANGEROUS");
      lcd.print(buffer);
      buzzerActive = true;
      buzzerStartTime = millis();
      playSong();
    }
    // If no readings are dangerous, but at least one is risky, overall status is risky
    else{
      sprintf(buffer, "RISKY");
      lcd.print(buffer);
    }

    // Update the LEDs based on the respective sensor status
    updateLED(phStatus, phPin);
    updateLED(tempStatus, tempPin);
    updateLED(turbStatus, turbPin);

  }

  // Display reading values on the LCD screen
  String toDisplay = " pH: " + ph + " Temp: " + temp + "F Turb: " + turb + "V";
    
  unsigned curr = millis();

  // Update LCD every interval
  if (curr - prev >= interval){
    prev = curr;
    lcd.setCursor(0,1); 
    // function to set display text
    printText(toDisplay); 
    // output for debugging
    Serial.println(toSend);
  }
}



// This function sets the buzzer alert tone
void playSong(){
    for (int thisNote = 0; thisNote < 8; thisNote++) {
      int noteDuration = 1000 / noteDurations[thisNote];
      tone(8, melody[thisNote], noteDuration);
    }
}



// This function sets the LCD display to output sensor information
void printText(String quote){

    // Sets up LCD to scroll
    String snippet = quote.substring(index, index + 16);

    // if you end up with something that is less than 16 charcters, get whats left of the quote
    if (snippet.length() < 16){
    snippet += quote.substring(0, 16 - snippet.length());
    }

    // Set the cursor to print string
    lcd.setCursor(0,1);
    lcd.print(snippet);

    // if your index ends up being bigger than the length of the code, 
    if (index > quote.length()){
    index = 0;
    }
    
    index++;
    index++;
   
}

// This function updates an LED based on a given status and pin
void updateLED(String stat, int pin){
  // Green if status is safe
  if (stat == "SAFE"){
    analogWrite(pin, 0);
  }
  // Yellow if status is risky
  else if (stat == "RISKY"){
    analogWrite(pin, 10); 
  }
  // Red if status is dangerous
  else{
    analogWrite(pin, 254);
  }
}


