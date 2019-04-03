/***************************************************
  This is an example for our Sunfounder Emo module
  connection:
     (for UNO)
  MOSI: pin 11
  MISO: pin 12
  SCK: pin 13
  CS : pin 10
  VCC & VIN & GND  5V
  use external power supply

  http://wiki.sunfounder.cc/index.php?title=24x8_LED_Dot_Matrix_Module_-_Emo

  $HOME/Documents/Arduino/libraries/SunFounder_Emo/SunFounder_Emo.cpp

 ****************************************************/

#include <SunFounder_Emo.h>

Emo my_emo(10);  // CS pin

#define ULONG_MAX  500000

const byte OFF[] = {
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00};

const byte all_on[] = {
    0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF};

byte emo_buffer24[24];

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

void setup() {
    Serial.begin(9600);
    Serial.println("Emo go!");
    init_b_digit();
}

void loop() {

  char sz_time[6];

  // !!!! show_string() is no implemented
  //
  //my_emo.show_string(szTime, 1);
  
  //all_on_off();

/*
  convert_3_digit_to_emo_buffer(digit_1, digit_3, digit_8, emo_buffer24, 24);
  my_emo.show_emo(emo_buffer24);
  delay(1000);
*/

  snprintf(sz_time, sizeof(sz_time), "23:59");
  convert_time_string_to_emo_buffer(sz_time, emo_buffer24, 24);
  my_emo.show_emo(emo_buffer24);
  delay(1000);
}

void all_on_off() {
    my_emo.show_emo(OFF);
    Serial.println("OFF");
    delay(1000);

    my_emo.show_emo(all_on);
    Serial.println("    ON");
    delay(1000);
}
