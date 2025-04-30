import processing.serial.*;
import java.util.ArrayList;

Serial myPort;

int appState = 0; // main state
MainButton summaryButton;
MainButton alertsButton;
MainButton historyButton;
String inputString = "";

// Store data for each sensor
ArrayList<Float> temperatureData = new ArrayList<Float>();
ArrayList<Float> turbidityData = new ArrayList<Float>();
ArrayList<Float> phData = new ArrayList<Float>();

// fresh data monitoring
long lastSerialTime = 0;
final int connThreshold = 5000;

int startTime;

ArrayList<Alert> pendingAlerts = new ArrayList<Alert>();
Alert currentAlert = null;
int currentAlertStartTime = 0;
int alertDisplayDuration = 3000;


int waterScore = 100;
//PFont scoreFont;


void setup() {
  size(430, 770);
  //println(Serial.list());
  myPort = new Serial(this, Serial.list()[0], 9600); // Connect to Arduino
  myPort.bufferUntil('\n');  
  frontendSetup();
  summaryButton = new MainButton(0, height - 50, width / 3, 50, "Summary", true);
  alertsButton = new MainButton(width / 3, height - 50, width / 3, 50, "Alerts", false);
  historyButton = new MainButton(width / 3 * 2, height - 50, width / 3, 50, "History", false);
  startTime = millis();
  
  summarySetup();
  
}


void addSampleTemperatureData() {
  // Add some sample temperature data for testing purposes
  for (int i = 0; i < 10; i++) {
    float sampleTemp = random(60, 100); // Random temperature between 60°F and 100°F
    temperatureData.add(sampleTemp);
  }
}

void draw() {
  background(240);

  // App state menu
  if (appState == 0) {
    drawSummaryScreen();
    summaryButton.isPressed = true;
    alertsButton.isPressed = false;
    historyButton.isPressed = false;
  } 
  else if (appState == 1) {
    displayAlertsScreen(alerts);
    summaryButton.isPressed = false;
    alertsButton.isPressed = true;
    historyButton.isPressed = false;
  }
  else if (appState == 2) {
    //drawHistoryScreen();
    if (showAlertDetails) {
      drawAlertDetails();
    } else {
      drawHistoryScreen();
    }
    summaryButton.isPressed = false;
    alertsButton.isPressed = false;
    historyButton.isPressed = true;
  }
  else if (appState == 3) {
     drawTempScreen();
  }
  else if (appState == 4) {
     drawTurbidityScreen();
  }
  else if (appState == 5) {
     drawPhScreen();
  }
  
  // Draw navigation buttons
  summaryButton.display();
  alertsButton.display();
  historyButton.display();
  
  drawDataAlert();
}


void serialEvent(Serial myPort) {
  lastSerialTime = millis();
  inputString = myPort.readStringUntil('\n');
  if (inputString != null) {
    inputString = trim(inputString);  
    
    // Split the input into parts based on commas
    String[] parts = split(inputString, ",");
    
    // Check if array has pH, Temperature, Turbidity
    if (parts.length == 6) {
      try {
        // Extract status and value
        String phStatus = parts[0].trim();  
        float phValue = float(parts[1].trim()); 
        String turbidityStatus = parts[2].trim();  
        float turbidityValue = float(parts[3].trim());
        String tempStatus = parts[4].trim(); 
        float tempValue = float(parts[5].trim());

        // Create alerts
        addSensorDataAndAlert("pH", phStatus, phValue);
        addSensorDataAndAlert("Temperature", tempStatus, tempValue);
        addSensorDataAndAlert("Turbidity", turbidityStatus, turbidityValue);
        
        updateScore(phStatus, tempStatus, turbidityStatus);

      } catch (Exception e) {
        println("Error parsing sensor data: " + e.getMessage());
      }
    }
  }
}



void updateScore(String phStatus, String tempStatus, String turbidityStatus) {
  int score = 100;

  // Apply penalties
  if (phStatus.equals("RISKY")) {
    score -= 15;
  } else if (phStatus.equals("DANGEROUS")) {
    score -= 30;
  }

  if (tempStatus.equals("RISKY")) {
    score -= 15;
  } else if (tempStatus.equals("DANGEROUS")) {
    score -= 30;
  }

  if (turbidityStatus.equals("RISKY")) {
    score -= 15;
  } else if (turbidityStatus.equals("DANGEROUS")) {
    score -= 30;
  }

  waterScore = max(0, score); // Make sure it doesn't go negative
}



// Helper method to add sensor data and create alerts
void addSensorDataAndAlert(String sensorType, String status, float value) {
  
  synchronized (alerts) {
    
    String alertMessage = sensorType + ": " + value;
    
    if (sensorType.equals("pH")) {
      phData.add(value);
    } else if (sensorType.equals("Temperature")) {
      temperatureData.add(value);
    } else if (sensorType.equals("Turbidity")) {
      turbidityData.add(value);
    }

    // Determine the alert type (Normal, DANGEROUS, or WARNING)
    String alertType = "SAFE"; // Default alert type
    
    if (status.equals("DANGEROUS")) {
      alertType = "DANGEROUS";
        alerts.add(new Alert(sensorType, alertMessage, getCurrentTime(), alertType));
        pendingAlerts.add(new Alert(sensorType, alertMessage, getCurrentTime(), alertType));
        String currentDate = getCurrentDate();  // This calls your existing getCurrentDate() function
        addAlert(currentDate, alerts.size() - 1);
        println("SensorType: " + sensorType + ", alertMssg: " + alertMessage + ", alertType: " + alertType);
    } else if (status.equals("RISKY")) {
      //score -= 10;
      alertType = "WARNING";
      alerts.add(new Alert(sensorType, alertMessage, getCurrentTime(), alertType));
      pendingAlerts.add(new Alert(sensorType, alertMessage, getCurrentTime(), alertType));
      String currentDate = getCurrentDate();  // This calls your existing getCurrentDate() function
      addAlert(currentDate, alerts.size() - 1);
      println("SensorType: " + sensorType + ", alertMssg: " + alertMessage + ", alertType: " + alertType);
    }

  }
}


String getCurrentTime() {
  // Utility function to get the current time as a string
  return String.format("%02d:%02d:%02d", hour(), minute(), second());
}


void drawDataAlert() {
  int sz = 12;                 // circle diameter
  int px = width - sz - 6;     // 6 px from right edge
  int py =      6;             // 6 px from top
  boolean alive = (millis() - lastSerialTime) < connThreshold;
  noStroke();
  if (alive) {
    fill(0, 200, 0);           // solid green
  } else {
    // flash red/grey at 2 Hz
    if ((millis() / 500) % 2 == 0) fill(200, 0, 0);
    else                             fill(100);
  }
  ellipse(px, py + sz/2, sz, sz);
}
