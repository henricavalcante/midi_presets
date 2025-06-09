#include "setlist.h"
#include <unordered_map>

static const Setlist setlists[MAX_SETLISTS] = {
  { "Wesleys",
    "2025-06-15",
    3,
    { "B1",
      "B2",
      "B4" } },
  { "Iate Clube",
    "2025-06-22",
    2,
    { "B0",
      "B3" } },
  { "Titan",
    "2025-07-01",
    4,
    { "B0",
      "B1",
      "B3",
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