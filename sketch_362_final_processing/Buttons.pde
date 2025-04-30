
// Navigation Button Class
class MainButton {
  float x, y, w, h;
  String label;
  color bgColor, textColor;
  boolean isPressed;

  MainButton(float x, float y, float w, float h, String label, boolean isPressed) {
    this.x = x;
    this.y = y;
    this.w = w;
    this.h = h;
    this.label = label;
    this.bgColor = color(200); // bg
    this.textColor = color(0);
    this.isPressed = isPressed;
  }

  color getColor(){
    if(isPressed){
      return color(250);
    }
    return color(27, 73, 101);
  }
  
  color getTextFill(){
    if(isPressed){
      return color(0, 48, 73);
      //return color(69, 70, 69);
    }
    return color(250);
  }
  
  void display() {
    fill(getColor());
    noStroke();
    rect(x, y, w, h);
    fill(getTextFill());
    textAlign(CENTER, CENTER);
    textSize(16);
    text(label, x + w / 2, y + h / 2);
  }
  

  boolean isClicked(float mouseX, float mouseY) {
    return mouseX > x && mouseX < x + w && mouseY > y && mouseY < y + h;
  }
}



// Summary Window Button Class
class MoreInfoButton {
  float x, y, w, h;
  String label;
  color bgColor, textColor;

  MoreInfoButton(float x, float y, float w, float h, String label) {
    this.x = x;
    this.y = y;
    this.w = w;
    this.h = h;
    this.label = label;
    this.bgColor = color(95, 168, 211);  // Default background color
    this.textColor = color(255);// Default text color
  }

  
  void display() {
    fill(bgColor);
    noStroke();
    rect(x, y, w, h, 10);
    fill(textColor);
    textAlign(LEFT);
    textSize(14);
    float padding = 10;
    text(label, x + padding, y + h / 2);
  }
  

  boolean isClicked(float mouseX, float mouseY) {
    return mouseX > x && mouseX < x + w && mouseY > y && mouseY < y + h;
  }
}
