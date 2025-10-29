\
#include "juego.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>

Juego::Juego(): mazo(nullptr), pilas(nullptr), numPilas(MAX_PILAS), jugadorActual(0), cartasRobadasEstaRonda(0), ultimaRondaRevelada(false), faseActual(Fase::COLOCACION) {
    pilas = new Pila[numPilas];
}

Juego::~Juego() {
    limpiarMemoria();
    if (pilas) delete[] pilas;
}

void Juego::limpiarMemoria() {
    for (Jugador* p : jugadores) delete p;
    jugadores.clear();
    if (mazo) { delete mazo; mazo = nullptr; }
}

int Juego::numJugadoresEnPartida() const {
    return (int)jugadores.size();
}

void Juego::nuevaPartida(const std::vector<std::string>& nombresJugadores) {
    limpiarMemoria();
    for (const auto &n : nombresJugadores) jugadores.push_back(new Jugador(n));
    int nj = numJugadoresEnPartida();
    mazo = new Mazo(nj);
    for (int i=0;i<numPilas;++i) { pilas[i].cartas.clear(); pilas[i].robada = false; }
    jugadorActual = 0;
    cartasRobadasEstaRonda = 0;
    ultimaRondaRevelada = false;
    faseActual = Fase::COLOCACION;
}

std::string Juego::colorPorId(int id) const {
    switch(id) {
        case 0: return "\033[31m"; // rojo
        case 1: return "\033[32m"; // verde
        case 2: return "\033[34m"; // azul
        case 3: return "\033[33m"; // amarillo
        case 4: return "\033[35m"; // magenta
        case 5: return "\033[36m"; // cyan
        case 6: return "\033[37m"; // blanco
        case 100: return "\033[1;33m"; // +2 - dorado
        case 99: return "\033[1;33m"; // ultima ronda - dorado
        default: return "\033[0m";
    }
}

std::string Juego::simboloPorId(int id) const {
    if (id == 100) return "+2";
    if (id == 99) return "★";
    return "■";
}

void Juego::animacionRobar() const {
    for (int i=0;i<3;++i) { std::cout << "."; std::this_thread::sleep_for(std::chrono::milliseconds(160)); }
    std::cout << "\n";
}

void Juego::animacionAdvertencia(const std::string& msg) const {
    for (int i=0;i<3;++i) {
        std::cout << "\033[31m" << msg << "\033[0m\r";
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(220));
        std::cout << "                                                  \r";
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(120));
    }
    std::cout << msg << "\n";
}

void Juego::animacionFaseRobo() const {
    const std::vector<std::string> cols = { "\033[31m", "\033[35m", "\033[34m", "\033[36m", "\033[32m", "\033[33m", "\033[37m" };
    std::string msg = "🌈 Todas las pilas están llenas. Es momento de tomar una pila.";
    for (int turn=0; turn<8; ++turn) {
        std::string c = cols[turn % cols.size()];
        std::cout << c << msg << resetColor() << "\r";
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(160));
    }
    std::cout << "\n";
}

void Juego::animacionUltimaRonda() const {
    const std::vector<std::string> cols = { "\033[1;33m", "\033[37m", "\033[36m", "\033[35m" };
    std::string lines[] = {
        "🏁  ¡HAS ROBADO LA CARTA DE ÚLTIMA RONDA!  🏁",
        "✨  Esta es la última ronda del juego.  ✨",
        "💫  Juega hasta que todos tomen una pila.  💫"
    };
    for (int i=0;i<8;++i) {
        std::string c = cols[i % cols.size()];
        std::cout << c;
        for (auto &ln : lines) std::cout << ln << "\n";
        std::cout << resetColor();
        std::this_thread::sleep_for(std::chrono::milliseconds(220));
        std::cout << "\033[2J\033[H";
    }
    // print final once
    std::cout << "\033[1;33m🏁  ¡HAS ROBADO LA CARTA DE ÚLTIMA RONDA!  🏁\033[0m\n";
    std::cout << "\033[1;33m✨  Esta es la última ronda del juego.  ✨\033[0m\n";
    std::cout << "\033[1;33m💫  Juega hasta que todos tomen una pila.  💫\033[0m\n";
}

void Juego::animacionFinal() const {
    const std::vector<std::string> cols = { "\033[38;5;190m", "\033[38;5;226m", "\033[38;5;45m", "\033[38;5;200m" };
    std::string texto = "🏆  ¡FIN DEL JUEGO!  🎉\nGracias por jugar Coloretto 🌈";
    for (int i=0;i<10;++i) {
        std::string c = cols[i % cols.size()];
        std::cout << c << texto << resetColor() << "\r";
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(220));
    }
    std::cout << "\n\033[1;37mHasta la próxima ronda... 👋\033[0m\n";
}

void Juego::mostrarEstado() const {
    std::cout << "\n\033[1m--- TABLERO ---\033[0m\n";
    std::cout << "Mazo restante: " << (mazo ? mazo->restantes() : 0) << (ultimaRondaRevelada ? " (Última Ronda activada)" : "") << "\n\n";
    for (int i=0;i<numPilas;++i) {
        if (pilas[i].robada) {
            std::cout << "\033[90mPila " << i << " [ROBADА] ";
            for (int c : pilas[i].cartas) std::cout << colorPorId(c) << simboloPorId(c) << resetColor() << " ";
            std::cout << resetColor() << "\n";
        } else {
            std::cout << "\033[4mPila " << i << ":\033[0m ";
            if (pilas[i].cartas.empty()) std::cout << "(vacía)";
            else for (int c : pilas[i].cartas) std::cout << colorPorId(c) << simboloPorId(c) << resetColor() << " ";
            std::cout << "\n";
        }
    }
    std::cout << "\n\033[1mJugadores:\033[0m\n";
    for (size_t i=0;i<jugadores.size();++i) {
        std::cout << (i==jugadorActual ? "-> " : "   ");
        std::cout << i << ") " << jugadores[i]->obtenerNombre() << " - ";
        for (int c : jugadores[i]->obtenerCartas()) std::cout << colorPorId(c) << simboloPorId(c) << resetColor() << " ";
        std::cout << " | Puntaje (simple): " << jugadores[i]->puntaje();
        if (i==jugadorActual) std::cout << " \033[33m(Tu turno)\033[0m";
        std::cout << "\n";
    }
    std::cout << "\n";
}

void Juego::siguienteJugador() {
    if (!jugadores.empty()) jugadorActual = (jugadorActual + 1) % (int)jugadores.size();
}

bool Juego::todasPilasRobadas() const {
    for (int i=0;i<numPilas;++i) if (!pilas[i].robada) return false;
    return true;
}

bool Juego::todasPilasLlenas() const {
    for (int i=0;i<numPilas;++i) if ((int)pilas[i].cartas.size() < MAX_CARTAS_PILA && !pilas[i].robada) return false;
    return true;
}

void Juego::forzarFaseRoboSiCorresponde() {
    if (faseActual == Fase::COLOCACION && todasPilasLlenas()) {
        faseActual = Fase::ROBO;
        animacionFaseRobo();
    }
}

void Juego::iniciarNuevaRonda() {
    for (int i=0;i<numPilas;++i) {
        pilas[i].robada = false;
        pilas[i].cartas.clear();
    }
    cartasRobadasEstaRonda = 0;
    if (mazo) mazo->barajar();
    faseActual = Fase::COLOCACION;
    for (int i=0;i<3;++i) {
        std::cout << "\033[32m♻️ Comienza una nueva ronda...\033[0m\r";
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        std::cout << "                                \r";
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
    std::cout << "\n";
}

void Juego::bucleJuego() {
    bool salir = false;
    while (!salir) {
        mostrarEstado();
        forzarFaseRoboSiCorresponde();
        if (faseActual == Fase::COLOCACION) {
            std::cout << "Opciones:\n1) Robar carta y colocar en pila\n2) Tomar una pila\n3) Guardar partida\n4) Salir al menú principal\nElija: ";
        } else {
            std::cout << "\033[33mFase de robo: debes tomar una pila disponible.\033[0m\n";
            std::cout << "Opciones:\n2) Tomar una pila\n3) Guardar partida\n4) Salir al menú principal\nElija: ";
        }

        int opt;
        if (!(std::cin >> opt)) { std::cin.clear(); std::string tmp; std::getline(std::cin, tmp); animacionAdvertencia("⚠️ Dato inválido. Por favor ingresa un número válido."); continue; }

        if (faseActual == Fase::COLOCACION && opt == 1) {
            if (!mazo) { animacionAdvertencia("⚠️ No hay mazo inicializado."); continue; }
            std::cout << "Robando";
            animacionRobar();
            int carta = mazo->robar();
            if (carta == -1) {
                animacionAdvertencia("⚠️ No hay más cartas para robar.");
                std::cout << "\n\033[1m--- Partida finalizada ---\033[0m\n";
                for (auto p : jugadores) std::cout << p->obtenerNombre() << " -> " << p->puntaje() << "\n";
                animacionFinal();
                return;
            }
            // si es carta de ultima ronda
            if (carta == 99 && !ultimaRondaRevelada) {
                ultimaRondaRevelada = true;
                animacionUltimaRonda();
            }
            std::cout << "Carta robada: " << colorPorId(carta) << simboloPorId(carta) << resetColor() << " (id " << carta << ")\n";
            // pedir pila con validacion y permitir reintentos sin perder turno
            while (true) {
                std::cout << "¿En qué pila colocarla (0.." << (numPilas-1) << ")? ";
                int p;
                if (!(std::cin >> p)) {
                    std::cin.clear(); std::string tmp; std::getline(std::cin, tmp);
                    animacionAdvertencia("⚠️ Dato inválido. Por favor ingresa un número válido."); 
                    continue;
                }
                if (p < 0 || p >= numPilas) {
                    animacionAdvertencia("⚠️ Pila inválida. Intenta de nuevo.");
                    continue;
                }
                if (pilas[p].robada) {
                    animacionAdvertencia("⚠️ No se puede colocar en una pila robada. Elige otra pila.");
                    continue;
                }
                if ((int)pilas[p].cartas.size() >= MAX_CARTAS_PILA) {
                    animacionAdvertencia("⚠️ La pila seleccionada está llena (máximo 3 cartas). Elige otra pila.");
                    continue;
                }
                pilas[p].cartas.push_back(carta);
                std::cout << "Carta colocada.\n";
                break;
            }
            forzarFaseRoboSiCorresponde();
            siguienteJugador();
        } else if (opt == 2) {
            int p;
            while (true) {
                std::cout << "¿Qué pila tomar (0.." << (numPilas-1) << ")? ";
                if (!(std::cin >> p)) {
                    std::cin.clear(); std::string tmp; std::getline(std::cin, tmp);
                    animacionAdvertencia("⚠️ Dato inválido. Por favor ingresa un número válido."); 
                    continue;
                }
                if (p < 0 || p >= numPilas) {
                    animacionAdvertencia("⚠️ Pila inválida. Intenta de nuevo.");
                    continue;
                }
                if (pilas[p].robada) {
                    animacionAdvertencia("⚠️ Esa pila ya fue robada. Elige otra pila.");
                    continue;
                }
                if (pilas[p].cartas.empty()) {
                    animacionAdvertencia("⚠️ No puedes tomar una pila vacía. Elige otra pila.");
                    continue;
                }
                jugadores[jugadorActual]->agregarCartas(pilas[p].cartas);
                pilas[p].cartas.clear();
                pilas[p].robada = true;
                cartasRobadasEstaRonda++;
                std::cout << "\033[90mPila " << p << " ha sido robada.\033[0m\n";
                break;
            }
            if (todasPilasRobadas()) {
                // si ultimaRondaRevelada entonces terminar juego después de esta ronda
                if (ultimaRondaRevelada) {
                    std::cout << "\n\033[1m--- Ronda finalizada por carta de última ronda ---\033[0m\n";
                    std::cout << "\033[1mCalculando puntajes finales...\033[0m\n";
                    for (auto pl : jugadores) std::cout << pl->obtenerNombre() << " -> " << pl->puntaje() << "\n";
                    animacionFinal();
                    return;
                } else {
                    iniciarNuevaRonda();
                }
            } else {
                if (faseActual == Fase::ROBO) animacionFaseRobo();
            }
            siguienteJugador();
        } else if (opt == 3) {
            std::string archivo; std::cout << "Nombre de archivo para guardar: "; std::cin >> archivo;
            if (guardar(archivo)) std::cout << "\033[32mGuardado OK.\033[0m\n"; else std::cout << "\033[31mError guardando.\033[0m\n";
        } else if (opt == 4) {
            salir = true;
        } else {
            animacionAdvertencia("⚠️ Opción inválida. Ingresa la opción correcta.");
        }
    }
}

bool Juego::guardar(const std::string& nombreArchivo) const {
    std::ofstream ofs(nombreArchivo, std::ios::trunc);
    if (!ofs) return false;
    ofs << (faseActual == Fase::COLOCACION ? 0 : 1) << "\n";
    ofs << jugadorActual << "\n";
    ofs << (ultimaRondaRevelada ? 1 : 0) << "\n";
    ofs << cartasRobadasEstaRonda << "\n";
    ofs << (mazo ? mazo->serializar() : "") << "\n";
    for (int i=0;i<numPilas;++i) {
        ofs << (pilas[i].robada ? 1 : 0) << "|";
        for (size_t j=0;j<pilas[i].cartas.size();++j) {
            if (j) ofs << ",";
            ofs << pilas[i].cartas[j];
        }
        ofs << "\n";
    }
    ofs << jugadores.size() << "\n";
    for (Jugador* p : jugadores) ofs << p->serializar() << "\n";
    return true;
}

bool Juego::cargar(const std::string& nombreArchivo) {
    std::ifstream ifs(nombreArchivo);
    if (!ifs) return false;
    std::string linea;
    if (!std::getline(ifs, linea)) return false;
    int fase = std::stoi(linea);
    faseActual = (fase==0 ? Fase::COLOCACION : Fase::ROBO);
    if (!std::getline(ifs, linea)) return false;
    jugadorActual = std::stoi(linea);
    if (!std::getline(ifs, linea)) return false;
    ultimaRondaRevelada = (std::stoi(linea) != 0);
    if (!std::getline(ifs, linea)) return false;
    cartasRobadasEstaRonda = std::stoi(linea);
    if (!std::getline(ifs, linea)) return false;
    limpiarMemoria();
    mazo = new Mazo(); // default to 5 players; try to deserialize rest of cards
    if (!mazo->deserializar(linea)) return false;
    for (int i=0;i<numPilas;++i) {
        if (!std::getline(ifs, linea)) return false;
        pilas[i].cartas.clear();
        pilas[i].robada = false;
        if (linea.empty()) continue;
        auto pos = linea.find('|');
        if (pos==std::string::npos) return false;
        int r = std::stoi(linea.substr(0,pos));
        pilas[i].robada = (r!=0);
        std::string rest = linea.substr(pos+1);
        if (!rest.empty()) {
            std::istringstream ss(rest);
            std::string tok;
            while (std::getline(ss, tok, ',')) pilas[i].cartas.push_back(std::stoi(tok));
        }
    }
    if (!std::getline(ifs, linea)) return false;
    int numJug = std::stoi(linea);
    jugadores.clear();
    for (int i=0;i<numJug;++i) {
        if (!std::getline(ifs, linea)) return false;
        Jugador* p = new Jugador();
        if (!p->deserializar(linea)) { delete p; return false; }
        jugadores.push_back(p);
    }
    return true;
}

void Juego::mostrarInstrucciones() const {
    std::cout << "\n\033[1m=== INSTRUCCIONES ===\033[0m\n";
    std::cout << "- Soporta 3-5 jugadores.\n";
    std::cout << "- Pilas: máximo 3 cartas.\n";
    std::cout << "- Cuando se roba la carta de última ronda (★) se completa la ronda actual y luego el juego finaliza.\n";
    std::cout << "- Guardar / cargar incluye la fase y si la última ronda fue revelada.\n";
    std::cout << "Presiona Enter para continuar..."; std::cin.ignore(); std::cin.get();
}
