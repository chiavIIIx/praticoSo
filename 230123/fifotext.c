#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    // 1. Controllo argomenti
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <pathname_fifo>\n", argv[0]);
        return 1;
    }

    char *fifo_path = argv[1];

    // 2. Creazione della FIFO (se non esiste già)
    // 0666 sono i permessi di lettura/scrittura
    if (mkfifo(fifo_path, 0666) == -1) {
        perror("Errore creazione FIFO (o esiste già)");
    }

    char buffer[1024];

    // 3. Ciclo infinito: apri, leggi, chiudi e riapri
    while (1) {
        // L'apertura in sola lettura (O_RDONLY) è bloccante:
        // il programma si ferma qui finché qualcuno non apre la pipe in scrittura
        int fd = open(fifo_path, O_RDONLY);
        if (fd == -1) {
            perror("Errore apertura FIFO");
            break;
        }

        // Leggiamo riga per riga dalla FIFO usando fdopen per poter usare fgets
        FILE *fifo_file = fdopen(fd, "r");
        
        while (fgets(buffer, sizeof(buffer), fifo_file) != NULL) {
            // Rimuoviamo il carattere newline (\n) se presente
            buffer[strcspn(buffer, "\n")] = 0;

            // Controllo se la riga è "FINE"
            if (strcmp(buffer, "FINE") == 0) {
                printf("Ricevuto FINE. Chiusura in corso...\n");
                fclose(fifo_file);
                unlink(fifo_path); // Cancella il file della FIFO
                return 0;
            }

            // Stampa la riga ricevuta
            printf("%s\n", buffer);
        }

        // Se arriviamo qui, lo scrittore ha chiuso la pipe.
        // Chiudiamo il descrittore attuale e il ciclo ripartirà dal comando open()
        fclose(fifo_file);
    }

    return 0;
}