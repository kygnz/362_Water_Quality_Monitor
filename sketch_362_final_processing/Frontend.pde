
PFont font;

void frontendSetup(){
  font = createFont("Trebuchet MS", 32);
  textFont(font);
  //background(239, 235, 239);
}


void mousePressed() {
  if(appState == 0){ 
    // Check if "View Temperature Data" button is clicked
    if (tempButton.isClicked(mouseX, mouseY)){
      appState = 3;
    }
    // Check if "View turb Data" button is clicked
    if (turbButton.isClicked(mouseX, mouseY)){
      appState = 4;
    }  
    // Check if "View ph Data" button is clicked
    if (phButton.isClicked(mouseX, mouseY)){
      appState = 5;
    } 
    
  }
  //// Check if "Summary" button is clicked
  if (summaryButton.isClicked(mouseX, mouseY)) {
    appState = 0;
  }
  // Check if "Alerts" button is clicked
  if (alertsButton.isClicked(mouseX, mouseY)) {
    appState = 1;
  }
  // Check if "History" button is clicked
  if (historyButton.isClicked(mouseX, mouseY)) {
    appState = 2;
  }
  
  if(appState == 2){
    
    if (showAlertDetails) {
      // Back button functionality
      if (mouseX > 10 && mouseX < 110 && mouseY > 10 && mouseY < 50) {
        showAlertDetails = false;
      }
    } else {
      // Calendar navigation
      if (mouseX > calendarX - 40 && mouseX < calendarX - 10 &&
          mouseY > calendarY + calendarHeight / 2 - 20 && mouseY < calendarY + calendarHeight / 2 + 20) {
        currentMonthIndex = max(0, currentMonthIndex - 1); // Navigate left
      } else if (mouseX > calendarX + calendarWidth + 10 && mouseX < calendarX + calendarWidth + 40 &&
                 mouseY > calendarY + calendarHeight / 2 - 20 && mouseY < calendarY + calendarHeight / 2 + 20) {
        currentMonthIndex = min(months.length - 1, currentMonthIndex + 1); // Navigate right
      }
  
      // Check if a red circle (alert day) is clicked
      int startX = calendarX + 30;
      int startY = calendarY + 120;
      int x = startX, y = startY;
  
      for (int day = 1; day <= daysInMonth[currentMonthIndex]; day++) {
        String key = months[currentMonthIndex] + "-" + day;
        if (alertsCalendar.containsKey(key) && mouseX > x - circleSize / 2 && mouseX < x + circleSize / 2 &&
            mouseY > y - circleSize / 2 && mouseY < y + circleSize / 2) {
          selectedDate = key;
          showAlertDetails = true;
          break;
        }
        x += circleSize + spacing;
        if ((day + 1) % 7 == 1) {
          x = startX;
          y += circleSize + spacing;
        }
      }
    }
  }
  
  
  

  
  
    
  
  
}
