
import java.util.Iterator;

ArrayList<Alert> alerts = new ArrayList<Alert>();
float scrollOffset = 0; 
float itemHeight = 90;  
float totalHeight;      


void mouseWheel(MouseEvent event) {
  float e = event.getCount();
  scrollOffset += e * 20; 
}

// Displays the alerts in a scrollable list view
void displayAlertsScreen(ArrayList<Alert> alerts) {
  background(236);
  fill(0, 48, 73);
  textAlign(CENTER, CENTER);
  textSize(34);
  text("Alerts", width / 2, height / 10);
  
  ArrayList<Alert> alertsCopy;
  synchronized (alerts) { 
    alertsCopy = new ArrayList<Alert>(alerts); 
  }
  
  totalHeight = alerts.size() * itemHeight; 

  if (alertsCopy.size() == 0) {
    textAlign(CENTER, CENTER);
    fill(0, 48, 73);
    textSize(24);
    text("No alerts to display", width / 2, height / 2);
  } else {
    pushMatrix();
    translate(0, -scrollOffset);

    float y = 130;
    for (Alert alert : alertsCopy) {  
      alert.display(30, y, width - 60, 80);
      y += itemHeight;
    }

    popMatrix();
    if (totalHeight > windowHeight) {
      drawScrollbar();
    }
  }
}



// Draws the scrollbar for the alerts screen
void drawScrollbar() {
  float scrollbarHeight = map(windowHeight, 0, totalHeight, 0, height);
  float scrollbarY = map(scrollOffset, 0, totalHeight - windowHeight, 0, height - scrollbarHeight);

  fill(255);
  rect(width - 10, 0, 10, height); 
  fill(240);
  rect(width - 10, scrollbarY, 10, scrollbarHeight);
}



class Alert {
  String sensorType;
  String message;
  String time;
  String alertType;

  Alert(String sensorType, String message, String time, String alertType) {
    this.sensorType = sensorType;
    this.message = message;
    this.time = time;
    this.alertType = alertType;
  }

  // displays an alert styled as a panel
  void display(float x, float y, float w, float h) {
    fill(getColor());  
    rect(x, y, w, h, 10);  
    fill(255);  
    textAlign(LEFT, CENTER);
    textSize(14);
    
    text(sensorType + ": " + message + "\n" + "Time: " + time, x + 10, y + h / 2);
  }

  // color based on the type of the alert
  color getColor() {
    if (this.alertType.equals("DANGEROUS")) {
      return color(219, 58, 52);  
    } else if (this.alertType.equals("WARNING")) {
      return color(255, 200, 87);  
    } 
    else {
      return color(100);
    }
  }
}
