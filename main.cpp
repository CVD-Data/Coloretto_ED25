\
#include <iostream>
#include <vector>
#include <string>
#include "Juego.h"
#include "Utilidades.h"

int main() {
    std::srand((unsigned)time(nullptr));
    Juego juego;
    while (true) {
        limpiarPantalla();
        std::cout << "\033[1;36m=== COLORETTO - Versión Simplificada e Interactiva ===\033[0m\n";
        std::cout << "1) Nueva partida\n2) Cargar partida\n3) Ver instrucciones\n4) Ver tabla de puntos\n5) Salir\nElija una opción: ";
        int opt;
        if (!(std::cin >> opt)) { std::cin.clear(); std::string tmp; std::getline(std::cin, tmp); continue; }
        if (opt == 1) {
            int n;
            do {
                limpiarPantalla();
                std::cout << "Número de jugadores (3-5): ";
                if (!(std::cin >> n)) { std::cin.clear(); std::string tmp; std::getline(std::cin,tmp); std::cout << "\033[31mEscoja la cantidad de jugadores de 3 a 5\033[0m\n"; pausa(); continue; }
                if (n < 3 || n > 5) { std::cout << "\033[31mEscoja la cantidad de jugadores de 3 a 5\033[0m\n"; pausa(); }
            }while (n < 3 || n > 5);
            std::vector<std::string> nombres;
            for (int i=0;i<n;++i) { 
                std::string s;
                std::cout << "Nombre jugador " << i+1 << ": "; 
                std::cin >> s; 
                nombres.push_back(s); 
            }

            juego.inicializarPartida(nombres);
            juego.iniciar();
        } else if (opt == 2) {
            std::string archivo; 
            std::cout << "Archivo a cargar: "; 
            std::cin >> archivo;
            if (juego.cargar(archivo)) { 
                std::cout << "\033[32mPartida cargada correctamente.\033[0m\n"; 
                pausa(); 
                juego.iniciar(); 
            }
            else { 
                std::cout << "\033[31mError cargando partida.\033[0m\n"; 
                pausa(); 
            }
        } else if (opt == 3) {
            juego.mostrarInstrucciones();
        } else if (opt == 4) {
            juego.mostrarTablaPuntos();
        } else break;
    }
    return 0;
}
