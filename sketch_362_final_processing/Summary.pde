// Variables for the temperature summary rectangle
float tRectX = 30;                  // X position of the rectangle
float tRectY = 250;                 // Y position of the rectangle
float tRectWidth = 370;             // Width of the rectangle
float tRectHeight = 100;            // Height of the rectangle
float tRectCornerRadius = 10;       // Corner radius for the rectangle

// Variables for the text inside the rectangle
float tTextX = 80;                  // X position of the text
float tTextY = 265;                 // Y position of the text
String tLabel = "Temperature";      // Text content for the label
int tTextSize = 14; 
String display_data="Loading..";
 
 // Variables for the turbidity summary rectangle
float hRectX = 30;                  // X position of the rectangle
float hRectY = 400;                 // Y position of the rectangle
float hRectWidth = 370;             // Width of the rectangle
float hRectHeight = 100;            // Height of the rectangle
float hRectCornerRadius = 10;       // Corner radius for the rectangle

// Variables for the text inside the rectangle
float hTextX = 40;                  // X position of the text
float hTextY = 420;                 // Y position of the text
String hLabel = "Turbidity";      // Text content for the label
int hTextSize = 14; 


// Variables for the pH summary rectangle
float pRectX = 30;                  // X position of the rectangle
float pRectY = 550;                 // Y position of the rectangle
float pRectWidth = 370;             // Width of the rectangle
float pRectHeight = 100;            // Height of the rectangle
float pRectCornerRadius = 10;       // Corner radius for the rectangle

// Variables for the text inside the rectangle
float pTextX = 40;                  // X position of the text
float pTextY = 560;                 // Y position of the text
String pLabel = "pH";               // Text content for the label
int pTextSize = 14; 

int lastAlertTime = 0; 
boolean showAlert = false;

MoreInfoButton tempButton;
MoreInfoButton turbButton;
MoreInfoButton phButton;

void summarySetup(){
 
  tempButton = new MoreInfoButton(tRectX, tRectY + 105, tRectWidth, tRectHeight / 2.7, "View Temperature Data");
  turbButton = new MoreInfoButton(hRectX, hRectY + 105, hRectWidth, hRectHeight / 2.7, "View Turbidity Data");
  phButton = new MoreInfoButton(pRectX, pRectY + 105, pRectWidth, pRectHeight / 2.7,"View PH Data");
 
  
}

void drawSummaryScreen() {
  //background(239, 235, 239);
  background(236);
  // Summary screen content
  fill(0, 48, 73);
  textAlign(CENTER, CENTER);
  textSize(34);
  //text("Welcome", width / 2, height / 10);
  
  textSize(24);
  text("Water Quality Score", width / 2, height / 15);
  
  
  if (currentAlert == null && pendingAlerts.size() > 0) {
    // Only load a new alert if none is currently showing
    currentAlert = pendingAlerts.remove(0);
    currentAlertStartTime = millis();
  }

  if (currentAlert != null) {
    currentAlert.display(width / 2 - (width - 60) / 2, height / 10 * 1.9, width - 60, 80);

    // Wait 3 seconds before clearing the current alert
    if (millis() - currentAlertStartTime > alertDisplayDuration) {
      currentAlert = null;
    }
    textAlign(CENTER, CENTER);
    textSize(34);
    fill(0, 48, 73);
  } 
  else if (pendingAlerts.size() == 0) {
    textSize(24);
    text("No Recent Alerts - looking good!", width / 2, height / 10 * 2.5);
  }
  
  
  fill(0, 150, 0); // Green color for healthy
  if (waterScore <= 70) fill(255, 200, 0); // Yellow if lower
  if (waterScore  <= 40) fill(200, 0, 0);   // Red if very low

  textSize(60);
  text(waterScore  + "%", width / 2, height / 8);
  
  
  // Temperature data display
synchronized (temperatureData) {
  fill(250);
  noStroke();
  rect(tRectX, tRectY, tRectWidth, tRectHeight, tRectCornerRadius);
  fill(0, 48, 73);
  textSize(tTextSize);
  text(tLabel, tTextX, tTextY);
  textSize(54);

  if (temperatureData.size() > 0) {
    float latestTemp = temperatureData.get(temperatureData.size() - 1);
    text("    " +(latestTemp) + " °F", tTextX + 70, tTextY + 45);

    float sum = 0;
    for (Float temp : temperatureData) {
      sum += temp;
    }
    int average = int(sum / temperatureData.size());
    textSize(14);
    text("Average: " + average + " °F", tTextX + 180, tTextY + 70);
  } else {
    text("        Loading...", tTextX + 70, tTextY + 50);
  }
}

  
  tempButton.display();
  

  // Humidity data display
  synchronized (turbidityData) {
    fill(250);
    noStroke();
    rect(hRectX, hRectY, hRectWidth, hRectHeight, hRectCornerRadius);
    fill(0, 48, 73);
    textSize(hTextSize);
    text(hLabel, hTextX, hTextY);
    textSize(54);

    if (turbidityData.size() > 0) {
      float latestHumidity = turbidityData.get(turbidityData.size() - 1);
      text(latestHumidity + " V", hTextX + 50, hTextY + 50);

      float sum = 0;
      for (Float turbidity : turbidityData) {
        sum += turbidity;
      }
      int average = int(sum / turbidityData.size());
      textSize(14);
      text("Average: " + average + "V", hTextX + 180, hTextY + 80);
    } else {
      text("Loading...", hTextX + 50, hTextY + 50);
    }
  }
  
  
  
  turbButton.display();
  
  
    // pH data display
  synchronized (phData) {
    fill(250);
    noStroke();
    rect(pRectX, pRectY, pRectWidth, pRectHeight, pRectCornerRadius);
    fill(0, 48, 73);
    textSize(pTextSize);
    text(pLabel, pTextX, pTextY);
    textSize(54);
  
    if (phData.size() > 0) {
      float latestPh = phData.get(phData.size() - 1);
      text(latestPh + " pH", pTextX + 50, pTextY + 50);
  
      float sum = 0;
      for (Float ph : phData) {
        sum += ph;
      }
      int average = int(sum / phData.size());
      textSize(14);
      text("Average: " + average + " pH", pTextX + 180, pTextY + 90);
    } else {
      text("Loading...", pTextX + 50, pTextY + 50); 
    }
  }
  phButton.display();
 
  
}




void displayRecentAlert() {
  ArrayList<Alert> alertsCopy;
  synchronized (alerts) {
    alertsCopy = new ArrayList<>(alerts); // Create a copy
  }

  if (alertsCopy.size() > 0) {
    alertsCopy.get(alertsCopy.size() - 1).display(width / 2 - (width - 60) / 2, height / 10 * 1.9, width - 60, 80);
  }
}
