#include "music.h"

// Array of Music objects
static const Music musics[TOTAL_MUSICS] = {
  {
    "B0",
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
  },
  {
    "B1",
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
  },
  {
    "B2",
    "A-Ha",
    "I've been losing y",
    "C",
    100,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 2 }
    }
  },
  {
    "B3",
    "Montgomery",
    "Fugas e Lamentos",
    "C",
    100,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 3 }
    }
  },
  {
    "B4",
    "Duran Duran",
    "Save a prayer",
    "",
    113,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 4 }
    }
  },
  {
    "B5",
    "Chris Isaak",
    "Wicked Game",
    "B",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 5 }
    }
  },
  {
    "B6",
    "Montgomery",
    "Final de Tempestad",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 6 }
    }
  },
  {
    "B7",
    "Joy Division",
    "Love Will Tear Us",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 7 }
    }
  },
  {
    "B8",
    "Montgomery",
    "Estrada de Sonhos",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 8 }
    }
  },
  {
    "B9",
    "The Verve",
    "Bitter Sweet S.",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 9 }
    }
  },
  {
    "B10",
    "Uns e Outros",
    "Carta aos Missiona",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 10 }
    }
  },
  {
    "B11",
    "Montgomery",
    "Algum Lugar",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 11 }
    }
  },
  {
    "B12",
    "Legiao Urbana",
    "Indios",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 12 }
    }
  },
  {
    "B13",
    "Montgomery",
    "Pintura",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 13 }
    }
  },
  {
    "B14",
    "Depeche Mode",
    "Enjoy the Silence",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 14 }
    }
  },
  {
    "B15",
    "Montgomery",
    "A mesma poesia",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 15 }
    }
  },
  {
    "B16",
    "Montgomery",
    "Pinoquio",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 16 }
    }
  },
  {
    "B17",
    "Echo & The Bunny",
    "Lips Like Sugar",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 17 }
    }
  },
  {
    "B18",
    "Montgomery",
    "Até o sol nascer",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 18 }
    }
  },
  {
    "B19",
    "The Cure",
    "Just Like Heaven",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 19 }
    }
  },
  {
    "B20",
    "Montgomery",
    "Êxodo",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 20 }
    }
  },
  {
    "B21",
    "RPM",
    "Juvenilia",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 21 }
    }
  },
  {
    "B22",
    "Montgomery",
    "Beijos de Arame F",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 22 }
    }
  },
  {
    "B23",
    "Pixies",
    "Where is my mind",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 23 }
    }
  },
  {
    "B24",
    "Barão Vermelho",
    "Meus bons Amigos",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 24 }
    }
  },
  {
    "B25",
    "REM",
    "Crush With Eyeline",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 25 }
    }
  },
  {
    "B26",
    "Simple Minds",
    "Don't you ...",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 26 }
    }
  },
  {
    "B27",
    "Oasis",
    "Wonderwall",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 27 }
    }
  },
  {
    "B28",
    "Legião Urbana",
    "Daniel na cova",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 28 }
    }
  },
  {
    "B29",
    "INXS",
    "Never tear us apar",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 29 }
    }
  },
  {
    "B30",
    "The weeknd",
    "Blinding Lights",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 30 }
    }
  },
  {
    "B31",
    "REM",
    "The one I love",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 31 }
    }
  },
  {
    "B32",
    "U2",
    "New Year's Day",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 32 }
    }
  },
  {
    "B33",
    "Montgomery",
    "Viver Morrer e Esq",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 33 }
    }
  },
  {
    "B34",
    "Montgomery",
    "Me Escolha",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 34 }
    }
  },
  {
    "B35",
    "Montgomery",
    "O Perdão",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 35 }
    }
  },
  {
    "B36",
    "Montgomery",
    "Chuva Inocente",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 36 }
    }
  },
  {
    "B37",
    "Montgomery",
    "Todo Tempo",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 37 }
    }
  },
  {
    "B38",
    "Plebe Rude",
    "Ate quando esperar",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 38 }
    }
  },
  {
    "B40",
    "Travis",
    "Side",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 39 }
    }
  },
  {
    "B41",
    "Bruce Springsteen",
    "Dancing in the dar",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 40 }
    }
  },
  {
    "B42",
    "Garbage",
    "Only happy when it",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 41 }
    }
  },
  {
    "B43",
    "U2",
    "With or Without Yu",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 42 }
    }
  },
  {
    "B44",
    "The Cult",
    "She sells sanctuar",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 43 }
    }
  },
  {
    "B45",
    "Ira",
    "Envelheco Na Cidad",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 44 }
    }
  },
  {
    "B46",
    "The Killers",
    "Somebody Told Me",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 45 }
    }
  },
  {
    "B47",
    "Oasis",
    "Stop Crying",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 46 }
    }
  },
  {
    "B48",
    "David Bowie",
    "Heroes",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 47 }
    }
  },
  {
    "B49",
    "Franz Ferdinand",
    "Take Me Out",
    "",
    0,
    {
      { CH_NORD_LEAD, CC, 0, 0 },
      { CH_NORD_LEAD, CC, 32, 1 },
      { CH_NORD_LEAD, PC, 0, 48 }
    }
  }
};

static std::unordered_map<String, int> musicMap;
static bool mapInitialized = false;

void initMusicMap() {
  if (mapInitialized) {
    return;
  }

  for (int i = 0; i < TOTAL_MUSICS; i++) {
    String code = String(musics[i].code);
    musicMap[code] = i;
  }

  mapInitialized = true;
}

Music getMusic(int musicIndex) {
  if (musicIndex >= 0 && musicIndex < TOTAL_MUSICS) {
    return musics[musicIndex];
  }

  return musics[0];
}

Music getMusic(const String& key) {
  if (!mapInitialized) {
    initMusicMap();
  }

  if (musicMap.find(key) != musicMap.end()) {
    int index = musicMap[key];
    return musics[index];
  }

  return musics[0];
}
