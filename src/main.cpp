#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>

// OLED config
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

#define SDA_PIN 20
#define SCL_PIN 21

TwoWire myWire(i2c0, SDA_PIN, SCL_PIN);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &myWire);

static const unsigned char PROGMEM DVD_logo[] = {0x3f,0x8f,0xe0,0x07,0xd8,0x30,0x63,0xde,0x30,0x66,0xf6,0x60,0x7c,0x67,0xc0,0x00,0x40,0x00,0x0f,0xfe,0x00,0xfe,0x1f,0xe0,0x3f,0xff,0x80};
const int dvdLogoWidth = 20;
const int dvdLogoHeight = 9;

void setup() {
  Serial.begin(115200);
  myWire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}

// Initial position and speed of the DVD logo (randomized)
int dvdX = random(0, SCREEN_WIDTH - dvdLogoWidth);
int dvdY = random(0, SCREEN_HEIGHT - dvdLogoHeight);
int dx = random(1, 3) * (random(0, 2) * 2 - 1);
int dy = random(1, 3) * (random(0, 2) * 2 - 1);
bool blink = false;
unsigned long blinkUntil = 0;

void loop() {
  unsigned long now = millis();

  display.clearDisplay();

  // Check for corner collision
  bool atCorner = 
    (dvdX <= 0 && dvdY <= 0) ||
    (dvdX <= 0 && dvdY >= SCREEN_HEIGHT - dvdLogoHeight) ||
    (dvdX >= SCREEN_WIDTH - dvdLogoWidth && dvdY <= 0) ||
    (dvdX >= SCREEN_WIDTH - dvdLogoWidth && dvdY >= SCREEN_HEIGHT - dvdLogoHeight);

  if (atCorner) {
    blink = true;
    blinkUntil = now + 1000; // Blink for 1000ms
  }

  // Draw logo unless blinking
  if (!blink || (blink && (now / 100) % 2 == 0)) {
    display.drawBitmap(dvdX, dvdY, DVD_logo, dvdLogoWidth, dvdLogoHeight, SSD1306_WHITE);
  }

  dvdX += dx;
  dvdY += dy;
  if (dvdX <= 0 || dvdX >= SCREEN_WIDTH - dvdLogoWidth) dx = -dx;
  if (dvdY <= 0 || dvdY >= SCREEN_HEIGHT - dvdLogoHeight) dy = -dy;

  if (blink && now > blinkUntil) {
    blink = false;
  }

  display.display();
  delay(20);
}
