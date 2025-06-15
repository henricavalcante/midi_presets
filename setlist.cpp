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
    27,
    { "A01",
      "B38",
      "B03",
      "B11",
      "B05",
      "B40",
      "A01",
      "B28",
      "B24",
      "B22",
      "B15",
      "B02",
      "B34",
      "A01",
      "B36",
      "B37",
      "B25",
      "A01",
      "B31",
      "B29",
      "B06",
      "B20",
      "B30",
      "A10",
      "A01",
      "B49",
      "B27" } },
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
