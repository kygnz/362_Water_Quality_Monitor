



// Calendar variables
int calendarX = 65;        
int calendarY = 150;        
int calendarWidth = 300;   
int calendarHeight = 350;  
int circleSize = 30;       
int spacing = 10;          
color bgColor = color(250); 
int totalHeightC;
float scrollOffsetC = 0;
float itemHeightC = 100;

// Sample data for alerts
//HashMap<String, Boolean> alertsCalendar = new HashMap<String, Boolean>();
HashMap<String, ArrayList<Integer>> alertsCalendar = new HashMap<String, ArrayList<Integer>>();

String[] months = {"January", "February", "March", "April"};
int[] daysInMonth = {30, 31, 30, 31};
int currentMonthIndex = 3; 

// Today's date
int todayMonthIndex = 3;
int todayDate = 28;
boolean showAlertDetails = false; 
String selectedDate = "";

void drawHistoryScreen() {
  background(236);
  // Header
  fill(50);
  textAlign(CENTER, CENTER);
  textSize(34);
  text("Water Quality History", width / 2, height / 10);
  drawCalendar();
  drawMonthNavigation();
}


void drawCalendar() {
  // background
  fill(bgColor);
  noStroke();
  rect(calendarX, calendarY, calendarWidth, calendarHeight, 20);

  // month title
  fill(0);
  textAlign(CENTER, CENTER);
  textSize(20);
  text(months[currentMonthIndex], calendarX + calendarWidth / 2, calendarY + 40);

  // week
  String[] daysOfWeek = {"SU", "M", "T", "W", "TH", "F", "SA"};
  textSize(14);
  for (int i = 0; i < daysOfWeek.length; i++) {
    fill(0);
    text(daysOfWeek[i], calendarX + 30 + i * (circleSize + spacing), calendarY + 80);
  }

  // days of the month
  int startX = calendarX + 30;
  int startY = calendarY + 120;
  int x = startX, y = startY;

  for (int day = 1; day <= daysInMonth[currentMonthIndex]; day++) {
    String key = months[currentMonthIndex] + "-" + day;

    if (currentMonthIndex > todayMonthIndex || (currentMonthIndex == todayMonthIndex && day > todayDate)) {
      fill(200); 
    //}// else if (currentMonthIndex == todayMonthIndex && day == todayDate) {
    //  fill(42, 157, 143); 
    } else if (alertsCalendar.containsKey(key)) {
      fill(231, 111, 81); 
    } else {
      fill(134, 203, 146);
    }

    ellipse(x, y, circleSize, circleSize);
    fill(0);
    textAlign(CENTER, CENTER);
    textSize(12);
    text(day, x, y);

    x += circleSize + spacing;
    if ((day + 1) % 7 == 1) { 
      x = startX;
      y += circleSize + spacing;
    }
  }
}


void drawMonthNavigation() {
  // Left button
  fill(200);
  rect(calendarX - 40, calendarY + calendarHeight / 2 - 20, 30, 40, 10);
  fill(0);
  textAlign(CENTER, CENTER);
  textSize(16);
  text("<", calendarX - 25, calendarY + calendarHeight / 2);

  // Right button
  fill(200);
  rect(calendarX + calendarWidth + 10, calendarY + calendarHeight / 2 - 20, 30, 40, 10);
  fill(0);
  textAlign(CENTER, CENTER);
  textSize(16);
  text(">", calendarX + calendarWidth + 25, calendarY + calendarHeight / 2);
}

void drawAlertDetails() {
  background(236);

  // Back button
  fill(200);
  rect(10, 10, 100, 40, 10);
  fill(0);
  textAlign(CENTER, CENTER);
  textSize(16);
  text("Back", 60, 30);

  // Header
  fill(0, 48, 73);
  textAlign(CENTER, CENTER);
  textSize(34);
  text("Alerts on " + selectedDate, width / 2, height / 10);

  // Display alerts with scrolling
  ArrayList<Integer> alertIndices = alertsCalendar.getOrDefault(selectedDate, new ArrayList<Integer>());
   totalHeight = int(alertIndices.size() * itemHeight); // Calculate total height

  
    pushMatrix();
    translate(0, -scrollOffset);

    float y = 130; 
    for (int index : alertIndices) {
      alerts.get(index).display(30, y, width - 60, 80); 
      y += itemHeight;
    }

    popMatrix();
      drawScrollbarC(int(alertIndices.size() * itemHeight));

}


// Draws the scrollbar for the alerts screen
void drawScrollbarC(int totalHeightC) {
  float scrollbarHeight = map(windowHeight, 0, totalHeightC, 0, height);
  float scrollbarY = map(scrollOffset, 0, totalHeightC - windowHeight, 0, height - scrollbarHeight);

  fill(255);
  rect(width - 10, 0, 10, height); 
  fill(240);
  rect(width - 10, scrollbarY, 10, scrollbarHeight);
}


void addAlert(String date, int alertIndex) {
  if (!alertsCalendar.containsKey(date)) {
    alertsCalendar.put(date, new ArrayList<Integer>());
  }
  alertsCalendar.get(date).add(alertIndex);
}

String getCurrentDate() {
  int day = day();     
  //int month = 3;  
  String mName = months[todayMonthIndex]; 
  return mName + "-" + day;       
}
