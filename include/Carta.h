\
#ifndef CARTA_H
#define CARTA_H

#include <string>

class Carta {
public:
    Carta();
    Carta(int id, const std::string& nombre, bool ultima=false);
    ~Carta();
    int getId() const;
    std::string getNombre() const;
    bool esUltimaRonda() const;
private:
    int _id;
    std::string _nombre;
    bool _ultima;
};

#endif // CARTA_H
