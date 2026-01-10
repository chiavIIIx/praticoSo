#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> // Per operazioni su directory
#include <sys/stat.h> // Per stat e lstat
#include <unistd.h> // Per mkdir e symlink
#include <limits.h> // Per PATH_MAX

// Funzione per calcolare lo SHA1 di un file usando il comando di sistema "sha1sum"
// Restituisce la stringa dell'hash
void get_sha1(const char *path, char *output) {
    
    char command[PATH_MAX + 20]; // path_max + spazio per "sha1sum " e virgolette
    sprintf(command, "sha1sum \"%s\"", path); // Costruiamo il comando
    
    FILE *fp = popen(command, "r"); // r per leggere l'output
    if (fp == NULL) return; // per evitare crash se popen fallisce
    
    // Leggiamo solo i primi 40 caratteri (la lunghezza dell'hash SHA1)
    // Perché 40? Perché lo SHA1 è composto esattamente da 40 cifre esadecimali.
    // dopo i 40 caratteri c'è uno spazio e poi il nome del file, che non ci interessa.
    if (fscanf(fp, "%40s", output) != 1) { // Se non riusciamo a leggere l'hash
        output[0] = '\0'; // Impostiamo l'output a stringa vuota
    } 
    pclose(fp); // Chiudiamo il pipe
}

int main(int argc, char *argv[]) {
    char *dir_path = (argc > 1) ? argv[1] : "."; //Una stringa che conterrà il percorso della cartella degli hash.
    char hidden_dir[PATH_MAX]; // path massimo per il percorso della cartella nascosta
    
    // 1. Costruiamo il percorso della cartella nascosta .sha1index
    // %s è il percorso della directory da scansionare
    // dir_path è il percorso che va in %s
    snprintf(hidden_dir, sizeof(hidden_dir), "%s/.sha1index", dir_path); 
    
    // 2. Creiamo la directory (se non esiste già) con permessi rwxr-xr-x (0755)
    mkdir(hidden_dir, 0755);

    // 3. Apriamo la directory da scansionare
    DIR *d = opendir(dir_path);
    if (!d) {
        perror("Errore apertura directory");
        return 1;
    }

    struct dirent *entry; // Per memorizzare ogni elemento della directory
    struct stat statbuf; // Per memorizzare le informazioni sul file
    char file_path[PATH_MAX]; // Per memorizzare il percorso completo del file
    char link_path[PATH_MAX]; // Per memorizzare il percorso del link simbolico
    char hash[41]; // Per memorizzare l'hash SHA1 (40 caratteri + terminatore null)

    // 4. Leggiamo ogni elemento nella directory
    while ((entry = readdir(d)) != NULL) { //readir restituisce NULL quando non ci sono più file nella directory
        // Saltiamo le directory "." e ".." e la cartella .sha1index stessa
        // strcmp confronta due stringhe e restituisce 0 se sono uguali
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".sha1index") == 0)
            continue;
        
        // Costruiamo il percorso completo del file
        snprintf(file_path, sizeof(file_path), "%s/%s", dir_path, entry->d_name);

        // Controlliamo se è un file ordinario (non una directory o un link)
        //lstat: Riempie statbuf con le informazioni del file.
        // S_ISREG: Macro che verifica se il file è un file regolare.
        // altrimenti andiamo avanti con il prossimo file
        if (lstat(file_path, &statbuf) == 0 && S_ISREG(statbuf.st_mode)) {
            
            // Calcoliamo l'hash del contenuto
            get_sha1(file_path, hash);
            if (strlen(hash) == 40) {
                // Costruiamo il percorso del link simbolico: .sha1index/HASH
                snprintf(link_path, sizeof(link_path), "%s/%s", hidden_dir, hash);
                
                // Rimuoviamo il link se esisteva già (per evitare errori)
                unlink(link_path);
                
                // Creiamo il link simbolico che punta al file originale (usiamo il path relativo corretto)
                // Nota: per semplicità puntiamo al path assoluto o relativo fornito
                if (symlink(file_path, link_path) == 0) {
                    printf("Link creato: %s -> %s\n", entry->d_name, hash);
                }
            }
        }
    }

    closedir(d);
    return 0;
}