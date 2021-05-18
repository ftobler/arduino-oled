#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Oled3DDriver.h"

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);




int count1 = 0;
int count2 = 0;
int count3 = 0;
int shape = 0;

int accX = A3;
int accY = A6;
int accZ = A7;



float zoom = 1;
float angle = 1.02;

GPoint rotate;
GPoint linear;

byte graph[48];
int graphPosition = 0;




void setup()   {  

 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c); //initialize I2C addr 0x3c
  display.display();
  
  pinMode(accX, INPUT);
  pinMode(accY, INPUT);
  pinMode(accZ, INPUT);
  
  delay(100);
  
 
  
    
}

void loop() { 
  
  count1 = (count1 + 6) % 3600;
  count2 = (count2 + 100) % 3600;
  count3 = (count3 + 2) % 3600;
  if (count1 == 0) {
    shape = (shape + 1) % 8;
  }
  
  graphPosition = (graphPosition + 1) % 16;
  graph[(graphPosition * 3) + 0] = (analogRead(accX) - 280) / 9;
  graph[(graphPosition * 3) + 1] = (analogRead(accY) - 280) / 9;
  graph[(graphPosition * 3) + 2] = (analogRead(accZ) - 280) / 9;
  
  
  display.clearDisplay();

  
  transformReset();
  transformLinear(0, 0, 0);
  transformRotate(count1 / 10, count2 / 10, count3 / 10);
  wireQube(GraphicPoint(15,15,15), GraphicPoint(-15,-15,-15));
 // transformReset();
 // transformLinear(-30, 12, 0);
 // transformRotate(-count1 / 10, -count2 / 10, -count3 / 10);
 // wireQube(GraphicPoint(9,9,9), GraphicPoint(-9,-9,-9));


  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("X:" + String(analogRead(accX)));
  display.println("Y:" + String(analogRead(accY)));
  display.println("Z:" + String(analogRead(accZ)));
  
  
  int j = graphPosition;
  for (int i = 0; i < 16; i++) {
    j = (j + 1) % 16;
    display.drawPixel(111 + i, graph[(j * 3) + 0], WHITE);
    display.drawPixel(111 + i, graph[(j * 3) + 1] + 21, WHITE);
    display.drawPixel(111 + i, graph[(j * 3) + 2] + 42, WHITE);
  }
  
  display.drawLine(106, 0, 106, 20, WHITE);
  display.drawLine(106, 0, 109, 0, WHITE);
  display.drawLine(106, 20, 109, 20, WHITE);
  
  display.drawLine(106, 22, 106, 40, WHITE);
  display.drawLine(106, 22, 109, 22, WHITE);
  display.drawLine(106, 40, 109, 40, WHITE);
  
  display.drawLine(106, 42, 106, 60, WHITE);
  display.drawLine(106, 42, 109, 42, WHITE);
  display.drawLine(106, 60, 109, 60, WHITE);
  
  //for (int i = 0; i < 10; i++) {
  //  display.drawLine(random(127), random(63), random(127), random(63), WHITE);
  //}
    
  display.display();
  
  /*display.clearDisplay(); // clears the screen and buffer

  display.drawPixel(127, 63, WHITE);
  display.drawPixel(90, 15, WHITE);
  display.drawRoundRect(40, 12, 30, 7, 3, WHITE);
  display.drawRect(40, 21, 30, 5,WHITE);
  display.drawLine(0, 63, 127, 21, WHITE);
  display.drawCircle(110, 50, 12, WHITE);
  //display.fillCircle(45, 50, 8, WHITE);
  display.drawTriangle(70, 60, 90, 60, 80, 46, WHITE);
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Hello World, this is Florin");
  display.println("I2C Oled 0.96 exemple");
  display.setTextSize(2);
  display.println("I2C");
  display.setTextColor(BLACK, WHITE);
  display.setTextSize(1);
  display.println(count);
  display.setTextColor(WHITE, BLACK);
  display.display();*/
}


void transformLinear(float x, float y, float z) {
  linear.x = linear.x + x;
  linear.y = linear.y + y;
  linear.z = linear.z + z;
}

void transformRotate(float gradX, float gradY, float gradZ) {
  rotate.x = rotate.x + (gradX / 57.2957795);
  rotate.y = rotate.y + (gradY / 57.2957795);
  rotate.z = rotate.z + (gradZ / 57.2957795);
}

void transformReset() {
  linear.x = 0;
  linear.y = 0;
  linear.z = 0;
  rotate.x = 0;
  rotate.y = 0;
  rotate.z = 0;
}

GPoint transformPoint(GPoint ap) {
  GPoint bp;
  //Z rotate
  bp.x = (ap.x * cos(rotate.z)) - (ap.y * sin(rotate.z));
  bp.y = (ap.x * sin(rotate.z)) + (ap.y * cos(rotate.z));
  bp.z = ap.z;
  ap = bp;

  //X rotate
  bp.z = (ap.z * cos(rotate.x)) - (ap.y * sin(rotate.x));
  bp.y = (ap.z * sin(rotate.x)) + (ap.y * cos(rotate.x));
  bp.x = ap.x;
  ap = bp;

  //Y rotate
  bp.x = (ap.x * cos(rotate.y)) - (ap.z * sin(rotate.y));
  bp.z = (ap.x * sin(rotate.y)) + (ap.z * cos(rotate.y));
  bp.y = ap.y;


  //Move linear
  bp.x = bp.x + linear.x;
  bp.y = bp.y + linear.y;
  bp.z = bp.z + linear.z;
  
  return bp;
}

void line(GPoint p1, GPoint p2) {
  p1 = transformPoint(p1);
  p2 = transformPoint(p2);
  display.drawLine(
    round((p1.x / pow(angle, p1.z)) * zoom) + 63, round((p1.y / pow(angle, p1.z)) * zoom) + 31,
    round((p2.x / pow(angle, p2.z)) * zoom) + 63, round((p2.y / pow(angle, p2.z)) * zoom) + 31, 
    WHITE);
}

void wireQube(GPoint p1, GPoint p2) {
  line(GraphicPoint(p1.x, p1.y, p1.z), GraphicPoint(p1.x, p1.y, p2.z));
  line(GraphicPoint(p1.x, p1.y, p1.z), GraphicPoint(p1.x, p2.y, p1.z));
  line(GraphicPoint(p1.x, p2.y, p2.z), GraphicPoint(p1.x, p2.y, p1.z));
  line(GraphicPoint(p1.x, p2.y, p2.z), GraphicPoint(p1.x, p1.y, p2.z));

  line(GraphicPoint(p1.x, p1.y, p1.z), GraphicPoint(p2.x, p1.y, p1.z));
  line(GraphicPoint(p1.x, p1.y, p2.z), GraphicPoint(p2.x, p1.y, p2.z));
  line(GraphicPoint(p1.x, p2.y, p1.z), GraphicPoint(p2.x, p2.y, p1.z));
  line(GraphicPoint(p1.x, p2.y, p2.z), GraphicPoint(p2.x, p2.y, p2.z));

  line(GraphicPoint(p2.x, p1.y, p1.z), GraphicPoint(p2.x, p1.y, p2.z));
  line(GraphicPoint(p2.x, p1.y, p1.z), GraphicPoint(p2.x, p2.y, p1.z));
  line(GraphicPoint(p2.x, p2.y, p2.z), GraphicPoint(p2.x, p2.y, p1.z));
  line(GraphicPoint(p2.x, p2.y, p2.z), GraphicPoint(p2.x, p1.y, p2.z));
}

GPoint GraphicPoint(float x, float y, float z) {
  GPoint temp;
  temp.x = x;
  temp.y = y;
  temp.z = z;
  return temp;
}

void triangle(GPoint p1, GPoint p2, GPoint p3) {
  line(p1, p2);
  line(p2, p3);
  line(p3, p1);
}








