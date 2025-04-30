// Jessies switch charger
// COM4
// 
#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 3); // RX, TX
// pH configuration
const int phPin = A1;
const float VREF = 5.0;
const float PH_MULTIPLIER = -5.234;  // Calibrated from 10.01 and 4.00 buffers
const float PH_OFFSET = 29.88;       // Calibrated from 10.01 and 4.00 buffers
const int SAMPLE_COUNT = 10;

// LED pins
const int greenLED = 8;
const int yellowLED = 9;
const int redLED = 6;

// Timing variables
unsigned long lastSampleTime = 0;
unsigned long lastReadTime = 0;
unsigned long lastTempSendTime = 0;
const unsigned long sendInterval = 2000;      // pH sampling every 2 seconds
const unsigned long sampleDelay = 10;         // 10 ms delay between individual ADC reads
const unsigned long tempSendInterval = 5000;    // Temperature message every 5 seconds

int buf[SAMPLE_COUNT];
int sampleIndex = 0;
bool sampling = false;

void setup() {
  Serial.begin(9600);    // Debug output to PC via USB
  mySerial.begin(9600);   // Communication to Arduino 3 using hardware Serial1

  // Initialize LED pins
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
}

void loop() {
  unsigned long now = millis();

  // --- pH Sampling Cycle ---
  if (!sampling && now - lastSampleTime >= sendInterval) {
    // Start a new sample cycle
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
    
    // Sort the array (bubble sort) to remove outliers.
    for (int i = 0; i < SAMPLE_COUNT - 1; i++) {
      for (int j = i + 1; j < SAMPLE_COUNT; j++) {
        if (buf[i] > buf[j]) {
          int temp = buf[i];
          buf[i] = buf[j];
          buf[j] = temp;
        }
      }
    }
    
    // Average the middle values (discard the two lowest and two highest).
    float avgValue = 0;
    for (int i = 2; i < SAMPLE_COUNT - 2; i++) {
      avgValue += buf[i];
    }
    avgValue /= (SAMPLE_COUNT - 4);
    
    // Convert the ADC value to voltage.
    float voltage = avgValue * (VREF / 1024.0);
    // Apply the calibrated conversion to get pH.
    float phValue = PH_MULTIPLIER * voltage + PH_OFFSET;
    
    // Debug output to Serial Monitor.
    // Serial.print("Voltage: ");
    // Serial.println(voltage, 3);
    // Serial.print("pH Value: ");
    // Serial.println(phValue, 2);
    
    // Determine water status based on the pH measurement.
    String phStatus;
   if (phValue >= 6.5 && phValue <= 8.5) {
      phStatus = "SAFE";  // Safe
      digitalWrite(greenLED, HIGH);
      digitalWrite(yellowLED, LOW);
      digitalWrite(redLED, LOW);
    } else if ((phValue >= 5.5 && phValue < 6.5) || (phValue > 8.5 && phValue <= 9.5)) {
      phStatus = "RISKY";  // Risky
      digitalWrite(greenLED, LOW);
      digitalWrite(yellowLED, HIGH);
      digitalWrite(redLED, LOW);
    } else {
      phStatus = "DANGEROUS";  // Dangerous
      digitalWrite(greenLED, LOW);
      digitalWrite(yellowLED, LOW);
      digitalWrite(redLED, HIGH);
    }
    
    // Create and send a message with the water status and pH value.
    // Format: <STATUS>,<pH measurement>
    String phMessage = String(phStatus) + "," + String(phValue, 2);
    
    mySerial.println(phMessage);
    Serial.println("A1: " + phMessage);
    lastSampleTime = now;
  }
}

