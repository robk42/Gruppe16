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

/// Globale Variable zur Speicherung der aktuellen Statistiken.
extern Stats stats;

/**
 * @brief Lädt die Statistiken aus der Datei "stats.txt".
 * Falls die Datei existiert, werden die gespeicherten Werte gelesen.
 */
void loadStats(void);

/**
 * @brief Speichert die aktuellen Statistiken in die Datei "stats.txt".
 */
void saveStats(void);

/**
 * @brief Aktualisiert die Statistiken basierend auf dem Spielergebnis.
 * @param result 'X' wenn der Spieler gewinnt, 'O' wenn die KI gewinnt, 'D' für Draw.
 */
void updateStats(char result);

#endif  /* STATS_H */
