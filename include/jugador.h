\
#ifndef JUGADOR_H
#define JUGADOR_H

#include <string>
#include <map>
#include <vector>

class Jugador {
public:
    Jugador(const std::string &nombre);
    ~Jugador();
    void agregarCartas(const std::vector<int> &ids);
    const std::string& nombre() const;
    const std::map<int,int> &cartas_por_color() const;
    int puntaje() const;
    std::string serializar() const;
    bool deserializar(const std::string &datos);
private:
    std::string _nombre;
    std::map<int,int> _cartas; // id / cantidad(color)
    int calcular_puntaje() const;
};

#endif