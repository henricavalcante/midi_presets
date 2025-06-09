#include"lcd_st7567s.h"
#include <MIDI.h>

#define LCD_COLUMNS 18
#define MIDI_EVENTS_PER_SONG 16
#define COMMENT_LINES_PER_SONG 5
#define TOTAL_MUSICS 49

#define CH_NORD_LEAD_LAYER1 1
#define CH_NORD_LEAD_LAYER2 2
#define CH_NORD_LEAD_LAYER3 3
#define CH_NORD_LEAD_LAYER4 4
#define CH_NORD_LEAD 5
#define CH_STRYMON_MOBIUS 6
#define CH_STRYMON_TIMELINE 7
#define CH_STRYMON_BIGSKY 8
#define CH_SP404_PADS1 10
#define CH_SP404_PADS1 11

enum MidiEventType {
  PC,
  CC,
  NOTE_ON,
  NOTE_OFF,
};

struct MidiEvent {
  int channel;
  MidiEventType type;
  int key;
  int value;
};

struct Music {
  char author[LCD_COLUMNS+1];
  char title[LCD_COLUMNS+1];
  char key[3];
  byte tempo;
  MidiEvent midiPreset[MIDI_EVENTS_PER_SONG];
  char comments[COMMENT_LINES_PER_SONG][LCD_COLUMNS+1];
  MidiEvent midiButton1[MIDI_EVENTS_PER_SONG];
  MidiEvent midiButton2[MIDI_EVENTS_PER_SONG];
  MidiEvent midiButton3[MIDI_EVENTS_PER_SONG];
  MidiEvent midiButton4[MIDI_EVENTS_PER_SONG];
  MidiEvent midiButton5[MIDI_EVENTS_PER_SONG];
  MidiEvent midiButton6[MIDI_EVENTS_PER_SONG];
};

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

byte lastStateCLK;

bool buttonHasBeenPressed = false;
unsigned long lastButtonPress = 0;
bool encodeHasBeenUpdated = false;
unsigned long lastEncodeUpdate = 0;

#define LCD_NORMAL 0xA7
#define LCD_INVERTED 0xA6
#define LCD_BLINK_INTERVAL 1000
byte lcdStatus = LCD_NORMAL;

#define DEBOUNCE_TIME 50

void setup() {
  //Serial.begin(9600);

  Lcd.Init();
  Lcd.WriteByte_command(lcdStatus);
  Lcd.testPixel(1);
  displayMusic(selectedMusic);

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
  if (buttonHasBeenPressed == true) {
    activateMusic(selectedMusic);
    buttonHasBeenPressed = false;
  }

  if (encodeHasBeenUpdated == true && tickTime - lastEncodeUpdate > DEBOUNCE_TIME) {
    encodeHasBeenUpdated = false;
    displayMusic(selectedMusic);
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
      if (++selectedMusic > TOTAL_MUSICS - 1) {
        selectedMusic = TOTAL_MUSICS - 1;
      }
    } else {
      if (--selectedMusic < 0) {
        selectedMusic = 0;
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
      buttonHasBeenPressed = true;
		}
		lastButtonPress = millis();
	}
}

Music getMusic(int musicIndex) {
  switch (musicIndex) {
    case 0:
      return (Music) {
        "Montgomery",
        "Frente ao Sol",
        "C#",
        0,
        {
          {CH_NORD_LEAD, CC, 0, 0},
          {CH_NORD_LEAD, CC, 32, 1},
          {CH_NORD_LEAD, PC, 0, 0}
        },
        {
          "",
          "c# d e f#"
        }
      };
    case 1:
      return (Music) {
        "U2",
        "Mysterious Ways",
        "G",
        100,
        {
          {CH_NORD_LEAD, CC, 0, 0},
          {CH_NORD_LEAD, CC, 32, 1},
          {CH_NORD_LEAD, PC, 0, 1}
        },
        {
          "V: g c g d",
          "PC: G C",
          "C: G C A# F",
          "O: D ... Gm C Gm F"
        }
      };
    case 2:
      return (Music) {
        "A-Ha",
        "I've been losing y",
        "C",
        100,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 2 }
        }
      };
    case 3:
      return (Music) {
        "Montgomery",
        "Fugas e Lamentos",
        "C",
        100,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 3 }
        }
      };
    case 4:
      return (Music) {
        "Duran Duran",
        "Save a prayer",
        "",
        113,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 4 }
        }
      };
    case 5:
      return (Music) {
        "Chris Isaak",
        "Wicked Game",
        "B",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 5 }
        }
      };
    case 6:
      return (Music) {
        "Montgomery",
        "Final de Tempestad",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 6 }
        }
      };
    case 7:
      return (Music) {
        "Joy Division",
        "Love Will Tear Us",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 7 }
        }
      };
    case 8:
      return (Music) {
        "Montgomery",
        "Estrada de Sonhos",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 8 }
        }
      };
    case 9:
      return (Music) {
        "The Verve",
        "Bitter Sweet S.",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 9 }
        }
      };
    case 10:
      return (Music) {
        "Uns e Outros",
        "Carta aos Missiona",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 10 }
        }
      };
    case 11:
      return (Music) {
        "Montgomery",
        "Algum Lugar",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 11 }
        }
      };
    case 12:
      return (Music) {
        "Legião Urbana",
        "Indios",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 12 }
        }
      };
    case 13:
      return (Music) {
        "Montgomery",
        "Pintura",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 13 }
        }
      };
    case 14:
      return (Music) {
        "Depeche Mode",
        "Enjoy the Silence",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 14 }
        }
      };
    case 15:
      return (Music) {
        "Montgomery",
        "A mesma poesia",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 15 }
        }
      };
    case 16:
      return (Music) {
        "Montgomery",
        "Pinoquio",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 16 }
        }
      };
    case 17:
      return (Music) {
        "Echo & The Bunny",
        "Lips Like Sugar",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 17 }
        }
      };
    case 18:
      return (Music) {
        "Montgomery",
        "Até o sol nascer",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 18 }
        }
      };
    case 19:
      return (Music) {
        "The Cure",
        "Just Like Heaven",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 19 }
        }
      };
    case 20:
      return (Music) {
        "Montgomery",
        "Êxodo",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 20 }
        }
      };
    case 21:
      return (Music) {
        "RPM",
        "Juvenilia",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 21 }
        }
      };
    case 22:
      return (Music) {
        "Montgomery",
        "Beijos de Arame F",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 22 }
        }
      };
    case 23:
      return (Music) {
        "Pixies",
        "Where is my mind",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 23 }
        }
      };
    case 24:
      return (Music) {
        "Barão Vermelho",
        "Meus bons Amigos",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 24 }
        }
      };
    case 25:
      return (Music) {
        "REM",
        "Crush With Eyeline",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 25 }
        }
      };
    case 26:
      return (Music) {
        "Simple Minds",
        "Don't you ...",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 26 }
        }
      };
    case 27:
      return (Music) {
        "Oasis",
        "Wonderwall",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 27 }
        }
      };
    case 28:
      return (Music) {
        "Legião Urbana",
        "Daniel na cova",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 28 }
        }
      };
    case 29:
      return (Music) {
        "INXS",
        "Never tear us apar",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 29 }
        }
      };
    case 30:
      return (Music) {
        "The weeknd",
        "Blinding Lights",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 30 }
        }
      };
    case 31:
      return (Music) {
        "REM",
        "The one I love",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 31 }
        }
      };
    case 32:
      return (Music) {
        "U2",
        "New Year's Day",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 32 }
        }
      };
    case 33:
      return (Music) {
        "Montgomery",
        "Viver Morrer e Esq",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 33 }
        }
      };
    case 34:
      return (Music) {
        "Montgomery",
        "Me Escolha",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 34 }
        }
      };
    case 35:
      return (Music) {
        "Montgomery",
        "O Perdão",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 35 }
        }
      };
    case 36:
      return (Music) {
        "Montgomery",
        "Chuva Inocente",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 36 }
        }
      };
    case 37:
      return (Music) {
        "Montgomery",
        "Todo Tempo",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 37 }
        }
      };
    case 38:
      return (Music) {
        "Plebe Rude",
        "Ate quando esperar",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 38 }
        }
      };
    case 39:
      return (Music) {
        "Travis",
        "Side",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 39 }
        }
      };
    case 40:
      return (Music) {
        "Bruce Springsteen",
        "Dancing in the dar",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 40 }
        }
      };
    case 41:
      return (Music) {
        "Garbage",
        "Only happy when it",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 41 }
        }
      };
    case 42:
      return (Music) {
        "U2",
        "With or Without Yu",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 42 }
        }
      };
    case 43:
      return (Music) {
        "The Cult",
        "She sells sanctuar",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 43 }
        }
      };
    case 44:
      return (Music) {
        "Ira",
        "Envelheco Na Cidad",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 44 }
        }
      };
    case 45:
      return (Music) {
        "The Killers",
        "Somebody Told Me",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 45 }
        }
      };
    case 46:
      return (Music) {
        "Oasis",
        "Stop Crying",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 46 }
        }
      };
    case 47:
      return (Music) {
        "David Bowie",
        "Heroes",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 47 }
        }
      };
    case 48:
      return (Music) {
        "Franz Ferdinand",
        "Take Me Out",
        "",
        0,
        {
          { CH_NORD_LEAD, CC, 0, 0 },
          { CH_NORD_LEAD, CC, 32, 1 },
          { CH_NORD_LEAD, PC, 0, 48 }
        }
      };
  }
}