#define _GNU_SOURCE
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signalfd.h>
#include <unistd.h>

int main(void) {
    // 1) Prepara la mask dei segnali che vuoi ricevere via signalfd
    sigset_t mask;
    if (sigemptyset(&mask) == -1) {
        perror("sigemptyset");
        return 1;
    }
    if (sigaddset(&mask, SIGUSR1) == -1) {
        perror("sigaddset(SIGUSR1)");
        return 1;
    }
    if (sigaddset(&mask, SIGUSR2) == -1) {
        perror("sigaddset(SIGUSR2)");
        return 1;
    }

    // 2) Blocca SIGUSR1 e SIGUSR2: così NON vanno ai signal handler,
    //    ma restano "pending" e li leggerai dal file descriptor.
    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) {
        perror("sigprocmask(SIG_BLOCK)");
        return 1;
    }

    // 3) Crea il file descriptor per leggere i segnali
    int sfd = signalfd(-1, &mask, SFD_CLOEXEC);
    if (sfd == -1) {
        perror("signalfd");
        return 1;
    }

    printf("PID = %ld\n", (long)getpid());
    printf("Invia segnali con:\n");
    printf("  kill -USR1 %ld\n", (long)getpid());
    printf("  kill -USR2 %ld\n", (long)getpid());
    fflush(stdout);

    int c
