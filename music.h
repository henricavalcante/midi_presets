#ifndef MUSIC_H
#define MUSIC_H

#include <Arduino.h>
#include <unordered_map>
#include <string>

// Define hash function for Arduino String
namespace std {
  template <>
  struct hash<String> {
    size_t operator()(const String& s) const {
      return hash<std::string>()(s.c_str());
    }
  };
}

// Constants from main file
#define LCD_COLUMNS 18
#define MIDI_EVENTS_PER_SONG 16
#define COMMENT_LINES_PER_SONG 5
#define TOTAL_MUSICS 53

#define CH_NORD_LEAD_LAYER1 1
#define CH_NORD_LEAD_LAYER2 2
#define CH_NORD_LEAD_LAYER3 3
#define CH_NORD_LEAD_LAYER4 4
#define CH_NORD_LEAD 5
#define CH_FLUIDSYNTH 6
#define CH_STRYMON_MOBIUS 7
#define CH_STRYMON_TIMELINE 8
#define CH_STRYMON_BIGSKY 9
#define CH_SP404_PADS1 10
#define CH_SP404_PADS1 11

// Enum and struct definitions
typedef enum {
  PC,
  CC
} MidiEventType;

struct MidiEvent {
  int channel;
  MidiEventType type;
  int key;
  int value;
};

struct Music {
  char code[4];
  char author[20];
  char title[27];
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

// Function declarations
Music getMusic(int musicIndex);
Music getMusic(const String& key);
void initMusicMap();

#endif // MUSIC_H
