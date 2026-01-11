# praticoSo
materiale utile per pratico sistemi operativi

## siti utili:

syscal: https://github.com/AlleNeri/esercizi-pratici-so/blob/main/README.md

notion utile:
https://glacier-shrew-efc.notion.site/Sistemi-operativi-f073b17b59ce4ef8957574f3e6e40075

altro notion:
https://mesquite-idea-e8e.notion.site/Sistemi-Operativi-020a54acb8d042308900e63cf996e79d

Perfetto, ho capito benissimo l’obiettivo 👍
Qui sotto trovi **una versione “pulita da README”**, **senza esercitazioni, senza riferimenti ai file**, solo **nome del comando / funzione + cosa fa**.
Stile asciutto, da repo Git.

Puoi copiare–incollare direttamente.

---

# Esercizi-pratici-so

Raccolta di **system call e funzioni utili** usate nella preparazione alla prova pratica di **Sistemi Operativi**.

---

## System call e funzioni utili in C

### Gestione processi

* `fork` – Crea un nuovo processo figlio duplicando il processo chiamante.
* `execv` – Sostituisce l’immagine del processo corrente con un nuovo programma.
* `execvp` – Come `execv`, ma cerca il comando nel `PATH`.
* `wait` – Attende la terminazione di un processo figlio.
* `waitpid` – Attende la terminazione di uno specifico processo figlio.
* `prctl` – Controllo e configurazione di proprietà del processo.

### File e filesystem

* `open` – Apre un file e restituisce un file descriptor.
* `creat` – Crea un file.
* `read` – Legge dati da un file descriptor.
* `write` – Scrive dati su un file descriptor.
* `lseek` – Sposta l’offset di lettura/scrittura di un file descriptor.
* `ftell` – Restituisce la posizione corrente nel file.
* `truncate` – Modifica la dimensione di un file.
* `stat` – Restituisce informazioni sullo stato di un file.
* `lstat` – Come `stat`, ma non segue i link simbolici.
* `remove` – Rimuove un file o una directory.
* `link` – Crea un hard link.
* `symlink` – Crea un link simbolico.
* `readlink` – Legge il contenuto di un link simbolico.
* `realpath` – Restituisce il path assoluto risolvendo i link simbolici.

### Directory

* `opendir` – Apre una directory.
* `readdir` – Legge una entry di una directory.
* `closedir` – Chiude una directory.
* `fdopendir` – Apre una directory a partire da un file descriptor.

### Pipe e comunicazione

* `pipe` – Crea una pipe anonima.
* `mkfifo` – Crea una named pipe (FIFO).
* `dup2` – Duplica un file descriptor su un altro.

### Segnali

* `kill` – Invia un segnale a un processo.
* `sigaction` – Definisce il comportamento alla ricezione di un segnale.
* `sigemptyset` – Inizializza una maschera di segnali vuota.
* `sigaddset` – Aggiunge un segnale a una maschera.
* `sigqueue` – Invia un segnale con un valore associato.
* `signalfd` – Riceve segnali tramite file descriptor.
* `sigaltstack` – Imposta uno stack alternativo per la gestione dei segnali.

### Timer e tempo

* `time` – Restituisce il tempo corrente in secondi dall’Epoch.
* `clock_gettime` – Restituisce il tempo con precisione in nanosecondi.
* `timerfd_create` – Crea un timer come file descriptor.
* `timerfd_settime` – Imposta e avvia un timer.
* `poll` – Monitora più file descriptor contemporaneamente.

### Memoria

* `memcpy` – Copia memoria byte per byte.
* `memcmp` – Confronta memoria byte per byte.

### Librerie dinamiche

* `dlopen` – Carica una libreria dinamica.
* `dlsym` – Ottiene il simbolo di una libreria caricata.
* `dlerror` – Restituisce l’ultimo errore della libreria dinamica.
* `dlclose` – Chiude una libreria dinamica.

---

## System call e funzioni utili in Python

### File e filesystem

* `os.stat` – Restituisce informazioni sullo stato di un file.
* `os.listdir` – Elenca il contenuto di una directory.
* `os.path.exists` – Verifica l’esistenza di un path.
* `os.path.is_*` – Verifica il tipo di file (directory, file, link, ecc.).
* `os.path.splitext` – Separa nome file ed estensione.
* `os.path.split` – Divide path e nome file.
* `os.path.join` – Concatena correttamente path.
* `os.path.dirname` – Restituisce la directory di un path.
* `os.path.getsize` – Restituisce la dimensione di un file.
* `os.readlink` – Legge il contenuto di un link simbolico.
* `os.remove` – Rimuove un file.
* `os.makedirs` – Crea una directory (anche nidificata).
* `os.getcwd` – Restituisce la directory corrente.

### File I/O

* `open` – Apre un file.
* `sys.stdin` – Accesso diretto allo standard input.

### Processi e comandi

* `subprocess.run` – Esegue un comando di shell.
* `multiprocessing.Pool` – Esecuzione parallela di funzioni.

### Utility

* `shutil.copy` – Copia un file.
* `shutil.copytree` – Copia una directory.
* `binascii.hexlify` – Converte dati binari in esadecimale.
* `magic.from_file` – Determina il tipo di un file.
* `getpass.getuser` – Restituisce l’utente corrente.
* `pwd.getpwuid` – Restituisce l’utente proprietario di un file.
* `shlex.split` – Tokenizza una stringa come farebbe la shell.


