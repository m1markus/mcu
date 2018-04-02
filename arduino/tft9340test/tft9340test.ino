//
// display the current time on a oled display using a arduino yun
//
// components:
// yun
// tft 240 x 320 ILI9340C    https://github.com/adafruit/Adafruit_ILI9340
//
// wiring: https://www.arduino.cc/en/Guide/TFTtoBoards
//
// GFX lib: https://learn.adafruit.com/adafruit-gfx-graphics-library/using-fonts
//
// helper from ili9340:
// uint16_t Color565(uint8_t r, uint8_t g, uint8_t b);

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9340.h>
#include <Fonts/FreeMono24pt7b.h>
#include <Process.h>

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

// These are the pins used for the UNO
// for Due/Mega/Leonardo/Yun use the hardware SPI pins (which are different)
// #define _sclk 13
// #define _miso 12
// #define _mosi 11

#define _cs 10
#define _dc 9
#define _rst 8

// Using software SPI is really not suggested, its incredibly slow
//Adafruit_ILI9340 tft = Adafruit_ILI9340(_cs, _dc, _mosi, _sclk, _rst, _miso);
// Use hardware SPI

Adafruit_ILI9340 tft = Adafruit_ILI9340(_cs, _dc, _rst);

int hour, min, sec, prevMin;
uint16_t timeColor;


void setup()   {

  SerialUSB.begin(9600);
  Bridge.begin();
  tft.begin();

  testFillScreen();

  clearHHMMSS();

  // rotation values: 0, 1, 2, 3
  tft.setRotation(1);
  tft.setFont(&FreeMono24pt7b);

  //testText();
  
  tft.fillScreen(ILI9340_BLACK);

  // kind of blue
  //                   red green blue
  timeColor = tft.Color565(196, 207, 252);

  prevMin = -1;
}


void loop()
{
  int rc = 0;
  boolean bClearScreen = false;

  rc = getTimeFromDateCommand(&hour, &min, &sec);
  if (prevMin != min) {
    bClearScreen = true;
  }
  if (bClearScreen) {
     tft.fillScreen(ILI9340_BLACK);
  }
  
  // hour=20; min=99;
  if (0 == rc) {
      prevMin = min;
    dispTime(hour, min, sec);
  }
  else {
    dipsError("linux date", rc);
  }
  delay(1000);
}


void testFillScreen() {
  tft.fillScreen(ILI9340_RED);
  delay(1000);
  tft.fillScreen(ILI9340_BLACK);
  delay(1000);
  tft.fillScreen(ILI9340_GREEN);
  delay(1000);
  tft.fillScreen(ILI9340_BLACK);
  delay(1000);
  tft.fillScreen(ILI9340_BLUE);
  delay(1000);
  tft.fillScreen(ILI9340_BLACK);
  delay(1000);
}


void testText() {
  tft.fillScreen(ILI9340_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9340_WHITE);  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(ILI9340_YELLOW); tft.setTextSize(2);
  tft.println(1234.56);
  tft.setTextColor(ILI9340_RED);    tft.setTextSize(3);
  tft.println(0xDEADBEEF, HEX);
  tft.println();
  tft.setTextColor(ILI9340_GREEN);
  tft.setTextSize(5);
  tft.println("Groop");
  tft.setTextSize(2);
  tft.println("I implore thee,");
  tft.setTextSize(1);
  tft.println("my foonting turlingdromes.");
  tft.println("And hooptiously drangle me");
  tft.println("with crinkly bindlewurdles,");
  tft.println("Or I will rend thee");
  tft.println("in the gobberwarts");
  tft.println("with my blurglecruncheon,");
  tft.println("see if I don't!");
}


void clearHHMMSS()
{
  hour = 0;
  min = 0;
  sec = 0;
}


// Call the yun linux date(1) command:
// $ date "+%T"
//
int getTimeFromDateCommand(int *hour, int *min, int *sec)
{
  char szBuf[16];
  char *pBuf = szBuf;
  Process p;
  p.begin("date");
  p.addParameter("+%T");
  p.run();

  int rc = 0;
  rc = p.exitValue();
  if (0 == rc) {
    while (p.available() > 0) {
      char c = p.read();
      if (c == '\n') {
        *pBuf = 0;
        break;

      } else {
        *pBuf++ = c;
      }
    }
    *pBuf = 0;

    // read the HH:MM:SS from the char array
    //
    sscanf(szBuf, "%d:%d:%d", hour, min, sec);
  }
  else {
    snprintf(szBuf, sizeof(szBuf), "Err %d", rc);
    clearHHMMSS();
  }

  SerialUSB.println(szBuf);
  return rc;
}


void dispTime(int hour, int min, int sec)
{
  char szBuf[16];
  // display seconds
  //snprintf(szBuf, sizeof(szBuf), "%02d", sec);
  //
  // display HH:MM
  //
  tft.setTextColor(ILI9340_WHITE);
  //tft.setTextColor(timeColor);
  tft.setTextSize(2);
  snprintf(szBuf, sizeof(szBuf), "%02d:%02d", hour, min);
  tft.setCursor(15, 150);
  tft.println(szBuf);
}


void dipsError(char *pErr, int errCode)
{
  char szErrBuf[128];
  tft.fillScreen(ILI9340_BLACK);
  tft.setTextColor(ILI9340_RED);
  tft.setTextSize(3);
  snprintf(szErrBuf, sizeof(szErrBuf), "Error: %s %d", pErr, errCode);
  tft.setCursor(0, 0);
  tft.println(szErrBuf);
}
