#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

void get_sha1(const char *path, char *output) {
    char command[PATH_MAX + 20];
    sprintf(command, "shasum \"%s\"", path); // Ho usato shasum per macOS
    FILE *fp = popen(command, "r");
    if (fp == NULL) return;
    if (fscanf(fp, "%40s", output) != 1) output[0] = '\0';
    pclose(fp);
}

int main(int argc, char *argv[]) {
    char *dir_path = (argc > 1) ? argv[1] : ".";
    char hidden_dir[PATH_MAX];
    snprintf(hidden_dir, sizeof(hidden_dir), "%s/.sha1index", dir_path);

    DIR *d = opendir(hidden_dir);
    if (!d) return 1;

    struct dirent *entry;
    struct stat stat_link, stat_orig;
    char link_full_path[PATH_MAX];
    char target_path[PATH_MAX];

    while ((entry = readdir(d)) != NULL) {
        if (entry->d_name[0] == '.') continue;

        snprintf(link_full_path, sizeof(link_full_path), "%s/%s", hidden_dir, entry->d_name);

        // Leggiamo dove punta il link
        ssize_t len = readlink(link_full_path, target_path, sizeof(target_path) - 1);
        if (len == -1) continue;
        target_path[len] = '\0';

        // IMPORTANTE: stat() deve controllare il file originale.
        // Se il link punta a "./f1", dobbiamo assicurarci che il percorso sia corretto.
        if (stat(target_path, &stat_orig) != 0) {
            // Se stat fallisce, il file è davvero sparito
            printf("File rimosso o non raggiungibile: elimino link %s\n", entry->d_name);
            unlink(link_full_path);
        } else {
            lstat(link_full_path, &stat_link);
            if (stat_orig.st_mtime > stat_link.st_mtime) {
                printf("File modificato: aggiorno %s\n", target_path);
                char new_hash[41];
                get_sha1(target_path, new_hash);
                if (strlen(new_hash) == 40) {
                    char new_link_path[PATH_MAX];
                    snprintf(new_link_path, sizeof(new_link_path), "%s/%s", hidden_dir, new_hash);
                    unlink(link_full_path);
                    unlink(new_link_path);
                    symlink(target_path, new_link_path);
                }
            }
        }
    }
    closedir(d);
    return 0;
}