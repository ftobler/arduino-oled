#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);




int accX = A3;
int accY = A6;
int accZ = A7;

int maxIter = 100;
float maxQuadrat = 4;

float mcx = -1.23679281079116;
float mcy = -0.159552232089318;  
float zoom = 0.96;//0.03;


void setup()   {  

 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c); //initialize I2C addr 0x3c
  display.display();
  
  pinMode(accX, INPUT);
  pinMode(accY, INPUT);
  pinMode(accZ, INPUT);
  
  delay(100);  
  display.clearDisplay();  
}

void loop() { 
     
  float cy = mcy - (zoom * 32);  
  float cx;
  for (int py = 0; py < 64; py++) {
    cx = mcx - (zoom * 63);
    for (int px = 0; px < 128; px++) {
      int iter = 0;
      float quadrat = 0;
      float x = 0;
      float y = 0;
      float tx = 0;
      float ty = 0;
      while ((quadrat <= maxQuadrat) & (iter < maxIter)) {
        tx = (x * x) - (y * y) + cx;
        ty = (2 * x * y) + cy;
        x = tx;
        y = ty;
        iter++;
        quadrat = (x * x) + (y * y);
      }
      if (iter >= maxIter) {
        iter = 100;
      } else {
        iter = iter % 4;  
      }
      if ((iter == 0) | (iter == 100)) {
        display.drawPixel(px, py, BLACK);
      }
      if (iter == 2) {
        display.drawPixel(px, py, WHITE);
      }
      if ((iter == 1) | (iter == 3)) {
        display.drawPixel(px, py, ((px % 2) == 0) ^ ((py % 2) == 0));
      }
      
      cx = cx + zoom;
    }
    
    cy = cy + zoom;
    display.display();  
  }
  
  
  zoom = zoom * 0.5;
  

    

    
  
}









