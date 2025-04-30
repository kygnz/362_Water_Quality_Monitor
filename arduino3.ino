// Fariha's - ipad charger
// COM3

// include the library code:
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include "pitches.h"

SoftwareSerial mySerial(0, 13);

int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

// play song function
void playSong(){
    for (int thisNote = 0; thisNote < 8; thisNote++) {
      int noteDuration = 1000 / noteDurations[thisNote];
      tone(8, melody[thisNote], noteDuration);
    }
}

// set up index and quote values
int index = 0;
String phStatus = "";
String tempStatus = "";
String turbStatus = "";


String ph = "";
String temp = "";
String turb = "";

// set up pins
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int phPin = 6;
int tempPin = 10;
int turbPin = 9;
int buzzerPin = 8;

bool buzzerActive = false;
unsigned long buzzerStartTime = 0;
const unsigned long buzzerDuration = 2000;
// time variables
unsigned long prev = 0;  
const long interval = 3000; // half a second

char buffer[40];
String toSend = "";

void setup() {

  Serial.begin(9600);   // For Serial Monitor
  Serial1.begin(9600);  // RX from Arduino 3

  pinMode(phPin,OUTPUT);
  pinMode(tempPin,OUTPUT);
  pinMode(turbPin,OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  // set up number of columns and rows
  lcd.begin(16, 2);

  // set cursor to middle and print out name
  
  
}


void printText(String quote){

   String snippet = quote.substring(index, index + 16); // get a snippet of code of 16 characters

   // if you end up with something that is less than 16 charcters, get whats left of the quote
   if (snippet.length() < 16){
    snippet += quote.substring(0, 16 - snippet.length());
   }

   // set the cursor to the first spot and print out the code
   lcd.setCursor(0,1);
   lcd.print(snippet);


   // if your index ends up being bigger than the length of the code, 
   if (index > quote.length()){
    index = 0;
   }
   
   index++;
   index++;
   
}

void updateLED(String stat, int pin){
  if (stat == "SAFE"){
    analogWrite(pin, 0);
  }
  else if (stat == "RISKY"){
    analogWrite(pin, 10); 
  }
  else{
    analogWrite(pin, 254);
  }
  
}

void loop() {


  // String message = "";
  if (Serial1.available()) {
    String message = Serial1.readStringUntil('\n');
    // Serial.println(message);
    
    //Serial.print("Final message: ");
    // Serial.println(message);
    toSend = message;
    // toSend = "DANGEROUS,7.0, RISKY,123,SAFE,75.6";

    int commaInd = message.indexOf(',');
    phStatus = message.substring(0,commaInd);
    message = message.substring(commaInd + 1);
   
    commaInd = message.indexOf(',');
    ph = message.substring(0,commaInd);
    ph.trim();
    message = message.substring(commaInd + 1);
    // Serial.println(ph);
    commaInd = message.indexOf(',');
    tempStatus = message.substring(0,commaInd);
    message = message.substring(commaInd + 1);

    commaInd = message.indexOf(',');
    temp = message.substring(0,commaInd);
    temp.trim();
    message = message.substring(commaInd + 1);

    commaInd = message.indexOf(',' );
    turbStatus = message.substring(0,commaInd);
    message = message.substring(commaInd + 1);

    turb = message;
    turb.trim();


   
  
    lcd.setCursor(0,0);   // move to the top‐left
    lcd.print("                ");  // 16 spaces to blank out the line
    lcd.setCursor(0,0);
    // lcd.print(buffer);
    if (phStatus == "SAFE" && tempStatus == "SAFE" && turbStatus == "SAFE"){
      sprintf(buffer, "ALL SAFE");
      lcd.print(buffer);
    }
    else if (phStatus == "DANGEROUS" || tempStatus == "DANGEROUS" || turbStatus == "DANGEROUS"){
      sprintf(buffer, "DANGEROUS");
      lcd.print(buffer);
      buzzerActive = true;
      buzzerStartTime = millis();
      playSong();
    }
    else{
      sprintf(buffer, "RISKY");
      lcd.print(buffer);
    }


    updateLED (phStatus, phPin);
    updateLED (tempStatus, tempPin);
    updateLED (turbStatus, turbPin);

  }
    String toDisplay = " pH: " + ph + " Temp: " + temp + "F Turb: " + turb + "V";
    
    unsigned curr = millis();
  
    // repeat every half second
    if (curr - prev >= interval){
      prev = curr;
      lcd.setCursor(0,1); // set the cursor to the very beginning
      printText(toDisplay); // call function to print on that line
      Serial.println(toSend);
      // Serial.println("RISKY,4.0,SAFE,37,SAFE,63");
    }

  // }
  
}