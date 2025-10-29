\
#include "jugador.h"
#include <sstream>

Jugador::Jugador(const std::string& nombre): nombre(nombre) {}
Jugador::~Jugador() {}

void Jugador::agregarCarta(int color) {
    cartas.push_back(color);
}

void Jugador::agregarCartas(const std::vector<int>& cols) {
    for (int c : cols) cartas.push_back(c);
}

const std::string& Jugador::obtenerNombre() const { return nombre; }
const std::vector<int>& Jugador::obtenerCartas() const { return cartas; }

int Jugador::puntaje() const {
    // scoring placeholder: sum 1 per card, +2 cards add 2 points
    int total = 0;
    for (int c : cartas) {
        if (c == 100) total += 2; // +2 card
        else total += 1;
    }
    return total;
}

std::string Jugador::serializar() const {
    std::ostringstream ss;
    ss << nombre << "|";
    for (size_t i=0;i<cartas.size();++i) {
        if (i) ss << ",";
        ss << cartas[i];
    }
    return ss.str();
}

bool Jugador::deserializar(const std::string& datos) {
    auto pos = datos.find('|');
    if (pos == std::string::npos) return false;
    nombre = datos.substr(0,pos);
    cartas.clear();
    std::string resto = datos.substr(pos+1);
    if (resto.empty()) return true;
    std::istringstream ss(resto);
    std::string token;
    while (std::getline(ss, token, ',')) {
        try { cartas.push_back(std::stoi(token)); }
        catch(...) { return false; }
    }
    return true;
}
