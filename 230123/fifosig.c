#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Uso: %s <fifo>\n", argv[0]);
        exit(1);
    }

    char *fifo = argv[1];
    char buffer[128];

    mkfifo(fifo, 0666);   // se esiste già non è un problema

    while (1) {
        int fd = open(fifo, O_RDONLY);
        FILE *f = fdopen(fd, "r");

        while (fgets(buffer, sizeof(buffer), f) != NULL) {
            pid_t pid;
            int sig;

            if (sscanf(buffer, "%d %d", &pid, &sig) == 2) {
                kill(pid, sig);
            }
        }

        fclose(f);
    }
}
