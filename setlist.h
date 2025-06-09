#ifndef SETLIST_H
#define SETLIST_H

#include <Arduino.h>
#include "music.h"

#define MAX_SETLISTS 10

#define MAX_SONGS_PER_SETLIST 30

struct Setlist {
  char title[LCD_COLUMNS+1];
  char date[11];
  int songCount;
  char songCodes[MAX_SONGS_PER_SETLIST][4];
};

int getSetlistCount();
Setlist getSetlist(int setlistIndex);
Music getSetlistSong(int setlistIndex, int songIndex);
void initSetlists();

#endif