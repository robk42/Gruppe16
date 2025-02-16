# Tic Tac Toe – Netzwerk- und ncurses-basiertes Mehrspielerspiel

Dieses Projekt implementiert das klassische Tic Tac Toe-Spiel in C. Es bietet einen Singleplayer-Modus (Spieler vs. einfache KI) sowie einen Netzwerkmodus (Server/Client) und verwendet **ncurses** für eine ansprechende, terminalbasierte Benutzeroberfläche. Zusätzlich werden Spielstatistiken (Wins, Losses, Draws) in der Datei `stats.txt` gespeichert, es gibt eine Replay-Funktion, eine Win-Animation und die Möglichkeit, das Spiel jederzeit durch Eingabe von `quit` zu beenden.

---

## Features

- **Singleplayer-Modus**
  - Spieler (X) vs. KI (O)
  - Eingabe der Züge als Zahl von **1 bis 9** (intern wird 1 subtrahiert, sodass 1 dem Feldindex 0 entspricht)
  - Möglichkeit, das Spiel jederzeit mit `quit` zu beenden
  - Replay-Funktion: Nach Spielende wird abgefragt, ob ein weiteres Spiel gestartet werden soll
  - Win-Animation: Bei einem Sieg blinkt ein Text, der den Gewinner anzeigt
  - Speicherung der Spielstatistiken in `stats.txt`

- **Netzwerkmodus**
  - **Server-Modus:** Startet den Host (spielt als 'X') und wartet auf eine Verbindung
  - **Client-Modus:** Verbindet sich mit dem Server (spielt als 'O')
  - Kommunikation über TCP/IP-Sockets
  - Bei Verbindungsverlust versucht der Client, sich innerhalb von 30 Sekunden (6 Versuche à 5 Sekunden) neu zu verbinden; scheitert dies, gewinnt der Host

- **ncurses-basierte GUI**
  - Darstellung des Spielfelds mit klaren Trennlinien
  - Leere Felder werden numerisch von 1 bis 9 angezeigt, besetzte Felder zeigen X oder O
  - Anzeige von Eingabeaufforderungen, Replay-Abfragen und Win-Animationen in Farbe

- **Statistik-Modul**
  - Die aktuellen Spielstatistiken werden beim Programmstart aus `stats.txt` geladen und beim Beenden aktualisiert und gespeichert

---

## Systemanforderungen

- **Betriebssystem:**  
  Linux, macOS oder Windows 10  
  - **Hinweis:** Unter Windows wird empfohlen, das Windows Subsystem for Linux (WSL) zu verwenden oder alternativ PDCurses einzusetzen (dies erfordert ggf. zusätzliche Anpassungen im Code)
- **Compiler:**  
  GCC (C99 oder höher)
- **Benötigte Bibliotheken:**  
  ncurses und libtinfo (auf Linux/macOS/WSL)
- **Build-Tool:**  
  Make (alternativ kann auch gcc direkt verwendet werden)

---

## Projektstruktur

- **main.c**  
  Enthält den Einstiegspunkt, die Auswahl des Modus (Singleplayer/Netzwerk) sowie Replay-Logik und die Integration der Statistiken.
- **spiellogik.h / spiellogik.c**  
  Enthält die Spiellogik: Initialisierung, Gewinn-/Draw-Prüfung, Zugvalidierung, KI-Zug.
- **netzwerk.h / netzwerk.c**  
  Enthält die Netzwerkfunktionen für den Server- und Client-Modus (Socket-Erstellung, Senden/Empfangen von Zügen, Wiederverbindungsversuch).
- **gui.h / gui.c**  
  Enthält alle ncurses-bezogenen Funktionen: GUI-Initialisierung, Spielfeldanzeige, Benutzereingabe, Replay-Abfrage, Win-Animation, Anzeige von Nachrichten.
- **stats.h / stats.c**  
  Enthält Funktionen zum Laden, Aktualisieren und Speichern der Spielstatistiken.
- **Makefile**  
  Automatisiert den Build-Prozess und kompiliert alle Module zu einer ausführbaren Datei namens `tic_tac_toe`.

---

## Installation und Build

### Für Linux / macOS / WSL

1. **Repository klonen oder herunterladen:**

   ```bash
   git clone https://github.com/YourUsername/tic-tac-toe.git
   cd tic-tac-toe

2: **Benötigte Pakete installieren (Beispiel für Ubuntu/WSL):**
    sudo apt update
    sudo apt install build-essential libncurses5-dev libncursesw5-dev

3: ** Projekt kompilieren: **

      make clean
      make

4: ** Spielmodianleitung **
      ./tic_tac_toe --help
