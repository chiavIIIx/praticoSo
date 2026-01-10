#include <errno.h> // Per errno
#define _GNU_SOURCE
#include <signal.h> // Per signalfd
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signalfd.h> // Per signalfd
#include <unistd.h> // Per close

int main(void) {
    // 1) Prepara la mask dei segnali che vuoi ricevere via signalfd
    sigset_t mask;
    // Inizializza la mask a zero
    if (sigemptyset(&mask) == -1) {
        perror("sigemptyset");
        return 1;
    }
    // Aggiungi SIGUSR1 e SIGUSR2 alla mask
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

    int counter = 0;

    // 4) Loop: leggi struct signalfd_siginfo dal fd
    for (;;) {
        struct signalfd_siginfo si;
        ssize_t n = read(sfd, &si, sizeof(si));

        if (n == -1) {
            if (errno == EINTR) continue;
            perror("read(signalfd)");
            close(sfd);
            return 1;
        }

        if (n != (ssize_t)sizeof(si)) {
            fprintf(stderr, "read: size inattesa (%zd)\n", n);
            close(sfd);
            return 1;
        }

        // 5) Aggiorna contatore in base al segnale ricevuto
        if (si.ssi_signo == SIGUSR1) {
            counter += 1;
            printf("%d\n", counter);
            fflush(stdout);
        } else if (si.ssi_signo == SIGUSR2) {
            counter -= 1;
            printf("%d\n", counter);
            fflush(stdout);
        } else {
            // Non dovrebbe succedere perché la mask include solo questi due.
            fprintf(stderr, "Segnale inatteso: %u\n", si.ssi_signo);
        }
    }

    // Non si arriva mai qui
    // close(sfd);
    // return 0;
}
