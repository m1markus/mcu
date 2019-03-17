//
// display the current time on a oled display using a arduino yun
//
// components:
// ===========
//
// Arduino Yun
//
// 1.3" OLED display 128x64  connection I2C  controller SSH 1106  vcc 3.3V
//
// order:   https://smile.amazon.de/dp/B078J78R45/ref=pe_3044161_189395811_TE_SCE_dp_1
// library: https://github.com/olikraus/u8g2/
// fonts:   https://github.com/olikraus/u8g2/wiki/fntlistall
// 

#include <Process.h>
#include <U8g2lib.h>
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

int hour, min, sec, prevMin;

void setup()   {

  SerialUSB.begin(9600);
  Bridge.begin();

  u8g2.begin();

  clearHHMMSS();

  prevMin = -1;
}


void loop()
{
  int rc = 0;
  boolean bClearScreen = false;

  rc = getTimeFromDateCommand(&hour, &min, &sec);
  if (rc == 0 && prevMin != min) {
    prevMin = min;
    dispTime(hour, min, sec);
  }
  else if (rc != 0) {
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
  snprintf(szBuf, sizeof(szBuf), "%02d:%02d", hour, min);
  u8g2.clearBuffer();
  //u8g2.setFont(u8g2_font_ncenB14_tr);
  //u8g2.setFont(u8g2_font_logisoso32_tf);
  u8g2.setFont(u8g2_font_logisoso42_tn);
  u8g2.drawStr(0, 55, szBuf);
  u8g2.sendBuffer();
}


void dipsError(char *pErr, int errCode)
{
  char szBuf[128];
  snprintf(szBuf, sizeof(szBuf), "Error: %s %d", pErr, errCode);
  u8g2.setFont(u8g2_font_ncenB14_tr);
  //u8g2.setFont(u8g2_font_logisoso32_tf);
  //u8g2.setFont(u8g2_font_logisoso42_tn);
  u8g2.drawStr(0, 55, szBuf);
  u8g2.sendBuffer();
}

