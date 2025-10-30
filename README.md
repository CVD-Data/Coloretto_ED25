# 🎨 Coloretto - Versión Simplificada (C++ Modular)

Un juego de cartas inspirado en **Coloretto**, desarrollado en **C++** con una arquitectura modular y funcionalidad interactiva por consola.  
El proyecto simula las fases principales del juego original: **colocación, robo de pilas y cálculo de puntajes**, conservando su esencia estratégica.

---

## 🧱 Estructura del Proyecto

├── include/
│ ├── Carta.h
│ ├── Jugador.h
│ ├── Juego.h
│ └── Utilidades.h
├── src/
│ ├── Carta.cpp
│ ├── Jugador.cpp
│ ├── Juego.cpp
│ └── Utilidades.cpp
├── main.cpp
├── Makefile
└── README.md

---

## ⚙️ Compilación y Ejecución

### 🔹 Compilar manualmente (Windows o Linux)

    g++ -std=c++17 main.cpp src/*.cpp -Iinclude -o coloretto -pthread

### 🔹 Compilar usando Makefile

    make

## 🔹 Ejecutar el juego

En Windows:

    coloretto.exe

En Linux/macOS:

    ./coloretto


### 💡 En la carpeta se incluye un ejecutable listo para usar:
    coloretto.exe (solo hacer doble clic).

## 🧩 Características Principales

- Juego para (3 - 5) jugadores. 
- Colores ANSI en consola para distinguir cartas y fases.
- Sistema de guardado/carga de partidas mediante serialización XOR (archivos no legibles directamente).
- Limpieza de pantalla automática (system("clear") o cls).
- Control de errores en entrada con validación robusta.
- Soporte para 2 a 5 jugadores con adaptación del mazo.
- Implementación de la última ronda según modo de juego.
- Cálculo automático de puntajes según la tabla oficial de Coloretto.

## 🎮 Descripción del Menú Principal

Al iniciar el juego se muestra un menú interactivo con las siguientes opciones:

| Opción                   | Descripción                                       |
| ------------------------ | ------------------------------------------------- |
| **1. Nueva Partida**     | Inicia un juego nuevo configurando los jugadores. |
| **2. Cargar Partida**    | Carga una partida guardada (si existe).           |
| **3. Guardar y Salir**   | Guarda el progreso actual y termina el programa.  |
| **4. Salir sin Guardar** | Finaliza el programa sin guardar el estado.       |

Durante el turno, los jugadores pueden:

- Robar y colocar una carta en una pila.
- Tomar una pila para conservar las cartas y pasar de fase.

## 📜 Reglas Básicas del Juego

1) En cada turno, el jugador puede:
- Robar una carta del mazo y colocarla en una pila disponible.
- Tomar una pila, quedándose con todas sus cartas y quedando fuera de la ronda actual.

2) Cada pila puede contener un máximo de 3 cartas.
3) Cuando todos los jugadores han tomado una pila, se inicia una nueva ronda.
4) La carta de Última Ronda marca el final del juego; cuando se roba, se juega una última ronda antes del conteo final.
5) Al final:
- Solo los 3 colores más numerosos de cada jugador suman puntos positivos.
- Los colores restantes restan puntos.
- La tabla de puntuación es:

## 💡 Ejemplos de Juego
### 🔸 Ejemplo 1 — Robar y colocar carta

> Turno de Ana
Robas una carta verde
La colocas en la pila 2

### 🔸 Ejemplo 2 — Tomar pila

> Turno de Luis
Toma la pila 1 con las cartas: [Roja, Azul, Amarilla]

### 🔸 Ejemplo 3 — Fin de ronda y puntuación

Ronda finalizada.
Ana: 24 puntos
Luis: 19 puntos
Carlos: 10 puntos
Ganador: Ana 🎉

### 👨‍💻 Créditos
Proyecto académico desarrollado por:
- Cristian Valdés Díaz
- Miguel Ángel Rendón Barrios
- Juan David Rodíguez Leguizamón