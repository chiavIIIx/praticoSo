#!/usr/bin/env python3
import shlex 
import subprocess
import sys


def main() -> int:
    while True:
        try:
            line = input("pysh> ") # prendi input
        except EOFError:
            # Fine input (^D)
            print()
            break

        line = line.strip() # Rimuovi spazi iniziali/finali
        if line == "":
            continue

        if line == "logout":
            break

        # Split stile shell (gestisce virgolette, ecc.)
        try:
            argv = shlex.split(line)
        except ValueError as e:
            # Es. virgolette non chiuse
            print(f"parse error: {e}", file=sys.stderr)
            continue

        # Esegui senza passare da /bin/sh (shell=False)
        try:
            subprocess.run(argv, shell=False, check=False)
        except FileNotFoundError:
            # comando non trovato
            print(f"{argv[0]}: command not found", file=sys.stderr)
        except PermissionError:
            # esiste ma non eseguibile
            print(f"{argv[0]}: permission denied", file=sys.stderr)
        except Exception as e:
            # altri errori
            print(f"error: {e}", file=sys.stderr)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
