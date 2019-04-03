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
#include <SunFounder_Emo.h>

Emo my_emo(10);  // CS pin

#define ULONG_MAX  500000

byte emo_buffer24[24];

int hour, min, sec, prevMin;


// ----- begin copy paste

const char * digit_0[] = {
    " ##     ",
    "#  #    ",
    "#  #    ",
    "#  #    ",
    "#  #    ",
    "#  #    ",
    " ##     ",
    "        "
};

const char * digit_1[] = {
    "  #     ",
    " ##     ",
    "# #     ",
    "  #     ",
    "  #     ",
    "  #     ",
    "  #     ",
    "        "
};

const char * digit_2[] = {
    " ##     ",
    "#  #    ",
    "   #    ",
    "  #     ",
    " #      ",
    "#       ",
    "####    ",
    "        "
};

const char * digit_3[] = {
    " ##     ",
    "#  #    ",
    "   #    ",
    "  #     ",
    "   #    ",
    "#  #    ",
    " ##     ",
    "        "
};

const char * digit_4[] = {
    "#  #    ",
    "#  #    ",
    "#  #    ",
    "####    ",
    "   #    ",
    "   #    ",
    "   #    ",
    "        "
};

const char * digit_5[] = {
    "####    ",
    "#       ",
    "#       ",
    "###     ",
    "   #    ",
    "#  #    ",
    " ##     ",
    "        "
};

const char * digit_6[] = {
    " ##     ",
    "#  #    ",
    "#       ",
    "###     ",
    "#  #    ",
    "#  #    ",
    " ##     ",
    "        "
};

const char * digit_7[] = {
    "####    ",
    "   #    ",
    "  #     ",
    " #      ",
    "#       ",
    "#       ",
    "#       ",
    "        "
};

const char * digit_8[] = {
    " ##     ",
    "#  #    ",
    "#  #    ",
    " ##     ",
    "#  #    ",
    "#  #    ",
    " ##     ",
    "        "
};

const char * digit_9[] = {
    " ##     ",
    "#  #    ",
    "#  #    ",
    " ###    ",
    "   #    ",
    "#  #    ",
    " ##     ",
    "        "
};

const char * sep_minutes[] = {
    "        ",
    "        ",
    "#       ",
    "        ",
    "#       ",
    "        ",
    "        ",
    "        "
};

byte b_digit_0[8];
byte b_digit_1[8];
byte b_digit_2[8];
byte b_digit_3[8];
byte b_digit_4[8];
byte b_digit_5[8];
byte b_digit_6[8];
byte b_digit_7[8];
byte b_digit_8[8];
byte b_digit_9[8];
byte b_sep_minutes[8];

int binstr2ul(const char *s, unsigned long *num);
void strreplace(char s[], char chr, char repl_chr);


void convert_string_digit_to_byte_digit(const char *p[], byte *pOut)
{
    char sz_buff[16];
    for(int ii = 0; ii < 8; ii++) {
        unsigned long ulByte;
        strcpy(sz_buff, p[ii]);
        strreplace(sz_buff, ' ', '0');
        strreplace(sz_buff, '#', '1');
        binstr2ul(sz_buff, &ulByte);
        byte outChar = (byte)ulByte;
        *pOut++ = outChar;
    }
}

void init_b_digit()
{
    convert_string_digit_to_byte_digit(digit_0, b_digit_0);
    convert_string_digit_to_byte_digit(digit_1, b_digit_1);
    convert_string_digit_to_byte_digit(digit_2, b_digit_2);
    convert_string_digit_to_byte_digit(digit_3, b_digit_3);
    convert_string_digit_to_byte_digit(digit_4, b_digit_4);
    convert_string_digit_to_byte_digit(digit_5, b_digit_5);
    convert_string_digit_to_byte_digit(digit_6, b_digit_6);
    convert_string_digit_to_byte_digit(digit_7, b_digit_7);
    convert_string_digit_to_byte_digit(digit_8, b_digit_8);
    convert_string_digit_to_byte_digit(digit_9, b_digit_9);
    
    convert_string_digit_to_byte_digit(sep_minutes, b_sep_minutes);
}

// from: https://codereview.stackexchange.com/questions/43256/binary-string-to-integer-and-integer-to-binary-string
//
/*
 Parameters:
 s   - String with a maximum of log2(ULONG_MAX) binary characters
 num - Memory address to store the result in
 Return:
 - Status integer
 Error:
 - Negative returned on error check errno
 */
int binstr2ul(const char *s, unsigned long *num)
{
    unsigned long rc;
    for (rc = 0; '\0' != *s; s++) {
        if (rc > (ULONG_MAX/2)) {
            return -1;
        } else if ('1' == *s) {
            rc = (rc * 2) + 1;
        } else if ('0' == *s) {
            rc *= 2;
        } else {
            return -1;
        }
    }
    *num = rc;
    return 0;
}

/*
 Parameters:
 num     - The number to convert to a binary string
 s       - Pointer to a memory region to return the string to
 len     - Size in bytes of the region pointed to by s
 Return:
 - Pointer to the beginning of the string
 Error:
 - NULL returned on error check errno
 */
char *ul2binstr(unsigned long num, char *s, size_t len)
{
    if (0 == len) {
        return NULL;
    } else {
        s[--len] = '\0';
        memset(s, '0', len);
    }
    
    do {
        if (0 == len) {
            return NULL;
        } else {
            s[--len] = ((num & 1) ? '1' : '0');
        }
    } while ((num >>= 1) != 0);
    
    return s + len;
}

void strreplace(char s[], char chr, char repl_chr)
{
    int i=0;
    while(s[i]!='\0')
    {
        if(s[i]==chr)
        {
            s[i]=repl_chr;
        }
        i++;
    }
    return;
}

void convert_3_digit_to_emo_buffer(const char *digit_1[], const char *digit_2[], const char *digit_3[], unsigned char *emo_buffer24, int emo_buffer_size)
{
    unsigned long ul_char;
    int emo_buffer_index = 0;
    int rc;
    char sz_buff[16];
    
    for (int ii=0; ii<8; ii++) {
        
        // digit 1
        //
        strcpy(sz_buff, digit_1[ii]);
        strreplace(sz_buff, ' ', '0');
        strreplace(sz_buff, '#', '1');
        rc = binstr2ul(sz_buff, &ul_char);
        emo_buffer24[emo_buffer_index++] = (unsigned char)ul_char;
        
        // digit 2
        //
        strcpy(sz_buff, digit_2[ii]);
        strreplace(sz_buff, ' ', '0');
        strreplace(sz_buff, '#', '1');
        rc = binstr2ul(sz_buff, &ul_char);
        emo_buffer24[emo_buffer_index++] = (unsigned char)ul_char;
        
        // digit 3
        //
        strcpy(sz_buff, digit_3[ii]);
        strreplace(sz_buff, ' ', '0');
        strreplace(sz_buff, '#', '1');
        rc = binstr2ul(sz_buff, &ul_char);
        emo_buffer24[emo_buffer_index++] = (unsigned char)ul_char;
        
        // check emp_buffer_index with emo_buffer_size
    }
}

byte *get_digit_byte_array_from_time_char(char inChar)
{
    byte *pRet;
    
    switch(inChar) {
        case '0':
            pRet = b_digit_0;
            break;
        case '1':
            pRet = b_digit_1;
            break;
        case '2':
            pRet = b_digit_2;
            break;
        case '3':
            pRet = b_digit_3;
            break;
        case '4':
            pRet = b_digit_4;
            break;
        case '5':
            pRet = b_digit_5;
            break;
        case '6':
            pRet = b_digit_6;
            break;
        case '7':
            pRet = b_digit_7;
            break;
        case '8':
            pRet = b_digit_8;
            break;
        case '9':
            pRet = b_digit_9;
            break;
        case ':':
            pRet = b_sep_minutes;
            break;
        default:
            pRet = nullptr;
    }
    return pRet;
}

void convert_time_string_to_emo_buffer(char *pszTimeString, unsigned char *emo_buffer24, int emo_buffer_size)
{
    byte *hour_1 = get_digit_byte_array_from_time_char(pszTimeString[0]);
    byte *hour_2 = get_digit_byte_array_from_time_char(pszTimeString[1]);
    byte *sep = get_digit_byte_array_from_time_char(pszTimeString[2]);
    byte *min_1 = get_digit_byte_array_from_time_char(pszTimeString[3]);
    byte *min_2 = get_digit_byte_array_from_time_char(pszTimeString[4]);
    
    //  l2 = (1 << 7)|(1 << 3);
    int emo_index = 0;
    
    for (int ii = 0; ii < 8; ii++) {
        
        // screen 1
        //
        emo_buffer24[emo_index] = (hour_1[ii] >> 1) | (hour_2[ii] >> 6);
        emo_index++;
        
        // screen 2
        //
        emo_buffer24[emo_index] = (hour_2[ii] << 2) | (sep[ii] >> 3) | (min_1[ii] >> 5);
        emo_index++;
        
        // screen 3
        //
        emo_buffer24[emo_index] = (min_1[ii] << 3) | (min_2[ii] >> 2);
        emo_index++;
    }
}

// ----- end copy paste

void setup()   {
  SerialUSB.begin(9600);
  Bridge.begin();

  init_b_digit();
  clearHHMMSS();
  prevMin = -1;
}


void loop()
{
  int rc = 0;
  boolean bClearScreen = false;

  rc = getTimeFromDateCommand(&hour, &min, &sec);
  if (rc == 0) {
    dispTime(hour, min, sec);
  }
  /*
  if (rc == 0 && prevMin != min) {
    prevMin = min;
    dispTime(hour, min, sec);
  } */
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
  convert_time_string_to_emo_buffer(szBuf, emo_buffer24, 24);
  my_emo.show_emo(emo_buffer24);
}

void dipsError(char *pErr, int errCode)
{
  char szBuf[128];
  snprintf(szBuf, sizeof(szBuf), "Error: %s %d", pErr, errCode);
  SerialUSB.println(szBuf);
}

