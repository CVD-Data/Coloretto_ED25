# Coloretto_ED25
# Coloretto - Reimplementación fiel (ANSI, Última Ronda)

Este proyecto es la reimplementación solicitada del juego Coloretto en C++ con:
- Soporte para 3, 4 y 5 jugadores con cantidad de cartas configuradas según reglas.
- Carta de última ronda insertada ~15 cartas antes del final.
- Animaciones ANSI: arcoíris para fases, dorado para última ronda, animación final.
- Validaciones de entrada, reintentos sin perder turno.
- Serialización que incluye fase actual y si la última ronda fue revelada.

Compilar (Linux / WSL / MinGW):
```
g++ -std=c++17 principal.cpp src/*.cpp -Iinclude -o coloretto -pthread
```

En Windows con MinGW, usa la misma línea (y asegúrate de tener g++ en el PATH).

Estructura de directorios en el ZIP:
- principal.cpp
- include/*.h
- src/*.cpp
- README.md
