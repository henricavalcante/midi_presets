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
#define TOTAL_MUSICS 55

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

#define CC_BANK_MSB 0
#define CC_VOLUME 7
#define CC_BANK_LSB 32
#define CC_STRYMON_PATCH_ACTIVE 102

// Originals
#define A01_FRENTE_AO_SOL "A01"
#define A02_FUGAS_E_LAMENTOS "A02"
#define A03_FINAL_DE_TEMPESTADE "A03"
#define A04_ESTRADA_DE_SONHOS "A04"
#define A05_ALGUM_LUGAR "A05"
#define A06_PINTURA "A06"
#define A07_A_MESMA_POESIA "A07"
#define A08_PINOQUIO "A08"
#define A09_ATE_O_SOL_NASCER "A09"
#define A10_BEIJOS_DE_ARAME_FARPADO "A10"
#define A11_VIVER_MORRER_E_ESQUECER "A11"
#define A12_ME_ESCOLHA "A12"
#define A13_O_PERDAO "A13"
#define A14_CHUVA_INOCENTE "A14"
#define A15_TODO_TEMPO "A15"
#define A16_EXODO "A16"

// Covers
#define B02_MYSTERIOUS_WAYS "B02"
#define B03_IVE_BEEN_LOSING_YOU "B03"
#define B05_SAVE_A_PRAYER "B05"
#define B06_WICKED_GAME "B06"
#define B08_LOVE_WILL_TEAR_US_APART "B08"
#define B10_BITTER_SWEET_SYMPHONY "B10"
#define B11_CARTA_AOS_MISSIONARIOS "B11"
#define B13_INDIOS "B13"
#define B15_ENJOY_THE_SILENCE "B15"
#define B17_BACK_ON_THE_CHAIN_GANG "B17"
#define B18_LIPS_LIKE_SUGAR "B18"
#define B20_JUST_LIKE_HEAVEN "B20"
#define B22_JUVENILIA "B22"
#define B24_WHERE_IS_MY_MIND "B24"
#define B25_MEUS_BONS_AMIGOS "B25"
#define B26_CRUSH_WITH_EYELINER "B26"
#define B27_DONT_YOU_FORGET_ABOUT_ME "B27"
#define B28_WONDERWALL "B28"
#define B29_DANIEL_NA_COVA_DOS_LEOES "B29"
#define B30_NEVER_TEAR_US_APART "B30"
#define B31_BLINDING_LIGHTS "B31"
#define B32_THE_ONE_I_LOVE "B32"
#define B33_WITH_OR_WITHOUT_YOU "B33"
#define B34_ONE "B34"
#define B35_UNCHAINED_MELODY "B35"
#define B36_THE_END_OF_THE_WORLD "B36"
#define B37_NEW_YEARS_DAY "B37"
#define B38_ATE_QUANDO_ESPERAR "B38"
#define B40_SIDE "B40"
#define B41_DANCING_IN_THE_DARK "B41"
#define B42_ONLY_HAPPY_WHEN_IT_RAINS "B42"
#define B43_REPTILIA "B43"
#define B44_SHE_SELLS_SANCTUARY "B44"
#define B45_ENVELHECO_NA_CIDADE "B45"
#define B46_SOMEBODY_TOLD_ME "B46"
#define B47_STOP_CRYING_YOUR_HEART_OUT "B47"
#define B48_HEROES "B48"
#define B49_TAKE_ME_OUT "B49"
#define B50_SUEDEHEAD "B50"

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
