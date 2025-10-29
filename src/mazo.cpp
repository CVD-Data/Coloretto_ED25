\
#include "mazo.h"
#include <algorithm>
#include <random>
#include <chrono>
#include <sstream>

Mazo::Mazo(int numJugadores) {
    cartas = new std::vector<int>();
    inicializarEstandar(numJugadores);
    barajar();
}

Mazo::~Mazo() {
    delete cartas;
}

void Mazo::inicializarEstandar(int numJugadores) {
    cartas->clear();
    int numColores = 7;
    if (numJugadores == 3) numColores = 5;
    else if (numJugadores == 4) numColores = 6;
    else if (numJugadores == 5) numColores = 7;
    // añadir 9 cartas por color (ids 0..numColores-1)
    for (int color=0;color<numColores;++color) {
        for (int i=0;i<9;++i) cartas->push_back(color);
    }
    // añadir 10 cartas +2 (id 100)
    for (int i=0;i<10;++i) cartas->push_back(100);
    // barajar antes de insertar ultima ronda to randomize distribution
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(cartas->begin(), cartas->end(), std::default_random_engine(seed));
    // insertar carta de ultima ronda (id 99) aproximadamente 15 cartas del final
    int pos = (int)cartas->size() - 15;
    if (pos < 0) pos = 0;
    cartas->insert(cartas->begin() + pos, 99);
}

void Mazo::barajar() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(cartas->begin(), cartas->end(), std::default_random_engine(seed));
}

int Mazo::robar() {
    if (cartas->empty()) return -1;
    int c = cartas->back();
    cartas->pop_back();
    return c;
}

int Mazo::restantes() const { return (int)cartas->size(); }

std::string Mazo::serializar() const {
    std::ostringstream ss;
    for (size_t i=0;i<cartas->size();++i) {
        if (i) ss << ",";
        ss << (*cartas)[i];
    }
    return ss.str();
}

bool Mazo::deserializar(const std::string& datos) {
    cartas->clear();
    if (datos.empty()) return true;
    std::istringstream ss(datos);
    std::string token;
    while (std::getline(ss, token, ',')) {
        try { cartas->push_back(std::stoi(token)); }
        catch(...) { return false; }
    }
    return true;
}

const std::vector<int>& Mazo::obtenerCartas() const { return *cartas; }
