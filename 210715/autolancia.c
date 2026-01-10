#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>  // Indispensabile per dlopen e dlsym
#include <unistd.h>    // Per fork ed exec
#include <sys/wait.h>  // Per wait

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <file> [argomenti...]\n", argv[0]);
        return 1;
    }

    // 1. Proviamo a caricarlo come libreria dinamica (.so)
    void *handle = dlopen(argv[1], RTLD_LAZY);

    if (handle) {
        // --- CASO LIBRERIA DINAMICA ---
        int (*libreria_main)(int, char **); // Creiamo la "sagoma" per la funzione main che sta dentro la libreria
        libreria_main = dlsym(handle, "main");

        if (libreria_main) {
            libreria_main(argc - 1, &argv[1]);
        }
        dlclose(handle);
    } 
    else {
        // --- CASO ESEGUIBILE NORMALE ---
        // Se dlopen fallisce, proviamo a eseguirlo come programma normale
        pid_t pid = fork(); // Crea un processo figlio

        if (pid == 0) {
            // Siamo nel processo figlio: eseguiamo il programma
            // execvp vuole il nome del file e l'array degli argomenti
            execvp(argv[1], &argv[1]);
            
            // Se arriviamo qui, execvp ha fallito
            perror("Errore nell'esecuzione dell'eseguibile");
            exit(1);
        } else {
            // Siamo nel processo padre: aspettiamo che il figlio finisca
            wait(NULL);
        }
    }

    return 0;
}