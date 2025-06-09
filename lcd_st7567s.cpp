#include "lcd_st7567s.h"
#include <avr/pgmspace.h>
#include "font.c"

lcd_st7567s::lcd_st7567s() {
    // Constructor can be empty or used for default setup
}

void lcd_st7567s::WriteByte_command(byte dat) {
    Wire.beginTransmission(addr);
    Wire.write((byte)0x00); // Co=0, A0=0 (Command)
    Wire.write(dat);
    Wire.endTransmission();
}

void lcd_st7567s::WriteByte_dat(byte dat) {
    Wire.beginTransmission(addr);
    Wire.write((byte)0x40); // Co=0, A0=1 (Data)
    Wire.write(dat);
    Wire.endTransmission();
}

void lcd_st7567s::Init() {
    Wire.begin();
    Wire.setClock(100000L); // Set I2C clock to 100kHz (Standard)
    delay(20); // Give it a bit more time

    WriteByte_command(0xE2); // Internal reset
    delay(20);
    WriteByte_command(0xA2); // LCD bias 1/9 (for 128x64)
    WriteByte_command(0xA0); // ADC select (0xA0=Normal, 0xA1=Reversed)
    WriteByte_command(0xC8); // COM output (0xC8=Reversed, 0xC0=Normal)
    WriteByte_command(0xA6); // 0xA6 Normal display, 0xA7 Inverse

    // Adjust display brightness (Example values, tweak if needed)
    WriteByte_command(0x25); // V5 voltage RR (0x20-0x27)
    WriteByte_command(0x81); // Electronic volume mode set
    WriteByte_command(0x20); // Electronic volume value (0-63)

    // Power control
    WriteByte_command(0x2C); // Booster ON
    delay(1);
    WriteByte_command(0x2E); // Regulator ON
    delay(1);
    WriteByte_command(0x2F); // Follower ON
    delay(1);

    WriteByte_command(0x40); // Set display start line to 0
    WriteByte_command(0xAF); // Display ON
    delay(10);
    Clear(false); // Clear DDRAM
}

void lcd_st7567s::Clear(bool invcolor) {
    for (int page = 0; page < 8; page++) {
        WriteByte_command(0xB0 + page); // Set page address
        WriteByte_command(0x10);        // Set column address high (0)
        WriteByte_command(0x00);        // Set column address low (0)
        byte data = invcolor ? 0xFF : 0x00;
        for (int col = 0; col < 128; col++) {
            WriteByte_dat(data);
        }
    }
}

void lcd_st7567s::ClearLine(int line, bool invcolor) {
    if (line < 0 || line > 7) return; // Basic bounds check
    WriteByte_command(0xB0 + line); // Set page address
    WriteByte_command(0x10);        // Set column address high (0)
    WriteByte_command(0x00);        // Set column address low (0)
    byte data = invcolor ? 0xFF : 0x00;
    for (int col = 0; col < 128; col++) {
        WriteByte_dat(data);
    }
}

void lcd_st7567s::DisplayPixel(int x, int y) {
    if (x < 0 || x >= 128 || y < 0 || y >= 64) return; // Bounds check

    byte page = y / 8;
    byte bit_pos = y % 8;
    byte dat;

    WriteByte_command(0xb0 + page);    // Set page
    WriteByte_command(0x10 + x / 16);  // Set col high
    WriteByte_command(x % 16);         // Set col low

    WriteByte_command(RMW);            // Start RMW (0xE0)

    Wire.requestFrom(addr, (byte)2); // Request 2 bytes
    if (Wire.available() >= 2) {
        Wire.read();                    // Dummy read (discard)
        dat = Wire.read();              // Actual data
    } else {
        //Serial.println(F("RMW Read Failed!"));
        dat = 0; // Assume 0 on failure, or try reading 1?
        WriteByte_command(END);         // Try to end RMW
        return;                         // Abort on failure
    }

    dat |= (1 << bit_pos);            // Set the bit
    WriteByte_dat(dat);                 // Write data back
    WriteByte_command(END);             // End RMW (0xEE)
}

void lcd_st7567s::ClearPixel(int x, int y) {
    if (x < 0 || x >= 128 || y < 0 || y >= 64) return; // Bounds check

    byte page = y / 8;
    byte bit_pos = y % 8;
    byte dat;

    WriteByte_command(0xb0 + page);    // Set page
    WriteByte_command(0x10 + x / 16);  // Set col high
    WriteByte_command(x % 16);         // Set col low

    WriteByte_command(RMW);            // Start RMW (0xE0)

    Wire.requestFrom(addr, (byte)2); // Request 2 bytes
    if (Wire.available() >= 2) {
        Wire.read();                    // Dummy read
        dat = Wire.read();              // Actual data
    } else {
        //Serial.println(F("RMW Read Failed!"));
        dat = 0xFF; // Assume all set on failure, or try reading 1?
        WriteByte_command(END);         // Try to end RMW
        return;                         // Abort on failure
    }

    dat &= ~(1 << bit_pos);           // Clear the bit
    WriteByte_dat(dat);                 // Write data back
    WriteByte_command(END);             // End RMW (0xEE)
}

void lcd_st7567s::Cursor(int x, int y) {
    if (x > 17) { x = 17; }
    if (y > 7) { y = 7; } // Fixed typo: was x=7
    cursor[0] = y;
    cursor[1] = x;
    MoveCursor();
}

void lcd_st7567s::MoveCursor() {
    WriteByte_command(0xb0 + cursor[0]);
    int x_pixel = cursor[1] * 7;
    WriteByte_command(0x10 + (x_pixel / 16));
    WriteByte_command(0x00 + (x_pixel % 16));
}

void lcd_st7567s::WriteFont(int num) {
    for (int i = 0; i < 7; i++) {
        WriteByte_dat(font_7x8[num][i]);
    }
}

void lcd_st7567s::Println(int line, const char *str) {
  Cursor(0, line);
  int count = 0;
  while (*str) {
    char c = *str++;
    int font_index = GetFontIndex(c);

    if (font_index != -1) {
        WriteFont(font_index);
        count++;
    }
  }
  while (count < 18) {
    WriteFont(87);
    count++;
  }
}

void lcd_st7567s::Print(const char *str) {
  while (*str) {
    char c = *str++;
    int font_index = GetFontIndex(c);

    if (font_index != -1) {
        WriteFont(font_index);
    }
  }
}

int lcd_st7567s::GetFontIndex(char c) {
  int font_index = -1;
  if (c >= '0' && c <= '9') font_index = c - '0';
  else if (c >= 'a' && c <= 'z') font_index = c - 'a' + 10;
  else if (c >= 'A' && c <= 'Z') font_index = c - 'A' + 36;
  else {
    switch(c){
      case '!': font_index = 62; break;
      case '"': font_index = 63; break;
      case '#': font_index = 64; break;
      case '$': font_index = 65; break;
      case '%': font_index = 66; break;
      case '&': font_index = 67; break;
      case '\'':font_index = 68; break;
      case '(': font_index = 69; break;
      case ')': font_index = 70; break;
      case '*': font_index = 71; break;
      case '+': font_index = 72; break;
      case ',': font_index = 73; break;
      case '-': font_index = 74; break;
      case '/': font_index = 75; break;
      case ':': font_index = 76; break;
      case ';': font_index = 77; break;
      case '<': font_index = 78; break;
      case '=': font_index = 79; break;
      case '>': font_index = 80; break;
      case '?': font_index = 81; break;
      case '@': font_index = 82; break;
      case '{': font_index = 83; break;
      case '|': font_index = 84; break;
      case '}': font_index = 85; break;
      case '~': font_index = 86; break;
      case ' ': font_index = 87; break;
      case '.': font_index = 88; break;
      case '^': font_index = 89; break;
      case '_': font_index = 90; break;
      case '`': font_index = 91; break;
      case '[': font_index = 92; break;
      case '\\':font_index = 93; break;
      case ']': font_index = 94; break;
    }
  }
  return font_index;
}

void lcd_st7567s::testPixel(int t) {
    for (int x = 0; x < 8; x++) {
        WriteByte_command(0xb0 + x);
        WriteByte_command(0x10);
        WriteByte_command(0x00);
        for (int i = 0; i < 128; i++) {
            WriteByte_dat(0xff);
            delay(t);
        }
    }
    for (int x = 0; x < 8; x++) {
        WriteByte_command(0xb0 + x);
        WriteByte_command(0x00);
        WriteByte_command(0x10);
        for (int i = 0; i < 128; i++) {
            WriteByte_dat(0x00);
            delay(t);
        }
    }
}

void lcd_st7567s::WriteCont_dat(byte str[], int len) {
    if (len <= 0) return;
    Wire.beginTransmission(addr);
    for (int i = 0; i < len - 1; i++) {
        Wire.write(0xc0); // Co=1,A0=1 (Continuous data write)
        Wire.write(str[i]);
    }
    Wire.write(0x40); // Co=0,A0=1 (Last data write)
    Wire.write(str[len - 1]);
    Wire.endTransmission();
}

void lcd_st7567s::DrawLine(int x1, int y1, int x2, int y2, bool invcolor) {
    int tmp;
    int x, y;
    int dx, dy;
    int err;
    int ystep;

    int swapxy = 0;

    if (x1 > x2) dx = x1 - x2; else dx = x2 - x1;
    if (y1 > y2) dy = y1 - y2; else dy = y2 - y1;

    if (dy > dx) {
        swapxy = 1;
        tmp = dx; dx = dy; dy = tmp;
        tmp = x1; x1 = y1; y1 = tmp;
        tmp = x2; x2 = y2; y2 = tmp;
    }
    if (x1 > x2) {
        tmp = x1; x1 = x2; x2 = tmp;
        tmp = y1; y1 = y2; y2 = tmp;
    }
    err = dx >> 1;
    if (y2 > y1) ystep = 1; else ystep = -1;
    y = y1;

    for (x = x1; x <= x2; x++) {
        if (swapxy == 0) {
            if (invcolor) ClearPixel(x, y);
            else DisplayPixel(x, y);
        } else {
            if (invcolor) ClearPixel(y, x);
            else DisplayPixel(y, x);
        }
        err -= dy;
        if (err < 0) {
            y += ystep;
            err += dx;
        }
    }
}