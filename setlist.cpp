#include "setlist.h"
#include <unordered_map>

static const Setlist setlists[MAX_SETLISTS] = {
  { "Wesleys",
    "2025-06-15",
    3,
    { "B1",
      B02_MYSTERIOUS_WAYS,
      "B4" } },
  { "Iate Clube",
    "2025-06-22",
    27,
    { A01_FRENTE_AO_SOL,
      B38_ATE_QUANDO_ESPERAR,
      B03_IVE_BEEN_LOSING_YOU,
      B11_CARTA_AOS_MISSIONARIOS,
      B05_SAVE_A_PRAYER,
      B40_SIDE,
      B17_BACK_ON_THE_CHAIN_GANG,
      B28_WONDERWALL,
      B24_WHERE_IS_MY_MIND,
      B22_JUVENILIA,
      B15_ENJOY_THE_SILENCE,
      B02_MYSTERIOUS_WAYS,
      B34_ONE,
      B35_UNCHAINED_MELODY,
      B36_THE_END_OF_THE_WORLD,
      B37_NEW_YEARS_DAY,
      B25_MEUS_BONS_AMIGOS,
      B43_REPTILIA,
      B31_BLINDING_LIGHTS,
      B29_DANIEL_NA_COVA_DOS_LEOES,
      B06_WICKED_GAME,
      B20_JUST_LIKE_HEAVEN,
      B30_NEVER_TEAR_US_APART,
      A10_BEIJOS_DE_ARAME_FARPADO,
      B50_SUEDEHEAD,
      B49_TAKE_ME_OUT,
      B27_DONT_YOU_FORGET_ABOUT_ME } },
  { "Titan",
    "2025-07-01",
    4,
    { "B0",
      "B1",
      B03_IVE_BEEN_LOSING_YOU,
      "B4" } }
};

static const int SETLIST_COUNT = 3;

static std::unordered_map<String, int> setlistMap;

void initSetlists() {
  for (int i = 0; i < SETLIST_COUNT; i++) {
    String key = String(i);
    setlistMap[key] = i;
  }
}

int getSetlistCount() {
  return SETLIST_COUNT;
}

Setlist getSetlist(int setlistIndex) {
  if (setlistIndex >= 0 && setlistIndex < SETLIST_COUNT) {
    return setlists[setlistIndex];
  }

  Setlist emptySetlist;
  strcpy(emptySetlist.title, "");
  strcpy(emptySetlist.date, "");
  emptySetlist.songCount = 0;
  return emptySetlist;
}

Music getSetlistSong(int setlistIndex, int songIndex) {
  if (setlistIndex >= 0 && setlistIndex < SETLIST_COUNT) {
    const Setlist& setlist = setlists[setlistIndex];
    if (songIndex >= 0 && songIndex < setlist.songCount) {
      String songCode = String(setlist.songCodes[songIndex]);
      return getMusic(songCode);
    }
  }

  return getMusic(0);
}
