# Tic Tac Toe – Netzwerk- und ncurses-basiertes Mehrspielerspiel

Dieses Projekt implementiert das klassische Tic Tac Toe-Spiel in C. Es bietet sowohl einen Singleplayer-Modus (Spieler vs. einfache KI) als auch einen Netzwerkmodus (Server/Client) und verwendet ncurses, um eine ansprechende, terminalbasierte Benutzeroberfläche zu realisieren. Zusätzlich werden Spielstatistiken (Wins, Losses, Draws) in der Datei `stats.txt` gespeichert, es gibt eine Replay-Funktion, eine Win-Animation und die Möglichkeit, das Spiel jederzeit mit dem Befehl `quit` zu beenden.

## Features

- **Singleplayer-Modus:**  
  - Spieler (X) vs. KI (O)
  - Eingabe der Züge als Zahl von 1 bis 9 (intern wird 1 subtrahiert, sodass 1 Feld 0 entspricht).
  - Möglichkeit, das Spiel jederzeit mit `quit` zu beenden.
  - Replay-Funktion: Nach Spielende wird abgefragt, ob ein weiteres Spiel gestartet werden soll.
  - Win-Animation: Bei einem Sieg blinkt ein Text, der den Gewinner anzeigt.
  - Speicherung der Spielstatistiken (Wins, Losses, Draws) in `stats.txt`.

- **Netzwerkmodus:**  
  - **Server-Modus:** Startet den Host, der den ersten Zug (als Spieler 'X') ausführt.
  - **Client-Modus:** Verbindet sich mit dem Server, um gemeinsam zu spielen (als 'O').
  - Kommunikation über TCP/IP-Sockets. (Achte bei Verbindungen über das Internet auf korrekte Netzwerkeinstellungen, z. B. Portweiterleitung und Firewallkonfiguration.)
  - Gleicher Spielablauf wie im Singleplayer-Modus, jedoch werden die Züge zwischen den Partnern synchronisiert.

- **ncurses-basierte GUI:**  
  - Darstellung des Spielbretts mit klaren Trennlinien.
  - Anzeige von Eingabeaufforderungen, Replay-Abfragen und Win-Animationen in Farbe.
  
- **Statistik-Modul:**  
  - Die aktuellen Statistiken werden beim Programmstart aus `stats.txt` geladen und beim Beenden wieder gespeichert.
  
## Systemanforderungen

- **Betriebssystem:**  
  - Linux, macOS oder Windows 10.  
  - Bei Windows wird empfohlen, das Windows Subsystem for Linux (WSL) zu verwenden oder alternativ PDCurses zu nutzen.
- **Compiler:**  
  - GCC (C99 oder höher)
- **Bibliotheken:**  
  - ncurses und libtinfo (auf Linux/macOS/WSL)  
  - Für Windows (native Ausführung) ggf. PDCurses (zusätzliche Anpassungen erforderlich)
- **Build-Tool:**  
  - Make (optional – alternativ kann auch gcc direkt verwendet werden)

## Projektstruktur

- **main.c:**  
  Enthält den Einstiegspunkt, die Auswahl des Modus (Singleplayer/Netzwerk) sowie die Replay-Schleifen und die Integration der Statistiken.
- **spiellogik.h / spiellogik.c:**  
  Enthält die Funktionen zur Spiellogik (Initialisierung, Gewinn-/Draw-Prüfung, Zugvalidierung, KI-Zug).
- **netzwerk.h / netzwerk.c:**  
  Enthält die Netzwerkfunktionen für den Server- und Client-Modus (Socket-Erstellung, Senden/Empfangen von Zügen).
- **gui.h / gui.c:**  
  Enthält alle ncurses-Funktionen (GUI-Initialisierung, Spielfeldanzeige, Benutzereingabe, Replay-Abfrage, Win-Animation).
- **stats.h / stats.c:**  
  Enthält Funktionen zum Laden, Aktualisieren und Speichern der Spielstatistiken.
- **Makefile:**  
  Automatisiert den Build-Prozess und kompiliert alle Module zu einer ausführbaren Datei (`tic_tac_toe`).

## Installation und Build

### Für Linux / macOS / WSL

1. **Repository klonen oder herunterladen:**

   ```bash
   git clone https://github.com/YourUsername/tic-tac-toe.git
   cd tic-tac-toe
