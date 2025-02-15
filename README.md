# Tic Tac Toe – Netzwerk- und ncurses-basiertes Mehrspielerspiel

Dieses Projekt implementiert das klassische Tic Tac Toe-Spiel in C. Es bietet einen Singleplayer-Modus (Spieler vs. einfache KI) sowie einen Netzwerkmodus (Server/Client) und nutzt ncurses zur Darstellung in der Konsole. Zusätzlich werden Spielstatistiken (Wins, Losses, Draws) in einer Datei gespeichert, und es gibt Funktionen wie Replay, Win-Animation sowie eine Möglichkeit, das Spiel jederzeit mit dem Befehl "quit" zu beenden.

## Features

- **Singleplayer-Modus:**  
  - Spieler (X) vs. KI (O)
  - Intuitive Eingabe: Der Benutzer gibt eine Zahl von 1 bis 9 ein, die intern auf die Felder 0–8 abgebildet wird.
  - Replay-Funktion: Nach Spielende wird abgefragt, ob ein neues Spiel gestartet werden soll.
  - Win-Animation: Bei einem Gewinn blinkt eine Animation, die den Sieger anzeigt.
  - Möglichkeit, das Spiel jederzeit mit der Eingabe "quit" zu verlassen.

- **Netzwerkmodus:**  
  - Server-Modus: Startet den Host, der den ersten Zug (als 'X' im Singleplayer-Schema) ausführt.
  - Client-Modus: Verbindet sich mit dem Host, um gemeinsam zu spielen.
  - Verwendung von TCP/IP-Sockets zur Übertragung der Spielzüge.
  - Gleicher Spielablauf wie im Singleplayer-Modus, nur dass die Züge über das Netzwerk synchronisiert werden.

- **Spielstatistiken:**  
  - Wins, Losses und Draws werden in der Datei `stats.txt` gespeichert.
  - Die Statistiken werden nach jedem Spiel aktualisiert und in der GUI angezeigt.

- **ncurses-GUI:**  
  - Konsolenbasierte, farbige Darstellung des Spielfelds.
  - Kompakte Darstellung des Grids mit Trennlinien.
  - Fehler- und Eingabeaufforderungen werden in der GUI angezeigt.

## Systemanforderungen

- **Betriebssystem:**  
  - Linux, macOS oder Windows 10 (empfohlen über WSL oder mit einer entsprechenden ncurses-Alternative, z.B. PDCurses)
- **Compiler:**  
  - GCC (C99 oder höher)
- **Bibliotheken:**  
  - ncurses, libtinfo (oder PDCurses auf Windows, falls nicht über WSL)
- **Build-Tool:**  
  - Make (oder gcc direkt)

## Installation und Build

### Unter Linux / macOS / WSL

1. **Repository klonen oder Dateien herunterladen:**

   ```bash
   git clone https://github.com/DeinBenutzername/tic-tac-toe.git
   cd tic-tac-toe
