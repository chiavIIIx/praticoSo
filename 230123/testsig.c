#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>

void handler(int sig) {
    printf("Ricevuto SIGUSR1 (%d)\n", sig);
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Uso: %s <fifo>\n", argv[0]);
        exit(1);
    }

    signal(SIGUSR1, handler);

    pid_t pid = getpid();
    printf("PID = %d\n", pid);

    int fd = open(argv[1], O_WRONLY);

    char buf[64];
    sprintf(buf, "%d %d\n", pid, SIGUSR1);
    write(fd, buf, strlen(buf));
    close(fd);

    while (1) {
        pause();
    }
}
