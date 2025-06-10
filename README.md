# MIDI Presets

This project is an Arduino-based MIDI controller with an LCD display for managing song presets.

## Songs
| Code | Song                       | Artist              |
|------|----------------------------|---------------------|
| A01  | Frente Ao Sol              | Montgomery          |
| A02  | Fugas e Lamentos           | Montgomery          |
| A03  | Final de Tempestade        | Montgomery          |
| A04  | Estrada de Sonhos          | Montgomery          |
| A05  | Algum Lugar                | Montgomery          |
| A06  | Pintura                    | Montgomery          |
| A07  | A mesma poesia             | Montgomery          |
| A08  | Pinoquio                   | Montgomery          |
| A09  | Até o sol nascer           | Montgomery          |
| A10  | Beijos de Arame Farpado    | Montgomery          |
| A11  | Viver Morrer e Esquecer    | Montgomery          |
| A12  | Me Escolha                 | Montgomery          |
| A13  | O Perdão                   | Montgomery          |
| A14  | Chuva Inocente             | Montgomery          |
| A15  | Todo Tempo                 | Montgomery          |
| A16  | Êxodo                      | Montgomery          |
| B02  | Mysterious Ways            | U2                  |
| B03  | I've been losing you       | (A-ha)              |
| B05  | Save a Prayer              | Duran Duran         |
| B06  | Wicked Game                | Chris Isaac         |
| B08  | Love Will Tear Us Apart    | Joy Division        |
| B10  | Bitter Sweet Symphony      | The Verve           |
| B11  | Carta aos Missionários     | Uns e Outros        |
| B13  | Indios                     | Legião Urbana       |
| B15  | Enjoy the Silence          | Depeche Mode        |
| B17  | Back On The Chain Gang     | The Pretenders      |
| B18  | Lips Like Sugar            | Echo & The Bunnymen |
| B20  | Just Like Heaven           | The Cure            |
| B22  | Juvenilia                  | RPM                 |
| B24  | Where is my mind           | Pixies              |
| B25  | Meus bons Amigos           | Barão Vermelho      |
| B26  | Crush With Eyeliner        | REM                 |
| B27  | Don't you forget about me  | Simple Minds        |
| B28  | Wonderwall                 | Oasis               |
| B29  | Daniel na cova dos leões   | Legião Urbana       |
| B30  | Never tear us apart        | INXS                |
| B31  | Blinding Lights            | The weeknd          |
| B32  | The one I love             | REM                 |
| B33  | With or Without You        | U2                  |
| B34  | One                        | U2                  |
| B36  | The end of the world       | U2                  |
| B37  | New Year's Day             | U2                  |
| B39  | Até quando esperar         | Prebe Rude          |
| B40  | Side                       | Travis              |
| B41  | Dancing in the dark        | Bruce Springsteen   |
| B42  | Only happy when it rains   | Garbage             |
| B44  | She sells sanctuary        | The Cult            |
| B45  | Envelheco Na Cidade        | Ira                 |
| B46  | Somebody Told Me           | The Killers         |
| B47  | Stop Crying Your Heart Out | Oasis               |
| B48  | Heroes                     | David Bowie         |
| B49  | Take Me Out                | Franz Ferdinand     |
| B50  | Suedhead                   | Morrissey           |

## Fetching Song Tempos

The project includes a Python script (`fetch_tempos.py`) that can automatically fetch tempo information for songs from the internet and update the `music.cpp` file.

### Requirements

- Python 3.6 or higher
- (Optional) Spotify API credentials for more accurate tempo information

### Installation

1. (Optional) Install the Spotify API client:
   ```
   pip install spotipy
   ```

2. (Optional) Set up Spotify API credentials:
   ```
   export SPOTIPY_CLIENT_ID='your-spotify-client-id'
   export SPOTIPY_CLIENT_SECRET='your-spotify-client-secret'
   ```
   You can get these credentials by creating an app on the [Spotify Developer Dashboard](https://developer.spotify.com/dashboard/).

### Usage

1. Run the script:
   ```
   python fetch_tempos.py
   ```

2. The script will:
   - Parse the `music.cpp` file to find songs without tempo information
   - Try to fetch tempo information from multiple sources:
     - A built-in dictionary of known tempos
     - Spotify API (if available)
     - SongBPM website
   - Update the `music.cpp` file with the new tempo values
   - Create a backup of the original file (`music.cpp.bak`)

3. If some songs still don't have tempo information after running the script, you can manually add them to the `KNOWN_TEMPOS` dictionary in the script.