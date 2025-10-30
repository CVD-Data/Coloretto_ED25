\
#ifndef JUEGO_H
#define JUEGO_H

#include <vector>
#include <string>
#include "Jugador.h"

enum class Fase { COLOCACION, ROBO };

struct Pila {
    std::vector<int> cartas; // ids
    bool robada;
    Pila(): robada(false) {}
};

class Juego {
public:
    Juego();
    ~Juego();
    void inicializarPartida(const std::vector<std::string>& nombres);
    void iniciar(); 
    bool guardar(const std::string& archivo) const;
    bool cargar(const std::string& archivo);
    void mostrarInstrucciones() const;
    void mostrarTablaPuntos() const;
private:
    Jugador** jugadores; // arreglo dinamico de punteros
    int numJugadores;
    Pila* pilas; // arreglo dinamico de pilas
    int numPilas;
    int jugadorActual;
    bool ultimaRondaRevelada;
    Fase faseActual;
    std::vector<int>* mazo; // Usamos memoria dinamica para el mazo
    void limpiar_memoria();
    void prepararMazoSegunModo();
    int contarTotalCartasSegunModo() const;
    void insertarUltimaRondaSegunModo();
    void mostrarTablero() const;
    bool hayPilaConCartas() const;
    void robarYColocar();
    void tomarPila();
    void iniciarNuevaRonda();
    void mostrarResultadosFinales() const;
};

#endif // JUEGO_H
