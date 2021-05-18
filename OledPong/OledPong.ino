#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);


int deltaX;
int deltaY;
int posX;
int posY;

int Player1 = 0;
int Player2 = 0;

#define playersize 20

int accX = A3;
int accY = A6;
int accZ = A7;



void setup() {
  Serial.begin(9600);
    
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c); //initialize I2C addr 0x3c
  display.clearDisplay();
  display.display();
  
  pinMode(accX, INPUT);
  pinMode(accY, INPUT);
  pinMode(accZ, INPUT);
  
  randomSeed(analogRead(accX));
  initBall();
  displayLogoAnim(100);
}

void initBall() {
  deltaX = 40;
  deltaY = 0;
  posX = 640;
  posY = 320;
}

void loop() {
  display.clearDisplay();  
  setPlayer1();
  setPlayer2();
  posX = posX - deltaX;
  posY = posY - deltaY;
  checkCollusion();
  display.display();
}

void setPlayer1() {
  Player1 = map(analogRead(accX), 300+30, 435-30, 0, 64 - playersize);
  if (Player1 < 0) {
    Player1 = 0;  
  }
  if (Player1 > 64 - playersize) {
    Player1 = 64 - playersize;  
  }
  display.drawLine(0, Player1, 0, Player1 + playersize, WHITE);
}

void setPlayer2() {
  if ((posY / 10) < Player2 + (playersize / 2) - 3) {
    Player2 = Player2 -3;
  } 
  if ((posY / 10) > Player2 + (playersize / 2) + 3) {
    Player2 = Player2 +3;
  }
  if (Player2 < 0) {
    Player2 = 0;  
  }
  if (Player2 > 64 - playersize) {
    Player2 = 64 - playersize;  
  }
  display.drawLine(127, Player2, 127, Player2 + playersize, WHITE);
}

void checkCollusion() {
  bool crash = true;
  if (((posY / 10) >= Player2 - 2) & ((posY / 10) <= Player2 + playersize + 2) & (deltaX < 0) & (posX > 1270))  {
    deltaX = 0 - deltaX;
    deltaY = 0 + (Player2 + (playersize / 2) - (posY / 10)) * 10 / 4;
    crash = false;
  }
  if (((posY / 10) >= Player1 - 2) & ((posY / 10) <= Player1 + playersize + 2) & (deltaX > 0) & (posX < 0))  {
    deltaX = 0 - deltaX;
    deltaY = 0 + (Player1 + (playersize / 2) - (posY / 10)) * 10 / 4;
    crash = false;
  }
  if (crash & ((posX > 1270) | (posX < 0))) {
    explosion(posX / 10, posY / 10);
    initBall();
    displayLogoAnim(20);
  }
  if ((posY <= 0) | (posY >= 630)) {
    deltaY = 0 - deltaY;
  }
  display.fillRect((posX / 10) - 1, (posY / 10) - 1, 3, 3, WHITE);
}



void explosion(int x, int y) {
  for (int i = 0; i < 20; i++) {
    int r1 = random(30);
    int r2 = random(30);
    float rad = random(6283);
    rad = rad / 1000;
    display.drawLine(sin(rad) * r1 + x, cos(rad) * r1 + y, sin(rad) * r2 + x, cos(rad) * r2 + y, WHITE);    
    display.display();
  }
}




void displayLogoAnim(int loops) {
  for (int i = 0; i < loops; i++) {
    setAlign(64, 32, map(analogRead(accX), 300, 435, 70, -70));
    displayLogo();
  }
}
void displayLogo() {
  display.clearDisplay();
  //P
  moveTo(0,0);
  lineTo(11,0);
  lineTo(11,9);
  lineTo(3,9);
  lineTo(3,13);
  lineTo(0,13);
  lineTo(0,0);
  
  moveTo(3,1);
  lineTo(8,1);
  lineTo(8,8);
  lineTo(3,8);
  lineTo(3,1);
  
  //O
  moveTo(14,0);
  lineTo(25,0);
  lineTo(25,13);
  lineTo(14,13);
  lineTo(14,0);
  
  moveTo(17,1);
  lineTo(22,1);
  lineTo(22,12);
  lineTo(17,12);
  lineTo(17,1);
  
  //N
  moveTo(28,0);
  lineTo(31,0);
  lineTo(36,5);
  lineTo(36,0);
  lineTo(39,0);
  lineTo(39,13);
  lineTo(36,13);
  lineTo(36,7);
  lineTo(31,2);
  lineTo(31,13);
  lineTo(28,13);
  lineTo(28,0);
  
  //G
  moveTo(42,0);
  lineTo(53,0);
  lineTo(53,3);
  lineTo(50,3);
  lineTo(50,1);
  lineTo(45,1);
  lineTo(45,12);
  lineTo(50,12);
  lineTo(50,7);
  lineTo(48,7);
  lineTo(48,6);
  lineTo(53,6);
  lineTo(53,13);
  lineTo(42,13);
  lineTo(42,0);
  
  display.display();
}

#define logoXOffset -54
#define logoYOffset -13
#define logoMulti 2
int moveXCoordinate;
int moveYCoordinate;
int centerDeg;
int centerXCoordinate;
int centerYCoordinate;


void setAlign(int x, int y, int deg) {
  centerDeg = deg;
  centerXCoordinate = x;
  centerYCoordinate = y;
}
void moveTo(int x, int y) {
  moveXCoordinate = x;
  moveYCoordinate = y;
}
void lineTo(int x, int y) {
  int x1 = moveXCoordinate * logoMulti + logoXOffset;
  int y1 = moveYCoordinate * logoMulti + logoYOffset;
  int x2 = x * logoMulti + logoXOffset;
  int y2 = y * logoMulti + logoYOffset;
  display.drawLine(round((x1 * cos(centerDeg * 0.017453278)) - (y1 * sin(centerDeg * 0.017453278))) + centerXCoordinate,
                   round((x1 * sin(centerDeg * 0.017453278)) + (y1 * cos(centerDeg * 0.017453278))) + centerYCoordinate,
                   round((x2 * cos(centerDeg * 0.017453278)) - (y2 * sin(centerDeg * 0.017453278))) + centerXCoordinate,
                   round((x2 * sin(centerDeg * 0.017453278)) + (y2 * cos(centerDeg * 0.017453278))) + centerYCoordinate, WHITE);
  moveTo(x, y);
}


