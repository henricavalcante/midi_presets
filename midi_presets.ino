#include"lcd_st7567s.h"
#include <MIDI.h>
#include "music.h"

// Creates a MIDI instance for the native USB port.
// This instance will be named 'MIDI' by default.
MIDI_CREATE_DEFAULT_INSTANCE();

// Creates a MIDI instance for the physical hardware port (TX/RX pins)
// We'll give it a new name, 'MIDI_Hardware', to avoid conflicts.
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI_Hardware);

lcd_st7567s Lcd;

#define CLK 2
#define DT 4
#define SW 3

int selectedMusic = 0;
int activeMusic = 0;

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
  Setlist
} ApplicationState;

ApplicationState currentState = Menu;

void setup() {
  //Serial.begin(115200);

  Lcd.Init();
  Lcd.WriteByte_command(lcdStatus);
  Lcd.testPixel(1);

  // Initialize the music map
  initMusicMap();

  //displayMusic(selectedMusic);
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

  // Check for long button press (3 seconds)
  if (buttonIsPressed && !longPressDetected && (tickTime - buttonPressStartTime >= LONG_PRESS_DURATION)) {
    longPressDetected = true;
    currentState = Menu;
    displayMenu();
  }

  // Check for normal button press
  if (buttonHasBeenPressed == true) {
    if (currentState == Menu) {
      if (selectedMenuOption == 0) {
        currentState = AllSongs;
        activateMusic(selectedMusic);
      } else if (selectedMenuOption == 1) {
        currentState = Setlist;
      }
    } else if (currentState == AllSongs) {
       activateMusic(selectedMusic);
   }

    buttonHasBeenPressed = false;
  }

  if (encodeHasBeenUpdated == true && tickTime - lastEncodeUpdate > DEBOUNCE_TIME) {
    encodeHasBeenUpdated = false;
    if (currentState == AllSongs) {
        displayMusic(selectedMusic);
    } else if (currentState == Menu) {
        displayMenu();
    }
  }

  blink();
}

unsigned long blinkTime = 0;

void blink() {
  if (selectedMusic != activeMusic && tickTime - blinkTime >= LCD_BLINK_INTERVAL) {
    blinkTime = tickTime;

    if (lcdStatus == LCD_NORMAL) {
      lcdStatus = LCD_INVERTED;
    } else {
      lcdStatus = LCD_NORMAL;
    }

    Lcd.WriteByte_command(lcdStatus);
  } if (selectedMusic == activeMusic && lcdStatus == LCD_INVERTED) {
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

  // Display menu options with the selected one inverted
  Lcd.Println(2, "1. All songs", selectedMenuOption == 0);
  Lcd.Println(3, "2. Setlist", selectedMenuOption == 1);
}

void displayMusic(int musicIndex) {
  Music m = getMusic(musicIndex);

  char author[LCD_COLUMNS+1];
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

  for (int i = 0; i< COMMENT_LINES_PER_SONG; i++) {
    Lcd.Println(i+3, m.comments[i]);
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
          }
      } else {
          if (currentState == Menu) {
            selectedMenuOption = 0;
          } else if (currentState == AllSongs) {
            if (--selectedMusic < 0) {
              selectedMusic = 0;
            }
          }
      }
      lastEncodeUpdate = millis();
      encodeHasBeenUpdated = true;
    }
    lastStateCLK = currentStateCLK;
}

void buttonPress() {
  // Button is pressed (LOW)
  if (digitalRead(SW) == LOW) {
    if (millis() - lastButtonPress > DEBOUNCE_TIME) {
      // If this is a new press (not already being held)
      if (!buttonIsPressed) {
        buttonIsPressed = true;
        buttonPressStartTime = millis();
      }
    }
    lastButtonPress = millis();
  } 
  // Button is released (HIGH)
  else {
    if (buttonIsPressed) {
      // If button was held for less than LONG_PRESS_DURATION, treat as a normal press
      if (millis() - buttonPressStartTime < LONG_PRESS_DURATION && !longPressDetected) {
        buttonHasBeenPressed = true;
      }
      buttonIsPressed = false;
      longPressDetected = false;
    }
  }
}
