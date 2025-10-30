\
#include "Jugador.h"
#include <sstream>
#include <algorithm>
#include <vector>

Jugador::Jugador(const std::string& nombre): _nombre(nombre) {}
Jugador::~Jugador() {}

void Jugador::agregarCartas(const std::vector<int>& ids) {
    for (int id : ids) _cartas[id]++;
}

const std::string& Jugador::nombre() const { return _nombre; }
const std::map<int,int>& Jugador::cartas_por_color() const { return _cartas; }

static int puntos_por_cantidad(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 3;
    if (n == 3) return 6;
    if (n == 4) return 10;
    if (n == 5) return 15;
    if (n == 6) return 21;
    return 28;
}

int Jugador::calcular_puntaje() const {
    std::vector<int> cantidades;
    for (auto &p : _cartas) {
        if (p.first == 99) continue;
        cantidades.push_back(p.second);
    }
    std::sort(cantidades.begin(), cantidades.end(), std::greater<int>());
    int total = 0;
    for (size_t i=0;i<cantidades.size();++i) {
        int pts = puntos_por_cantidad(cantidades[i]);
        if (i < 3) total += pts; else total -= pts;
    }

    return total;
}

int Jugador::puntaje() const { return calcular_puntaje(); }

std::string Jugador::serializar() const {
    std::ostringstream ss;
    ss << _nombre << "|";
    bool first = true;
    for (auto &p : _cartas) {
        if (!first) ss << ","; first = false;
        ss << p.first << ":" << p.second;
    }
    return ss.str();
}

bool Jugador::deserializar(const std::string& datos) {
    auto pos = datos.find('|');
    if (pos == std::string::npos) return false;
    _nombre = datos.substr(0,pos);
    _cartas.clear();
    std::string resto = datos.substr(pos+1);
    if (resto.empty()) return true;
    std::istringstream ss(resto);
    std::string token;
    while (std::getline(ss, token, ',')) {
        auto p = token.find(':');
        if (p==std::string::npos) continue;
        int id = std::stoi(token.substr(0,p));
        int cnt = std::stoi(token.substr(p+1));
        _cartas[id] = cnt;
    }
    return true;
}
