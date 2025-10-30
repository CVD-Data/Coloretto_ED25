\
#include "Utilidades.h"
#include <iostream>
#include <limits>
#include <thread>
#include <chrono>

std::string color_por_id(int id) {
    switch(id) {
        case 0: return "\033[31m"; // rojo
        case 1: return "\033[32m"; // verde
        case 2: return "\033[34m"; // azul
        case 3: return "\033[33m"; // amarillo
        case 4: return "\033[35m"; // magenta/naranja
        case 5: return "\033[36m"; // cyan
        case 6: return "\033[37m"; // blanco/rosa
        case 99: return "\033[1;33m"; // ultima ronda - dorado
        default: return "\033[0m";
    }
}

std::string reset_color() { return "\033[0m"; }

void pausa() {
    std::cout << "\nPresiona ENTER para continuar...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

int leerEnteroSeguro() {
    int v;
    while (!(std::cin >> v)) {
        std::cin.clear();
        std::string tmp; std::getline(std::cin, tmp);
        limpiarPantalla();
        std::cout << "\033[31mDato inválido. Intenta de nuevo.\033[0m\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
        std::cout << "> ";
    }
    std::string tmp; std::getline(std::cin, tmp); // consumir resto de linea
    return v;
}
