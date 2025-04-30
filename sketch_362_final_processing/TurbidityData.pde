int turbidityMax = 5; 
int turbidityMin = 0; 

void drawTurbidityScreen() {
  background(236);
  fill(0, 48, 73); 
  textAlign(CENTER, CENTER);
  textSize(14);
  text("Current Turbidity:", width / 2, height / 10);
  textSize(54);

  if (turbidityData.size() > 0) {
    text((turbidityData.get(turbidityData.size() - 1)) + " V", width / 2, height / 10 + 55);  

    // Average turbidity:
    textSize(14);
    float sum = 0.0f;
    for (Float turb : turbidityData) {
        sum += turb;
    }

    int average = int(sum / turbidityData.size());
    text("Average: " + average + " V", width / 2, height / 10 + 95);
  }

  
  drawTurbidityGraph(turbidityData);
}

void drawTurbidityGraph(ArrayList<Float> turbidityData) {
  int yBase = 270;        // Top margin for the graph
  int graphHeight = (windowHeight - (margin * 5)) / 4;  // Height of the graph
  int yMax = turbidityMax; 
  int yMin = turbidityMin; 

  // background
  fill(255);
  noStroke();
  rect(margin - 10, yBase - 10, windowWidth - margin * 2 + 40, graphHeight + 40, 20);

  // title
  fill(0, 48, 73);
  textAlign(CENTER, CENTER);
  textSize(18);
  text("Turbidity Data", windowWidth / 2, yBase - 40);

//frame
  stroke(0);
  strokeWeight(1);
  line(margin, yBase, margin, yBase + graphHeight);  // Y-axis
  line(margin, yBase + graphHeight, windowWidth - margin, yBase + graphHeight);  // X-axis

  // Y-axis label
  fill(0, 48, 73);
  textSize(14);
  pushMatrix();
  translate(margin - 40, yBase + graphHeight / 2);
  rotate(-HALF_PI);
  text("Turbidity (V)", 0, 0);
  popMatrix();

  // X-axis label
  textAlign(CENTER, CENTER);
  text("Time (s)", windowWidth / 2, yBase + graphHeight + 40);

  // Y-axis labels
  textAlign(RIGHT, CENTER);
  textSize(12);
  text(nf(yMax, 0, 1) + " V", margin - 10, yBase);
  text(nf(yMin, 0, 1) + " V", margin - 10, yBase + graphHeight);

  // X-axis labels
  textAlign(CENTER, TOP);
  int elapsedTime = (millis() - startTime) / 1000;  // Convert to seconds
  text(elapsedTime + " sec.", windowWidth - margin, yBase + graphHeight + 10);

  // Plot 
  if (turbidityData != null && turbidityData.size() > 1) {
    stroke(graphColor);
    strokeWeight(2);
    noFill();
    beginShape();

    for (int i = 0; i < turbidityData.size(); i++) {
      float x = map(i, 0, turbidityData.size() - 1, margin, windowWidth - margin);
      float y = map(turbidityData.get(i), yMin, yMax, yBase + graphHeight, yBase);
      vertex(x, y);
    }

    endShape();
  } else {
    fill(graphColor);
    noStroke();
    textAlign(CENTER, CENTER);
    text("No Data Available", windowWidth / 2, yBase + graphHeight / 2);
  }
}
