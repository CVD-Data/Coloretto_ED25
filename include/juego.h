\
#ifndef COLORETTO_JUEGO_H
#define COLORETTO_JUEGO_H

#include <vector>
#include <string>
//#include "jugador.h"
//#include "mazo.h"

enum class Fase { COLOCACION = 0, ROBO = 1 };

struct Pila {
    std::vector<int> cartas; // ids de color o especiales
    bool robada;
    Pila(): robada(false) {}
};

class Juego {
public:
    Juego();
    ~Juego();
    void nuevaPartida(const std::vector<std::string>& nombresJugadores);
    void bucleJuego();
    bool guardar(const std::string& nombreArchivo) const;
    bool cargar(const std::string& nombreArchivo);
    void mostrarInstrucciones() const;
private:
    std::vector<Jugador*> jugadores; // punteros dinámicos
    Mazo* mazo; // puntero dinámico al mazo
    Pila* pilas; // arreglo dinámico de Pila
    int numPilas;
    int jugadorActual;
    int cartasRobadasEstaRonda;
    bool ultimaRondaRevelada;
    Fase faseActual;
    static const int MAX_PILAS = 3;
    static const int MAX_CARTAS_PILA = 3;
    void limpiarMemoria();
    void mostrarEstado() const;
    void siguienteJugador();
    bool todasPilasRobadas() const;
    bool todasPilasLlenas() const;
    std::string colorPorId(int id) const;
    std::string simboloPorId(int id) const;
    std::string resetColor() const { return "\033[0m"; }
    void animacionRobar() const;
    void animacionAdvertencia(const std::string& msg) const;
    void animacionFaseRobo() const; // arcoiris suave
    void animacionUltimaRonda() const; // dorado
    void animacionFinal() const;
    void iniciarNuevaRonda();
    void forzarFaseRoboSiCorresponde();
    int numJugadoresEnPartida() const;
};

#endif // COLORETTO_JUEGO_H
