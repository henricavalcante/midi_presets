#pragma once

#include <Arduino.h>
#include <Wire.h>

const byte ST7567S_ADDR = 0x3F;

const byte RMW = 0xE0;
const byte END = 0xEE;

class lcd_st7567s {
  public:
    lcd_st7567s();
    void Init();
    void WriteByte_command(byte dat);
    void WriteByte_dat(byte dat);
    void WriteCont_dat(byte str[], int len); // Fixed: Pass length
    void Clear(bool invcolor = false);
    void ClearLine(int line, bool invcolor = false);
    void DisplayPixel(int x, int y);
    void ClearPixel(int x, int y);
    void Cursor(int x, int y);
    void Println(int line, const char *str);
    void Print(const char *str); // Use const char* for strings
    void testPixel(int t);
    void DrawLine(int x1, int y1, int x2, int y2, bool invcolor);
    void WriteFont(int num);

  private:
    void MoveCursor();
    byte addr = ST7567S_ADDR;
    int cursor[2] = {0, 0}; // [y_page, x_char_pos]
    int GetFontIndex(char c);
};