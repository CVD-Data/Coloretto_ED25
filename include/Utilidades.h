\
#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <string>

// Limpiar pantalla - Windows (según especificación)
inline void limpiarPantalla(){
    system("clear");
}

// Colores ANSI
std::string color_por_id(int id);
std::string reset_color();

// Pausa
void pausa();

// Leer entero seguro (no consume newline)
int leerEnteroSeguro();

#endif // UTILIDADES_H
