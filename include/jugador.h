\
#ifndef COLORETTO_JUGADOR_H
#define COLORETTO_JUGADOR_H

#include <vector>
#include <string>

class Jugador {
public:
    Jugador() = default;
    Jugador(const std::string& nombre);
    ~Jugador();
    void agregarCarta(int color);
    void agregarCartas(const std::vector<int>& cols);
    const std::string& obtenerNombre() const;
    const std::vector<int>& obtenerCartas() const;
    int puntaje() const;
    // serializacion
    std::string serializar() const;
    bool deserializar(const std::string& datos);
private:
    std::string nombre;
    std::vector<int> cartas; // ids de color recogidas (+2 as 100)
};

#endif // COLORETTO_JUGADOR_H
