import os
import sys

if len(sys.argv) != 2:
    print(f"Uso: {sys.argv[0]} <directory>")
    sys.exit(1)

root_dir = sys.argv[1]

file_piu_nuovo = None
file_piu_vecchio = None
tempo_piu_nuovo = None
tempo_piu_vecchio = None

for dirpath, dirnames, filenames in os.walk(root_dir):
    for nomefile in filenames:
        percorso = os.path.join(dirpath, nomefile)

        try:
            tempo = os.path.getmtime(percorso)
        except OSError:
            continue

        if tempo_piu_nuovo is None or tempo > tempo_piu_nuovo:
            tempo_piu_nuovo = tempo
            file_piu_nuovo = percorso

        if tempo_piu_vecchio is None or tempo < tempo_piu_vecchio:
            tempo_piu_vecchio = tempo
            file_piu_vecchio = percorso

print("File modificato più recentemente:")
print(file_piu_nuovo)

print("\nFile modificato più anticamente:")
print(file_piu_vecchio)
