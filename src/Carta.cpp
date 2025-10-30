\
#include "Carta.h"

Carta::Carta(): _id(-1), _nombre(""), _ultima(false) {}
Carta::Carta(int id, const std::string& nombre, bool ultima): _id(id), _nombre(nombre), _ultima(ultima) {}
Carta::~Carta() {}

int Carta::getId() const { return _id; }
std::string Carta::getNombre() const { return _nombre; }
bool Carta::esUltimaRonda() const { return _ultima; }
