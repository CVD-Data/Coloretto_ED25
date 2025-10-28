\
#ifndef COLORETTO_MAZO_H
#define COLORETTO_MAZO_H

#include <vector>
#include <string>

class Mazo {
public:
    Mazo(int numJugadores = 5); // por defecto 5
    ~Mazo();
    int robar(); // devuelve -1 si está vacío
    int restantes() const;
    void barajar();
    std::string serializar() const;
    bool deserializar(const std::string& datos);
    const std::vector<int>& obtenerCartas() const;
private:
    std::vector<int>* cartas; // vector en memoria dinámica
    void inicializarEstandar(int numJugadores);
};

#endif // COLORETTO_MAZO_H
