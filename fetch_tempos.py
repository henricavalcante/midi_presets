#!/usr/bin/env python3
import os
import re
import time
import json
import urllib.request
import urllib.parse
import urllib.error
import ssl

# Optional imports for Spotify API
try:
    import spotipy
    from spotipy.oauth2 import SpotifyClientCredentials
    SPOTIFY_AVAILABLE = True
except ImportError:
    SPOTIFY_AVAILABLE = False
    print("Spotipy not installed. Will use alternative methods to fetch tempo.")

# Dictionary of known tempos for songs that might be hard to find online
KNOWN_TEMPOS = {
    "U2 - Mysterious Ways": 99,
    "A-ha - I've been losing you": 118,
    "Duran Duran - Save a Prayer": 113,  # Already in the file
    "Chris Isaac - Wicked Game": 79,
    "Joy Division - Love Will Tear Us Apart": 148,
    "The Verve - Bitter Sweet Symphony": 86,
    "Legião Urbana - Indios": 112,
    "Depeche Mode - Enjoy the Silence": 120,
    "Echo & The Bunnymen - Lips Like Sugar": 126,
    "The Cure - Just Like Heaven": 151,
    "Pixies - Where is my mind": 82,
    "REM - Crush With Eyeliner": 126,
    "Simple Minds - Don't you forget about me": 112,
    "Oasis - Wonderwall": 87,
    "INXS - Never tear us apart": 92,
    "The weeknd - Blinding Lights": 171,
    "REM - The one I love": 124,
    "U2 - With or Without You": 110,
    "U2 - One": 91,
    "U2 - New Year's Day": 135,
    "Travis - Side": 80,
    "Bruce Springsteen - Dancing in the dark": 149,
    "Garbage - Only happy when it rains": 120,
    "The Cult - She sells sanctuary": 126,
    "The Killers - Somebody Told Me": 138,
    "Oasis - Stop Crying Your Heart Out": 76,
    "David Bowie - Heroes": 116,
    "Franz Ferdinand - Take Me Out": 104
}

def get_spotify_client():
    """Initialize and return a Spotify client."""
    if not SPOTIFY_AVAILABLE:
        return None

    try:
        client_credentials_manager = SpotifyClientCredentials()
        return spotipy.Spotify(client_credentials_manager=client_credentials_manager)
    except Exception as e:
        print(f"Error initializing Spotify client: {e}")
        print("Make sure you've set SPOTIPY_CLIENT_ID and SPOTIPY_CLIENT_SECRET environment variables.")
        return None

def get_song_tempo_from_spotify(sp, artist, title):
    """Get the tempo (BPM) for a song using Spotify API."""
    if not sp:
        return 0

    try:
        # Search for the track
        query = f"artist:{artist} track:{title}"
        results = sp.search(q=query, type='track', limit=1)

        # Check if we found a track
        if results['tracks']['items']:
            track_id = results['tracks']['items'][0]['id']

            # Get audio features which include tempo
            audio_features = sp.audio_features(track_id)[0]
            if audio_features and 'tempo' in audio_features:
                return int(round(audio_features['tempo']))

        # If we didn't find the track or tempo, try a more general search
        query = f"{artist} {title}"
        results = sp.search(q=query, type='track', limit=1)

        if results['tracks']['items']:
            track_id = results['tracks']['items'][0]['id']
            audio_features = sp.audio_features(track_id)[0]
            if audio_features and 'tempo' in audio_features:
                return int(round(audio_features['tempo']))

        return 0
    except Exception as e:
        print(f"Error getting tempo from Spotify for {artist} - {title}: {e}")
        return 0

def get_song_tempo_from_songbpm(artist, title):
    """Get the tempo (BPM) for a song using SongBPM API."""
    try:
        # Create a context that doesn't verify SSL certificates
        ctx = ssl.create_default_context()
        ctx.check_hostname = False
        ctx.verify_mode = ssl.CERT_NONE

        # Encode the artist and title for the URL
        encoded_artist = urllib.parse.quote(artist)
        encoded_title = urllib.parse.quote(title)

        # Construct the URL for the SongBPM API
        url = f"https://songbpm.com/api/searches?q={encoded_artist}+{encoded_title}"

        # Make the request
        req = urllib.request.Request(url, headers={'User-Agent': 'Mozilla/5.0'})
        with urllib.request.urlopen(req, context=ctx) as response:
            data = json.loads(response.read().decode())

            # Check if we got results
            if data and 'searches' in data and data['searches']:
                for result in data['searches']:
                    if 'tempo' in result and result['tempo']:
                        return int(round(float(result['tempo'])))

        return 0
    except Exception as e:
        print(f"Error getting tempo from SongBPM for {artist} - {title}: {e}")
        return 0

def get_song_tempo(artist, title):
    """Get the tempo (BPM) for a song using multiple methods."""
    # First check if we have the tempo in our known tempos dictionary
    key = f"{artist} - {title}"
    if key in KNOWN_TEMPOS:
        print(f"Found tempo for {key} in known tempos: {KNOWN_TEMPOS[key]} BPM")
        return KNOWN_TEMPOS[key]

    # Try to get tempo from Spotify
    sp = get_spotify_client()
    tempo = get_song_tempo_from_spotify(sp, artist, title)
    if tempo > 0:
        print(f"Found tempo for {artist} - {title} from Spotify: {tempo} BPM")
        return tempo

    # Try to get tempo from SongBPM
    tempo = get_song_tempo_from_songbpm(artist, title)
    if tempo > 0:
        print(f"Found tempo for {artist} - {title} from SongBPM: {tempo} BPM")
        return tempo

    print(f"Could not find tempo for {artist} - {title}")
    return 0

def parse_music_cpp(file_path):
    """Parse the music.cpp file to extract song information."""
    with open(file_path, 'r') as f:
        content = f.read()

    songs = []
    # Pattern to match song entries in the music.cpp file
    pattern = r'{\s*"([^"]+)",\s*"([^"]+)",\s*"([^"]+)",\s*"[^"]*",\s*(\d+),'

    for match in re.finditer(pattern, content):
        code, artist, title, tempo = match.groups()
        songs.append({
            'code': code,
            'artist': artist,
            'title': title,
            'tempo': int(tempo)
        })

    return songs

def update_music_cpp(file_path, songs):
    """Update the music.cpp file with the new tempo values."""
    with open(file_path, 'r') as f:
        content = f.read()

    # Create a backup of the original file
    with open(f"{file_path}.bak", 'w') as f:
        f.write(content)

    # Update each song's tempo in the content
    for song in songs:
        if song['tempo'] > 0:  # Only update if we have a valid tempo
            # Pattern to match the specific song entry - more precise for the actual format
            pattern = rf'{{(\s*)"({re.escape(song["code"])})"\s*,(\s*)"({re.escape(song["artist"])})"\s*,(\s*)"({re.escape(song["title"])})"\s*,(\s*)"[^"]*"\s*,(\s*)(\d+),'
            replacement = rf'{{\1"{song["code"]}"{2}"{song["artist"]}"{4}"{song["title"]}"{6}"{7}{song["tempo"]},'

            # Try to replace with the pattern
            new_content = re.sub(pattern, replacement, content)

            # If the content didn't change, the pattern didn't match
            if new_content == content:
                print(f"Warning: Could not update tempo for {song['artist']} - {song['title']} (code: {song['code']})")
            else:
                content = new_content
                print(f"Updated tempo for {song['artist']} - {song['title']} to {song['tempo']} BPM")

    # Write the updated content back to the file
    with open(file_path, 'w') as f:
        f.write(content)

    print(f"Updated {file_path} with new tempo values.")

def main():
    """Main function to fetch tempos and update the music.cpp file."""
    # Get the music.cpp file path
    music_cpp_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "music.cpp")

    print("=" * 60)
    print("MIDI Presets Tempo Fetcher")
    print("=" * 60)
    print("This script will fetch tempo information for songs and update the music.cpp file.")
    print("It will try multiple sources: known tempos, Spotify API, and SongBPM website.")
    print("=" * 60)

    # Parse the music.cpp file
    songs = parse_music_cpp(music_cpp_path)
    print(f"Found {len(songs)} songs in {music_cpp_path}")

    # Count songs that already have tempo
    songs_with_tempo = sum(1 for song in songs if song['tempo'] > 0)
    print(f"{songs_with_tempo} songs already have tempo information.")
    print(f"{len(songs) - songs_with_tempo} songs need tempo information.")

    if songs_with_tempo == len(songs):
        print("All songs already have tempo information. Nothing to do.")
        return

    print("\nFetching tempo information...")

    # Fetch tempo for each song
    for song in songs:
        if song['tempo'] == 0:  # Only fetch if tempo is not already set
            print(f"\nFetching tempo for {song['artist']} - {song['title']}...")
            song['tempo'] = get_song_tempo(song['artist'], song['title'])
            # Add a small delay to avoid hitting API rate limits
            time.sleep(1)

    # Update the music.cpp file
    print("\nUpdating music.cpp file...")
    update_music_cpp(music_cpp_path, songs)

    # Print summary
    songs_with_tempo_after = sum(1 for song in songs if song['tempo'] > 0)
    print("\n" + "=" * 60)
    print(f"Summary: Updated {songs_with_tempo_after - songs_with_tempo} songs with new tempo information.")
    print(f"Total songs with tempo: {songs_with_tempo_after} out of {len(songs)}.")

    if songs_with_tempo_after < len(songs):
        print("\nSome songs still don't have tempo information.")
        print("You can add them manually to the KNOWN_TEMPOS dictionary in this script.")
    else:
        print("\nAll songs now have tempo information!")

    print("=" * 60)

if __name__ == "__main__":
    main()
