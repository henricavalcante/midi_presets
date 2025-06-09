#include "music.h"

// Array of Music objects
static const Music musics[TOTAL_MUSICS] = {
  {
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
    "Legião Urbana",
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

Music getMusic(int musicIndex) {
  // Check if the index is within bounds
  if (musicIndex >= 0 && musicIndex < TOTAL_MUSICS) {
    return musics[musicIndex];
  }

  // Return the first music as a fallback if index is out of bounds
  return musics[0];
}
