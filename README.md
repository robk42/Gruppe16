## [Inhaltsverzeichnis](#inhaltsverzeichnis)

- [Features](#features)
- [Dependencies](#dependencies)
- [Installation und Build](#installation-und-build)
- [Nutzung](#nutzung)
- [Erklärung der Module](#erklärung-der-module)
- [Erweiterungsmöglichkeiten](#erweiterungsmöglichkeiten)

## [Features](#features)

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
  - Anzeige und Aktualisierung von Statistiken (Siege, Niederlagen, Unentschieden) – dabei werden Server und Client jeweils korrekt berücksichtigt.
  - Interaktives Hauptmenü zur Auswahl der Modi (Singleplayer, Multiplayer – Server, Multiplayer – Client, Quit).

## [Dependencies](#dependencies)

\- **Linux:**    

Installiere `build-essential`, `libncurses5-dev` und `libncursesw5-dev` (z. B. über:  ```bash  sudo apt update  sudo apt install build-essential libncurses5-dev libncursesw5-dev

-\ **macOS:** 

brew install ncurses 

-\ **Windows:** 

unter WSL Ubuntu befolge die obige Anleitung für Linux



## [Installation und Build](#installation-und-build)

\### Linux / macOS / WSL 

1. **Repository klonen:** git clone https://github.com/robk42/Gruppe16.git   
2. **Change Direction zum Respository**: cd Gruppe16
3. **Projekt kompilieren**: 
   1. make clean
   2. make



## [Nutzung](#nutzung)

- **Start** : ./tic_tac_toe (Zum Öffnen des Hauptmenüs)
- **Start über Kommandozeilenargumente** : 
  - **Singleplayer** : ./tic_tac_toe .1
  - **Netzwerkmodus - Server** : ./tic_tac_toe -s 12345
  - **Netzwerkmodus - Client** : ./tic_tac_toe -c <server_ip> [12345]
    - Um deine Ip herauszufinden gebe in deinem Terminal **ifconfig** ein
    - Oder gehe auf https://www.whatsmyip.org/



## [Erklärung der Module](#erklärung-der-module)

- **Spiellogik** :
  - Das Spielfeld wird als eindimensionales Array (`board[9]`) dargestellt. Funktionen wie `initBoard()`, `checkWin()`, `checkDraw()`, `isValidMove()`, `makeMove()` und `getAIMove()` steuern die Spiellogik und den KI-Zug.
- **Statistiken** : 
  - Die Spielstatistiken (Siege, Niederlagen, Unentschieden) werden in der Datei `stats.txt` gespeichert. Die Funktionen `loadStats()`, `saveStats()` und `updateStats()` sorgen für das Laden, Speichern und Aktualisieren der Statistiken.
- **Netzwerk** :
  - Die Netzwerkmodule (`startServer()`, `connectToServer()`, `sendMove()`, `receiveMove()`) ermöglichen den Server-/Client-Modus über TCP/IP-Sockets.
- **GUI** :
  - Die Benutzeroberfläche wird über ncurses realisiert. Funktionen wie `initGUI()`, `printBoardNCurses()`, `getUserMoveNCurses()`, `askReplayNCurses()`, `winAnimation()` und `displayMessage()` steuern die Anzeige des Spielfelds, Eingabeaufforderungen und Animationen. Das interaktive Hauptmenü wird in `main.c` genutzt.
- **main** :
  - Die Hauptfunktion entscheidet über Kommandozeilenparameter oder das interaktive Menü, welchen Spielmodus der Benutzer startet, und koordiniert das Laden und Speichern der Statistiken.



## [Erweiterungsmöglichkeiten](#erweiterungsmöglichkeiten)

- **KI-Verbesserung** :
  - Eine weiterentwickelte KI, beispielsweise durch den Einsatz eines Minimax-Algorithmus, könnte integriert werden.
- **Netzwerkfeatures** :
  - Funktionen wie ein integrierter Chat oder das Speichern von Spielständen können zukünftig ergänzt werden.
- **UI-Optimierung** :
  - Das Hauptmenü und andere Teile der Benutzeroberfläche können weiter grafisch verfeinert werden.









