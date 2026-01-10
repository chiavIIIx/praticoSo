#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h> // Indispensabile per dlopen e dlsym

int main(int argc, char *argv[]) {
    // Controllo minimo: dobbiamo avere almeno il nome della libreria
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <path_libreria> [argomenti...]\n", argv[0]);
        return 1;
    }

    // 1. Carica la libreria specificata nel primo argomento (es. ./hw.so)
    void *handle = dlopen(argv[1], RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Errore nel caricamento: %s\n", dlerror());
        return 1;
    }

    // 2. Definisci un puntatore a funzione con la firma del main
    int (*libreria_main)(int, char **);

    // 3. Trova l'indirizzo della funzione "main" dentro la libreria
    libreria_main = dlsym(handle, "main");
    if (dlerror() != NULL) {
        fprintf(stderr, "Errore: funzione main non trovata\n");
        dlclose(handle);
        return 1;
    }

    // 4. Invoca la funzione
    // Passiamo (argc - 1) perché togliamo il nome di 'lancia' stesso.
    // Passiamo &argv[1] così la libreria vede come primo argomento il nome della libreria
    libreria_main(argc - 1, &argv[1]);

    // 5. Chiudi la libreria
    dlclose(handle);

    return 0;
}