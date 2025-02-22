# Tic Tac Toe – Netzwerk- und ncurses-basiertes Mehrspielerspiel

Dieses Projekt implementiert das klassische Tic Tac Toe-Spiel in C. Es bietet sowohl einen **Singleplayer-Modus** (Spieler vs. einfache KI) als auch einen **Netzwerkmodus** (Server/Client) und verwendet [ncurses](https://invisible-island.net/ncurses/) zur Erstellung einer terminalbasierten Benutzeroberfläche. Zusätzlich werden Spielstatistiken (Siege, Niederlagen, Unentschieden) in der Datei `stats.txt` gespeichert und zwischen den Spielrunden aktualisiert. Ein interaktives Hauptmenü erleichtert die Modusauswahl.

---

## Inhaltsverzeichnis

- [Features](#features)
- [Dependencies](#dependencies)
- [Installation und Build](#installation-und-build)
- [Nutzung](#nutzung)
- [Erklärung der Module](#erklärung-der-module)
- [Erweiterungsmöglichkeiten](#erweiterungsmöglichkeiten)
- [Fazit](#fazit)

---

## Features

- **Grundfunktionen:**
  - Anzeige eines 3x3-Spielfelds, wobei leere Zellen numerisch (1–9) beschriftet sind.
  - Eingabe der Spielzüge über Zahlen (1–9) oder durch Eingabe von `quit` zum Beenden.
  - Automatische Überprüfung der Gewinnbedingungen und Erkennung von Unentschieden.

- **Netzwerkfunktionalität:**
  - Realisierung eines Server-/Client-Modus über TCP/IP-Sockets.
  - Echtzeit-Synchronisation der Spielzüge zwischen Host (Server) und Client.
  - Bei Fehlern im Netzwerkmodus (beim Senden oder Empfangen) wird das Spiel beendet.

- **Zusatzfunktionen:**
  - Singleplayer-Modus mit einer einfachen, zufälligen KI.
  - Anzeige und Aktualisierung von Statistiken (Siege, Niederlagen, Unentschieden) – wobei Server und Client ihre Ergebnisse jeweils korrekt erhalten.
  - Interaktives Hauptmenü zur Auswahl der Modi (Singleplayer, Multiplayer – Server, Multiplayer – Client und Quit).
  - Der Eingabe-Prompt weist explizit darauf hin, dass nur leere Zellen auswählbar sind.

---

## Dependencies

- **Linux:**  
  Installiere `build-essential`, `libncurses5-dev` und `libncursesw5-dev` (z. B. über `sudo apt install build-essential libncurses5-dev libncursesw5-dev`).

- **macOS:**  
  Die meisten ncurses-Komponenten sind vorinstalliert. Alternativ kannst Du [Homebrew](https://brew.sh/) nutzen:  
  ```bash
  brew install ncurses
  **Windows (10/11):**  
Empfohlen wird die Nutzung des Windows Subsystems for Linux (WSL).
## Installation und Build

### Linux / macOS / WSL

**Repository klonen:**
```bash
git clone https://github.com/robk42/Gruppe16.git
cd to repository
bhängigkeiten installieren:

Linux/WSL (Ubuntu):
sudo apt update
sudo apt install build-essential libncurses5-dev libncursesw5-dev

macOS: Nutze Homebrew, falls erforderlich:
brew install ncurses

**Projekt kompilieren:**
make clean
make

Nutzung
Über das interaktive Hauptmenü
Start:
Führe das Programm ohne Kommandozeilenparameter aus:

./tic_tac_toe
Hauptmenü:
Das Menü zeigt ein Banner in Orange und folgende Optionen an:

Singleplayer (Player vs. AI): Starte ein Spiel gegen die integrierte KI.
Multiplayer – Server: Starte den Server, der auf eingehende Verbindungen wartet.
Multiplayer – Client: Verbinde Dich als Client zu einem laufenden Server. (Falls keine IP übergeben wird, wirst Du zur Eingabe aufgefordert.)
Quit: Beendet das Programm.
Wähle eine Option durch Drücken der entsprechenden Zahl (1–4).

Über Kommandozeilenargumente
Singleplayer-Modus:
./tic_tac_toe -1
Netzwerkmodus – Server:
./tic_tac_toe -s [port]
(Ohne Port wird Standardport 12345 verwendet.)

Netzwerkmodus – Client:
./tic_tac_toe -c <server_ip> [port]
Ersetze <server_ip> durch die IP-Adresse des Hosts. Wird kein Port angegeben, verwendet das Programm 12345.

Erklärung der Module
Spiellogik:
Das Spielfeld wird als eindimensionales Array (board[9]) dargestellt. Funktionen wie initBoard(), checkWin(), checkDraw(), isValidMove(), makeMove() und getAIMove() steuern die Spiellogik und den KI-Zug.

Statistiken:
Die Spielstatistiken (Siege, Niederlagen, Unentschieden) werden in stats.txt gespeichert. Funktionen loadStats(), saveStats() und updateStats() sorgen für das Laden, Speichern und Aktualisieren der Statistiken.

Netzwerk:
Die Netzwerkmodule (startServer(), connectToServer(), sendMove(), receiveMove()) ermöglichen den Server-/Client-Modus über TCP/IP-Sockets.

GUI:
Die Benutzeroberfläche wird über ncurses realisiert. Funktionen wie initGUI(), printBoardNCurses(), getUserMoveNCurses(), askReplayNCurses(), winAnimation() und displayMessage() steuern die Anzeige des Spielfelds, Eingabeaufforderungen und Animationen. Das interaktive Hauptmenü wird in main.c genutzt.

main:
Die Hauptfunktion entscheidet über Kommandozeilenparameter oder das interaktive Menü, welchen Spielmodus der Benutzer startet, und koordiniert das Laden und Speichern der Statistiken.

Erweiterungsmöglichkeiten
KI-Verbesserungen:
Eine weiterentwickelte KI, beispielsweise durch den Einsatz eines Minimax-Algorithmus, könnte integriert werden.

Netzwerkfeatures:
Funktionen wie ein integrierter Chat oder das Speichern von Spielständen können zukünftig ergänzt werden.

UI-Optimierung:
Das Hauptmenü und andere Teile der Benutzeroberfläche können weiter grafisch verfeinert werden.





