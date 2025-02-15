#ifndef STATS_H
#define STATS_H

/* Struktur zur Speicherung der Spielstatistiken */
typedef struct {
    int wins;
    int losses;
    int draws;
} Stats;

/* Globale Variable für die Statistiken */
extern Stats stats;

/* Lädt die Statistiken aus "stats.txt" (falls vorhanden) */
void loadStats(void);

/* Speichert die aktuellen Statistiken in "stats.txt" */
void saveStats(void);

/* Aktualisiert die Statistiken basierend auf dem Ergebnis:
   - 'X': Spieler gewinnt
   - 'O': KI gewinnt
   - 'D': Draw
*/
void updateStats(char result);

#endif  /* STATS_H */
