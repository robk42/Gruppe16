#ifndef STATS_H
#define STATS_H

/**
 * @brief Struktur zur Speicherung der Spielstatistiken.
 */
typedef struct {
    int wins;    ///< Anzahl der Siege des Spielers.
    int losses;  ///< Anzahl der Niederlagen (Gewinne der KI).
    int draws;   ///< Anzahl der Unentschieden.
} Stats;

/* Globale Variable für die Statistiken. */
extern Stats stats;

/**
 * @brief Lädt die Statistiken aus der Datei "stats.txt".
 */
void loadStats(void);

/**
 * @brief Speichert die aktuellen Statistiken in die Datei "stats.txt".
 */
void saveStats(void);

/**
 * @brief Aktualisiert die Statistiken basierend auf dem Ergebnis.
 * @param result 'X' für Spielergewinn, 'O' für KI-Gewinn, 'D' für Unentschieden.
 */
void updateStats(char result);

#endif  /* STATS_H */
