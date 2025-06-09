#include "lcd_st7567s.h"
#include <MIDI.h>
#include "music.h"
#include "setlist.h"

MIDI_CREATE_DEFAULT_INSTANCE();
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI_Hardware);

lcd_st7567s Lcd;

#define CLK 2
#define DT 4
#define SW 3

int selectedMusic = 0;
int activeMusic = 0;

int selectedSetlist = 0;
int selectedSetlistSong = 0;
int activeSetlistSong = 0;

int selectedMenuOption = 0;

byte lastStateCLK;

bool buttonHasBeenPressed = false;
unsigned long lastButtonPress = 0;
bool encodeHasBeenUpdated = false;
unsigned long lastEncodeUpdate = 0;
bool buttonIsPressed = false;
unsigned long buttonPressStartTime = 0;
bool longPressDetected = false;
#define LONG_PRESS_DURATION 3000

#define LCD_NORMAL 0xA7
#define LCD_INVERTED 0xA6
#define LCD_BLINK_INTERVAL 1000
byte lcdStatus = LCD_NORMAL;

#define DEBOUNCE_TIME 50

typedef enum {
  Menu,
  AllSongs,
  Setlist,
  SetlistSongs
} ApplicationState;

ApplicationState currentState = Menu;

void setup() {
  //Serial.begin(115200);

  Lcd.Init();
  Lcd.WriteByte_command(lcdStatus);
  Lcd.testPixel(1);

  initMusicMap();
  initSetlists();

  displayMenu();

  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);

  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI_Hardware.begin(MIDI_CHANNEL_OMNI);

  lastStateCLK = digitalRead(CLK);
  attachInterrupt(digitalPinToInterrupt(CLK), updateEncoder, CHANGE);

  lastButtonPress = millis();
  attachInterrupt(digitalPinToInterrupt(SW), buttonPress, CHANGE);
}

unsigned long tickTime = millis();

void loop() {
  tickTime = millis();

  if (buttonIsPressed && !longPressDetected && (tickTime - buttonPressStartTime >= LONG_PRESS_DURATION)) {
    longPressDetected = true;
    currentState = Menu;
    displayMenu();
  }

  if (buttonHasBeenPressed == true) {
    if (currentState == Menu) {
      if (selectedMenuOption == 0) {
        currentState = AllSongs;
        activateMusic(selectedMusic);
      } else if (selectedMenuOption == 1) {
        currentState = Setlist;
        selectedSetlist = 0;
        selectedSetlistSong = 0;
        displaySetlist();
      }
    } else if (currentState == AllSongs) {
      activateMusic(selectedMusic);
    } else if (currentState == Setlist) {
      currentState = SetlistSongs;
      selectedSetlistSong = 0;
      displaySetlistSong(selectedSetlist, selectedSetlistSong);
    } else if (currentState == SetlistSongs) {
      activateSetlistSong(selectedSetlist, selectedSetlistSong);
    }

    buttonHasBeenPressed = false;
  }

  if (encodeHasBeenUpdated == true && tickTime - lastEncodeUpdate > DEBOUNCE_TIME) {
    encodeHasBeenUpdated = false;
    if (currentState == AllSongs) {
      displayMusic(selectedMusic);
    } else if (currentState == Menu) {
      displayMenu();
    } else if (currentState == Setlist) {
      displaySetlist();
    } else if (currentState == SetlistSongs) {
      displaySetlistSong(selectedSetlist, selectedSetlistSong);
    }
  }

  blink();
}

unsigned long blinkTime = 0;

void blink() {
  if (tickTime - blinkTime >= LCD_BLINK_INTERVAL && ((currentState == AllSongs && selectedMusic != activeMusic) || (currentState == SetlistSongs && selectedSetlistSong != activeSetlistSong))) {
    blinkTime = tickTime;

    if (lcdStatus == LCD_NORMAL) {
      lcdStatus = LCD_INVERTED;
    } else {
      lcdStatus = LCD_NORMAL;
    }

    Lcd.WriteByte_command(lcdStatus);
  }
  if (selectedMusic == activeMusic && lcdStatus == LCD_INVERTED) {
    lcdStatus = LCD_NORMAL;
    Lcd.WriteByte_command(lcdStatus);
  }
}

void activateMusic(int musicIndex) {
  activeMusic = musicIndex;
  Music m = getMusic(musicIndex);
  displayMusic(musicIndex);
  for (int i = 0; i < MIDI_EVENTS_PER_SONG; i++) {
    sendMidiEvent(m.midiPreset[i]);
  }
}

void activateSetlistSong(int setlistIndex, int songIndex) {
  activeSetlistSong = songIndex;
  Music m = getSetlistSong(setlistIndex, songIndex);
  displaySetlistSong(setlistIndex, songIndex);
  for (int i = 0; i < MIDI_EVENTS_PER_SONG; i++) {
    sendMidiEvent(m.midiPreset[i]);
  }
}

void sendMidiEvent(MidiEvent event) {
  if (event.type == PC) {
    MIDI.sendProgramChange(event.value, event.channel);
    MIDI_Hardware.sendProgramChange(event.value, event.channel);
    delay(10);
  } else if (event.type == CC) {
    MIDI.sendControlChange(event.key, event.value, event.channel);
    MIDI_Hardware.sendControlChange(event.key, event.value, event.channel);
    delay(10);
  }
}

void displayMenu() {
  Lcd.Clear(false);
  Lcd.Println(0, "Menu");
  Lcd.DrawLine(0, 10, 128, 10, false);

  Lcd.Println(2, "1. All songs", selectedMenuOption == 0);
  Lcd.Println(3, "2. Setlist", selectedMenuOption == 1);
}

void displaySetlist() {
  Lcd.Clear(false);
  Lcd.Println(0, "Setlists");
  Lcd.DrawLine(0, 10, 128, 10, false);

  struct Setlist setlist = getSetlist(selectedSetlist);

  char setlistInfo[LCD_COLUMNS + 1];
  sprintf(setlistInfo, "%d. %s", selectedSetlist + 1, setlist.title);
  Lcd.Println(2, setlistInfo, true);
  Lcd.Println(3, setlist.date);

  char songCount[LCD_COLUMNS + 1];
  sprintf(songCount, "Songs: %d", setlist.songCount);
  Lcd.Println(4, songCount);
}

void displaySetlistSong(int setlistIndex, int songIndex) {
  struct Setlist setlist = getSetlist(setlistIndex);
  Music m = getSetlistSong(setlistIndex, songIndex);

  char author[LCD_COLUMNS + 1];
  char musicNum[4];
  sprintf(musicNum, "%d/%d", songIndex + 1, setlist.songCount);
  strcpy(author, musicNum);
  strcat(author, " ");
  strcat(author, m.author);
  Lcd.Println(0, author);

  Lcd.Println(1, m.title);

  if (encodeHasBeenUpdated == true) {
    return;
  }

  Lcd.ClearLine(2, false);
  Lcd.Cursor(0, 2);
  Lcd.WriteFont(95);
  Lcd.Print(m.key);
  Lcd.Print(" - ");
  Lcd.WriteFont(96);
  char musicTempo[3];
  itoa(m.tempo, musicTempo, 10);
  Lcd.Print(musicTempo);

  for (int i = 0; i < COMMENT_LINES_PER_SONG; i++) {
    Lcd.Println(i + 3, m.comments[i]);
  }

  Lcd.DrawLine(0, 24, 128, 24, false);
}

void displayMusic(int musicIndex) {
  Music m = getMusic(musicIndex);

  char author[LCD_COLUMNS + 1];
  char musicNum[2];
  itoa(musicIndex + 1, musicNum, 10);
  strcpy(author, musicNum);
  strcat(author, " ");
  strcat(author, m.author);
  Lcd.Println(0, author);

  Lcd.Println(1, m.title);

  if (encodeHasBeenUpdated == true) {
    return;
  }

  Lcd.ClearLine(2, false);
  Lcd.Cursor(0, 2);
  Lcd.WriteFont(95);
  Lcd.Print(m.key);
  Lcd.Print(" - ");
  Lcd.WriteFont(96);
  char musicTempo[3];
  itoa(m.tempo, musicTempo, 10);
  Lcd.Print(musicTempo);

  for (int i = 0; i < COMMENT_LINES_PER_SONG; i++) {
    Lcd.Println(i + 3, m.comments[i]);
  }

  Lcd.DrawLine(0, 24, 128, 24, false);
}

void updateEncoder() {
  int currentStateCLK = digitalRead(CLK);
  if (currentStateCLK != lastStateCLK && currentStateCLK == 1) {
    if (digitalRead(DT) != currentStateCLK) {
      if (currentState == Menu) {
        selectedMenuOption = 1;
      } else if (currentState == AllSongs) {
        if (++selectedMusic > TOTAL_MUSICS - 1) {
          selectedMusic = TOTAL_MUSICS - 1;
        }
      } else if (currentState == Setlist) {
        if (++selectedSetlist > getSetlistCount() - 1) {
          selectedSetlist = getSetlistCount() - 1;
        }
      } else if (currentState == SetlistSongs) {
        struct Setlist setlist = getSetlist(selectedSetlist);
        if (++selectedSetlistSong > setlist.songCount - 1) {
          selectedSetlistSong = setlist.songCount - 1;
        }
      }
    } else {
      if (currentState == Menu) {
        selectedMenuOption = 0;
      } else if (currentState == AllSongs) {
        if (--selectedMusic < 0) {
          selectedMusic = 0;
        }
      } else if (currentState == Setlist) {
        if (--selectedSetlist < 0) {
          selectedSetlist = 0;
        }
      } else if (currentState == SetlistSongs) {
        if (--selectedSetlistSong < 0) {
          selectedSetlistSong = 0;
        }
      }
    }
    lastEncodeUpdate = millis();
    encodeHasBeenUpdated = true;
  }
  lastStateCLK = currentStateCLK;
}

void buttonPress() {
  if (digitalRead(SW) == LOW) {
    if (millis() - lastButtonPress > DEBOUNCE_TIME) {
      if (!buttonIsPressed) {
        buttonIsPressed = true;
        buttonPressStartTime = millis();
      }
    }
    lastButtonPress = millis();
  }
  else {
    if (buttonIsPressed) {
      if (millis() - buttonPressStartTime < LONG_PRESS_DURATION && !longPressDetected) {
        buttonHasBeenPressed = true;
      }
      buttonIsPressed = false;
      longPressDetected = false;
    }
  }
}
