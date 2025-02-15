#ifndef NETZWERK_H
#define NETZWERK_H

/* Startet einen Server, der auf dem angegebenen Port auf eine Verbindung wartet.
   Liefert den Socket, der die Verbindung repräsentiert. */
int startServer(int port);

/* Baut als Client eine Verbindung zu einem Server auf.
   Liefert den Socket, der die Verbindung repräsentiert. */
int connectToServer(const char *ip, int port);

/* Sendet einen Zug (als Integer) über den Socket. */
int sendMove(int sock, int move);

/* Empfängt einen Zug (als Integer) über den Socket und wandelt ihn um. */
int receiveMove(int sock, int *move);

#endif  /* NETZWERK_H */
