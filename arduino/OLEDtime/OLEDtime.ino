//
// display the current time on a oled display using a arduino yun
//
// components:
// yun
// oled 128 x 64 https://github.com/adafruit/Adafruit_SSD1306
//               http://www.adafruit.com/category/63_98
//

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Process.h>

#define OLED_RESET 4

Adafruit_SSD1306 display(OLED_RESET);

int hour, min, sec;

/*
  #if (SSD1306_LCDHEIGHT != 64)
  #error("Height incorrect, please fix Adafruit_SSD1306.h!");
  #endif
*/

void setup()   {

  SerialUSB.begin(9600);
  Bridge.begin();

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  // initialize with the I2C addr 0x3C (for the 128x64)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  clearHHMMSS();
}


void loop()
{
  int rc;
  rc = getTimeFromDateCommand(&hour, &min, &sec);
  if (0 == rc) {
    dispTime(hour, min, sec);
  }
  else {
    dipsError("linux date", rc);
  }
  delay(1000);
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

  // Clear the buffer.
  display.clearDisplay();

  // display seconds
  //
  snprintf(szBuf, sizeof(szBuf), "%02d", sec);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  //display.println(szBuf);

  // display HH:MM
  //
  display.setTextSize(3);
  snprintf(szBuf, sizeof(szBuf), " %02d:%02d", hour, min);
  display.setCursor(0, 10);
  display.println(szBuf);

  display.display();
}


void dipsError(char *pErr, int errCode)
{
  char szErrBuf[128];
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);  
  snprintf(szErrBuf, sizeof(szErrBuf), "Error: %s %d", pErr, errCode);
  display.println(szErrBuf);
  display.display();
}


/*
  display.print("0x"); display.println(0xDEADBEEF, HEX);

  display.display();
  delay(2000);
  display.clearDisplay();

  // miniature bitmap display
  display.drawBitmap(30, 16,  logo16_glcd_bmp, 16, 16, 1);
  display.display();
  delay(1);

  // invert the display
  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);

  display.clearDisplay();
*/

