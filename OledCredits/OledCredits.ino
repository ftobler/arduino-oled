#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);


#define linecount 8
String lines[linecount] =  {
"Arduino &",
"OLED",
"Display",
"",
"",
"",
"",
""};

int index = 64;


void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  display.clearDisplay();
  display.display();
}

void loop() {
  display.clearDisplay();
  display.setCursor(0,index);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  for (int i = 0; i < linecount; i++) {
    display.println(lines[i]);
  }
  index--;
  display.display();
}
