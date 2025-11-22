cls
del Pac-Man.exe
gcc -std=c11 -o3 main.c Init.c Engine.c GhostAI.c -o Pac-Man.exe -I SDL2/include -L SDL2/lib/x64 -lSDL2main -lSDL2
Pac-Man.exe
