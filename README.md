# Gruppe16
Programmentwurf Tic Tac Toe


# Tic Tac Toe mit Netzwerkunterstützung, ncurses-GUI und Replay-Funktion

Dieses Projekt implementiert das klassische Tic Tac Toe-Spiel in C. Neben der Basis-Spiellogik werden folgende Kernfeatures geboten:

- **Multimodus-Unterstützung:**  
  - **Einzelspielermodus:** Spieler vs. einfache KI  
  - **Multiplayer-Modus:** Über TCP/IP (Server-/Client-Modell)

- **Textbasierte grafische Oberfläche (TUI):**  
  Mittels ncurses wird ein farbiges, strukturiertes Spielbrett in Deinem Terminal dargestellt.

- **Replay-Funktion:**  
  Nach Beendigung eines Spiels kann per Eingabe eine erneute Runde gestartet werden, ohne dass das Programm neu gestartet werden muss.

- **Persistente Spielstatistiken:**  
  Siege, Niederlagen und Unentschieden werden in der Datei `stats.txt` gespeichert und nach jedem Spiel angezeigt.

## Inhaltsverzeichnis

- [Überblick](#überblick)
- [Features](#features)
- [Systemanforderungen](#systemanforderungen)
- [Installation und Build](#installation-und-build)
- [Nutzung](#nutzung)
- [Projektstruktur](#projektstruktur)
- [Autoren](#autoren)
- [Lizenz](#lizenz)
- [Zukünftige Erweiterungen](#zukünftige-erweiterungen)

## Überblick

Dieses Projekt realisiert ein Tic Tac Toe-Spiel in drei verschiedenen Modi:

- **Einzelspielermodus:**  
  Spiele gegen eine einfache KI.

- **Multiplayer-Modus:**  
  - **Server:** Starte das Spiel als Host und warte auf einen Client.
  - **Client:** Verbinde Dich mit einem laufenden Server.

Die Darstellung erfolgt über ncurses – das heißt, die grafische Benutzeroberfläche wird direkt im Terminal realisiert und zeigt farblich hervorgehobene Spielzüge, Spielfeldlinien und Meldungen an.

## Features

- **Spielregeln:**  
  Klassisches 3x3 Tic Tac Toe mit Gewinn- und Unentschieden-Überprüfung.
  
- **Netzwerkunterstützung:**  
  Kommunikation zwischen Server und Client über TCP/IP-Sockets.

- **Farbige Darstellung:**  
  - **X:** Wird in Rot dargestellt  
  - **O:** Wird in Blau dargestellt  
  - **Spielfeldlinien:** Werden in Grün gezeichnet

- **Replay-Funktion:**  
  Nach Spielende wird abgefragt, ob ein weiteres Spiel gestartet werden soll (sowohl im Einzelspieler- als auch im Multiplayer-Modus).

- **Spielstatistiken:**  
  Die Anzahl der Siege, Niederlagen und Unentschieden wird in `stats.txt` gespeichert und angezeigt.

## Systemanforderungen

- **Betriebssystem:** Linux/Unix (Terminalbasiert)
- **Compiler:** GCC (C99 oder höher, getestet z.B. mit GCC 14)
- **Bibliotheken:** ncurses, libtinfo
- **Build-Tool:** make

## Installation und Build

1. **Repository klonen oder Dateien herunterladen:**

   ```sh
   git clone https://github.com/robk42/tic-tac-toe-network.git
   cd tic-tac-toe-network
