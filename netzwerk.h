#ifndef NETZWERK_H
#define NETZWERK_H

/**
 * @brief Startet den Server und wartet auf eine Verbindung.
 * @param port Der Port, auf dem der Server lauscht.
 * @return Den Socket, der die Verbindung zum Client repräsentiert.
 */
int startServer(int port);

/**
 * @brief Baut eine Verbindung zu einem Server auf.
 * @param ip Die IP-Adresse des Servers.
 * @param port Der Port, auf dem der Server lauscht.
 * @return Den Socket, der die Verbindung repräsentiert.
 */
int connectToServer(const char *ip, int port);

/**
 * @brief Sendet einen Zug (als Integer) über den Socket.
 * @param sock Der Socket, über den gesendet wird.
 * @param move Der Zug (als Integer).
 * @return 0 bei Erfolg, -1 bei Fehler.
 */
int sendMove(int sock, int move);

/**
 * @brief Empfängt einen Zug (als Integer) über den Socket.
 * @param sock Der Socket, von dem empfangen wird.
 * @param move Pointer, unter dem der empfangene Zug gespeichert wird.
 * @return 0 bei Erfolg, -1 bei Fehler.
 */
int receiveMove(int sock, int *move);

#endif  /* NETZWERK_H */
