
#include <SPI.h>
#include <Process.h>

#include <U8g2lib.h>
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void setup(void) {
  u8g2.begin();
}

void loop(void) {
  u8g2.clearBuffer();
  //u8g2.setFont(u8g2_font_ncenB14_tr);
  //u8g2.setFont(u8g2_font_logisoso32_tf);
  u8g2.setFont(u8g2_font_logisoso42_tn);
  u8g2.drawStr(0,55,"23:59");
  u8g2.sendBuffer();
}

/*

void setup(void) {
  u8g2.begin();
}

void loop(void) {
  u8g2.clearBuffer();
  //u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.setFont(u8g2_font_logisoso32_tf);
  u8g2.drawStr(15,55,"23:59");
  u8g2.sendBuffer();
}

*/

