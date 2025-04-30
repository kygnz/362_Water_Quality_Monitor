
float phMax = 14.0;
float phMin = 0.0;

void drawPhScreen() {
  background(236);

  // Current pH
  fill(0, 48, 73);
  textAlign(CENTER, CENTER);
  textSize(14);
  text("Current pH:", width/2, height/10);
  textSize(54);
  if (phData.size() > 0) {
    float latest = phData.get(phData.size() - 1);
    text(nf(latest, 0, 2) + " pH", width/2, height/10 + 55);

    // Average pH
    float sum = 0;
    for (float v : phData) sum += v;
    float avg = sum / phData.size();
    textSize(14);
    text("Average: " + nf(avg, 0, 2)+ " pH", width/2, height/10 + 95);
  }

  drawPhGraph(phData);
}

void drawPhGraph(ArrayList<Float> data) {
  int yBase      = 270;
  int graphH     = (windowHeight - margin*5)/4;
  float yMax     = phMax;
  float yMin     = phMin;

  // background
  fill(255);
  noStroke();
  rect(margin-10, yBase-10, windowWidth - margin*2 + 40, graphH + 40, 20);

  // title
  fill(0, 48, 73);
  textAlign(CENTER, CENTER);
  textSize(18);
  text("pH Data", windowWidth/2, yBase - 40);

  // axes
  stroke(0);
  line(margin, yBase, margin, yBase + graphH);
  line(margin, yBase + graphH, windowWidth - margin, yBase + graphH);

  // y-axis label
  fill(0, 48, 73);
  textSize(14);
  pushMatrix();
    translate(margin - 40, yBase + graphH/2);
    rotate(-HALF_PI);
    text("pH", 0, 0);
  popMatrix();

  // x-axis label
  textAlign(CENTER, CENTER);
  textSize(14);
  text("Time (s)", windowWidth/2, yBase + graphH + 40);

  // y-axis ticks
  textAlign(RIGHT, CENTER);
  textSize(12);
  text(nf(yMax, 0, 1), margin - 10, yBase);
  text(nf(yMin, 0, 1), margin - 10, yBase + graphH);

  // x-axis tick
  textAlign(CENTER, TOP);
  int elapsed = (millis() - startTime)/1000;
  text(elapsed + " sec.", windowWidth - margin, yBase + graphH + 10);

  // plot line
  if (data.size() > 1) {
    stroke(graphColor);
    strokeWeight(2);
    noFill();
    beginShape();
      for (int i = 0; i < data.size(); i++) {
        float x = map(i, 0, data.size()-1, margin, windowWidth - margin);
        float y = map(data.get(i), yMin, yMax, yBase + graphH, yBase);
        vertex(x, y);
      }
    endShape();
  } else {
    // placeholder
    fill(graphColor);
    noStroke();
    textAlign(CENTER, CENTER);
    text("No Data Available", windowWidth/2, yBase + graphH/2);
  }
}
