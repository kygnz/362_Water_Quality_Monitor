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
  This file instructs Arduino 1, which reads information from the pH sensor. 
  It constructs a message containing the status and value reading to send to 
  Arduino 2. It also updates its LEDs to provide a visual representation of the 
  pH reading status.

*/

#include <SoftwareSerial.h>

// Define serial connections
SoftwareSerial mySerial(4, 3); // RX, TX

// pH configuration
const int phPin = A1;
const float VREF = 5.0;
const float PH_MULTIPLIER = -5.234; 
const float PH_OFFSET = 29.88;       
const int SAMPLE_COUNT = 10;

// LED pins
const int greenLED = 8;
const int yellowLED = 9;
const int redLED = 6;

// Timing variables
unsigned long lastSampleTime = 0;
unsigned long lastReadTime = 0;
unsigned long lastTempSendTime = 0;
const unsigned long sendInterval = 2000;     
const unsigned long sampleDelay = 10;       
const unsigned long tempSendInterval = 5000;    

int buf[SAMPLE_COUNT];
int sampleIndex = 0;
bool sampling = false;

void setup() {
  Serial.begin(9600);    // Serial output for debugging
  mySerial.begin(9600);   // Serial connection for transmitting

  // Set LED pins for output
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
}

void loop() {
  unsigned long now = millis();

  // Create a sampling cycle for the pH sensor readings
  if (!sampling && now - lastSampleTime >= sendInterval) {
    sampleIndex = 0;
    sampling = true;
    lastReadTime = now;
  }
  
  if (sampling && sampleIndex < SAMPLE_COUNT && now - lastReadTime >= sampleDelay) {
    buf[sampleIndex++] = analogRead(phPin);
    lastReadTime = now;
  }
  
  if (sampling && sampleIndex == SAMPLE_COUNT) {
    sampling = false;
    
    // Remove outlier readings
    for (int i = 0; i < SAMPLE_COUNT - 1; i++) {
      for (int j = i + 1; j < SAMPLE_COUNT; j++) {
        if (buf[i] > buf[j]) {
          int temp = buf[i];
          buf[i] = buf[j];
          buf[j] = temp;
        }
      }
    }
    
    // Average the middle values
    float avgValue = 0;
    for (int i = 2; i < SAMPLE_COUNT - 2; i++) {
      avgValue += buf[i];
    }
    avgValue /= (SAMPLE_COUNT - 4);
    
    // Convert the ADC value to voltage
    float voltage = avgValue * (VREF / 1024.0);
    // Apply the calibrated conversion to get pH
    float phValue = PH_MULTIPLIER * voltage + PH_OFFSET;
    
    // Determine ph reading status based on thresholds
    String phStatus;
    // Safe threshold
   if (phValue >= 6.5 && phValue <= 8.5) {
      phStatus = "SAFE"; 
      digitalWrite(greenLED, HIGH);
      digitalWrite(yellowLED, LOW);
      digitalWrite(redLED, LOW);
      // Risk threshold
    } else if ((phValue >= 5.5 && phValue < 6.5) || (phValue > 8.5 && phValue <= 9.5)) {
      phStatus = "RISKY"; 
      digitalWrite(greenLED, LOW);
      digitalWrite(yellowLED, HIGH);
      digitalWrite(redLED, LOW);
      // Dangerous threshold
    } else {
      phStatus = "DANGEROUS"; 
      digitalWrite(greenLED, LOW);
      digitalWrite(yellowLED, LOW);
      digitalWrite(redLED, HIGH);
    }
    
    // Create a message with the water status and pH value
    String phMessage = String(phStatus) + "," + String(phValue, 2);
    
    mySerial.println(phMessage); // output for debugging

    // Send water status and value to arduino 2
    Serial.println("A1: " + phMessage);

    lastSampleTime = now;
  }
}

