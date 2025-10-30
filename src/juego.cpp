\
#include "Juego.h"
#include "Utilidades.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>

Juego::Juego(): jugadores(nullptr), numJugadores(0), pilas(nullptr), numPilas(3),
    jugadorActual(0), ultimaRondaRevelada(false), faseActual(Fase::COLOCACION), mazo(nullptr) {}

Juego::~Juego() {
    limpiar_memoria();
}

void Juego::limpiar_memoria() {
    if (jugadores) {
        for (int i=0;i<numJugadores;++i) delete jugadores[i];
        delete[] jugadores; jugadores = nullptr;
    }
    if (pilas) { delete[] pilas; pilas = nullptr; }
    if (mazo) { delete mazo; mazo = nullptr; }
    numJugadores = 0;
    jugadorActual = 0;
    ultimaRondaRevelada = false;
    faseActual = Fase::COLOCACION;
}

int Juego::contarTotalCartasSegunModo() const {
    if (numJugadores == 3) return 56;
    if (numJugadores == 4) return 65;
    return 74;
}

void Juego::prepararMazoSegunModo() {
    if (mazo) { delete mazo; mazo = nullptr; }
    mazo = new std::vector<int>();
    int numColores = (numJugadores==3?5:(numJugadores==4?6:7));
    for (int c=0;c<numColores;++c) {
        for (int i=0;i<9;++i) mazo->push_back(c);
    }
    // barajar
    unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(mazo->begin(), mazo->end(), std::default_random_engine(seed));
}

// void Juego::insertarUltimaRondaSegunModo() {
//     // Ahora esta función no inserta la carta al inicio. La lógica para añadir la carta de última
//     // ronda ocurre dinámicamente durante el juego cuando queden exactamente 5 cartas en el mazo.
// }

void Juego::inicializarPartida(const std::vector<std::string>& nombres) {
    limpiar_memoria();
    numJugadores = (int)nombres.size();
    jugadores = new Jugador*[numJugadores];
    for (int i=0;i<numJugadores;++i) jugadores[i] = new Jugador(nombres[i]);
    numPilas = 3;
    pilas = new Pila[numPilas];
    jugadorActual = 0;
    ultimaRondaRevelada = false;
    faseActual = Fase::COLOCACION;
    prepararMazoSegunModo();
}

void Juego::mostrarInstrucciones() const {
    limpiarPantalla();
    std::cout << "\033[1mINSTRUCCIONES - COLORETTO (CONSOLA)\033[0m\n\n";
    std::cout << "Reglas principales (versión simplificada):\n";
    std::cout << "- 3 a 5 jugadores. No se muestra la cantidad de cartas del mazo.\n";
    std::cout << "- Hay 3 pilas. Cada pila admite máximo 3 cartas.\n";
    std::cout << "- En tu turno puedes robar una carta y colocarla en una pila, o tomar una pila.\n";
    std::cout << "- Si se roba la carta de Última Ronda (★), se completa la ronda actual y luego finaliza el juego.\n";
    std::cout << "- Tabla de puntuación disponible en el menú.\n";
    pausa();
}

void Juego::mostrarTablaPuntos() const {
    limpiarPantalla();
    std::cout << "\033[36mTabla de Puntos:\033[0m\n\n";
    std::cout << "| Cartas del mismo color | Puntos obtenidos o perdidos |\n";
    std::cout << "| ---------------------- | --------------------------- |\n";
    std::cout << "| 1 carta                | ±1 punto                    |\n";
    std::cout << "| 2 cartas               | ±3 puntos                   |\n";
    std::cout << "| 3 cartas               | ±6 puntos                   |\n";
    std::cout << "| 4 cartas               | ±10 puntos                  |\n";
    std::cout << "| 5 cartas               | ±15 puntos                  |\n";
    std::cout << "| 6 cartas               | ±21 puntos                  |\n";
    std::cout << "| 7 o más cartas         | ±28 puntos                  |\n";
    pausa();
}

void Juego::mostrarTablero() const {
    limpiarPantalla();
    std::cout << "\033[1m--- TABLERO ---\033[0m\n";
    if (ultimaRondaRevelada) std::cout << "\033[1;33m(Última Ronda activada)\033[0m\n";
    for (int i=0;i<numPilas;++i) {
        if (pilas[i].robada) {
            std::cout << "\033[90mPila " << i << " [ROBADA] ";
            for (int id : pilas[i].cartas) std::cout << color_por_id(id) << (id==99? "★" : "■") << reset_color() << " ";
            std::cout << "\n";
        } else {
            std::cout << "Pila " << i << ": ";
            if (pilas[i].cartas.empty()) std::cout << "(vacía)\n";
            else {
                for (int id : pilas[i].cartas) std::cout << color_por_id(id) << (id==99? "★" : "■") << reset_color() << " ";
                std::cout << "\n";
            }
        }
    }
    std::cout << "\nJugadores:\n";
    for (int i=0;i<numJugadores;++i) {
        std::cout << i << ") " << jugadores[i]->nombre() << " - Puntaje parcial: " << jugadores[i]->puntaje() << (i==jugadorActual? " <- Tu turno":"") << "\n";
    }
    std::cout << "\n";
}

bool Juego::hayPilaConCartas() const {
    for (int i=0;i<numPilas;++i) if (!pilas[i].cartas.empty() && !pilas[i].robada) return true;
    return false;
}

void Juego::robarYColocar() {
    if (!mazo || mazo -> empty()) {
        mostrarResultadosFinales();
        return;
    }
    int carta = mazo->back();
    mazo -> pop_back();
    if (!ultimaRondaRevelada && mazo && (int)mazo->size() == 5) {
        bool tiene99 = false;
        for (int v : *mazo) if (v == 99) { tiene99 = true; break; }
        if (!tiene99) {
            int idx = std::rand() % (int)mazo->size();
            (*mazo)[idx] = 99;
        }
    }
    if (carta == 99 && !ultimaRondaRevelada) {
        ultimaRondaRevelada = true;
        limpiarPantalla();
        std::cout << "\033[1;33m¡HAS ROBADO LA CARTA DE ÚLTIMA RONDA! Se completará la ronda actual.\033[0m\n";
        pausa();
    }
    limpiarPantalla();
    std::cout << "Carta robada: " << color_por_id(carta) << (carta==99? "★" : "■") << reset_color() << "\n";
    while (true) {
        std::cout << "¿En qué pila colocarla (0-" << (numPilas-1) << ")? ";
        int p;

        if (!(std::cin >> p)) {
            std::cin.clear(); 
            std::string tmp; 
            std::getline(std::cin,tmp); 
            limpiarPantalla(); 
            std::cout << "\033[31mDato inválido.\033[0m\n"; 
            continue;
        }
        if (p < 0 || p >= numPilas) { 
            limpiarPantalla(); 
            std::cout << "\033[31mPila inválida.\033[0m\n"; 
            continue; 
        }

        if (pilas[p].robada) { 
            limpiarPantalla(); 
            std::cout << "\033[31mNo se puede colocar en una pila robada.\033[0m\n"; 
            continue; 
        }

        if ((int)pilas[p].cartas.size() >= 3) { 
            limpiarPantalla(); 
            std::cout << "\033[31mLa pila está llena (máximo 3).\033[0m\n"; 
            continue; 
        }
        pilas[p].cartas.push_back(carta);
        break;
    }
    // si todas las pilas (no robadas) están llenas comienza OBLIGAO la fase de robo
    
    bool todasLlenas=true;
    for (int i=0;i<numPilas;++i) if ((int)pilas[i].cartas.size() < 3 && !pilas[i].robada) { 
        todasLlenas=false; 
        break; 
    }
    if (todasLlenas) faseActual = Fase::ROBO;
}

void Juego::tomarPila() {
    if (!hayPilaConCartas()) {
        limpiarPantalla();
        std::cout << "\033[31mTodas las pilas están vacías, por favor robar una carta.\033[0m\n";
        pausa();
        return;
    }
    while (true) {
        std::cout << "¿Qué pila tomar (0-" << (numPilas-1) << ")? ";
        int p;
        if (!(std::cin >> p)) { 
            std::cin.clear(); 
            std::string tmp; 
            std::getline(std::cin,tmp); 
            limpiarPantalla(); 
            std::cout << "\033[31mDato inválido.\033[0m\n"; 
            continue; 
        }
        if (p < 0 || p >= numPilas) { 
            limpiarPantalla(); 
            std::cout << "\033[31mPila inválida.\033[0m\n"; 
            continue; 
        }
        if (pilas[p].robada) { 
            limpiarPantalla(); 
            std::cout << "\033[31mEsa pila ya fue robada.\033[0m\n"; 
            continue; 
        }
        if (pilas[p].cartas.empty()) {
            limpiarPantalla();
            std::cout << "\033[31mNo se puede tomar una pila vacía.\033[0m\n";
            if (hayPilaConCartas()) { std::cout << "Por favor elige otra pila.\n"; continue; }
            else { std::cout << "No hay pilas con cartas. Volviendo al menú principal.\n"; pausa(); return; }
        }
        // dar cartas al jugador actual

        jugadores[jugadorActual] -> agregarCartas(pilas[p].cartas);
        pilas[p].cartas.clear();
        pilas[p].robada = true;

        // revisar si todas robadas
        bool todasRobadas=true;
        for (int i=0;i<numPilas;++i) if (!pilas[i].robada) { 
            todasRobadas=false; 
            break; 
        }
        if (todasRobadas) {
            if (ultimaRondaRevelada) {
                mostrarResultadosFinales();
                return;
            } else {
                iniciarNuevaRonda();
            }
        }
        break;
    }
}

void Juego::iniciarNuevaRonda() {
    for (int i=0; i < numPilas; ++i){
        pilas[i].robada=false; 
        pilas[i].cartas.clear(); 
    }
    faseActual = Fase::COLOCACION;
    limpiarPantalla();
    std::cout << "\033[32mComienza una nueva ronda...\033[0m\n";
    pausa();
}

void Juego::mostrarResultadosFinales() const {
    limpiarPantalla();
    std::vector<std::pair<std::string,int>> res;
    for (int i = 0; i < numJugadores; ++i) res.push_back({jugadores[i]->nombre(), jugadores[i]->puntaje()});
    std::sort(res.begin(), res.end(), [](const auto &a,const auto &b){ return a.second > b.second; });
    std::cout << "\033[1m=== RESULTADOS FINALES ===\033[0m\n\n";
    for (auto &p : res) std::cout << p.first << " -> " << p.second << " puntos\n";
    std::cout << "\n\033[1;33m🏆 ¡GANADOR: " << res.front().first << "!\033[0m\n";
    pausa();    
    exit(0);
}

void Juego::iniciar() {
    while (true) {
        mostrarTablero();
        std::cout << "Jugador actual: " << jugadores[jugadorActual]->nombre() << "\n";
        if (faseActual == Fase::COLOCACION) {
            std::cout << "Opciones:\n1) Robar carta y colocar\n2) Tomar pila\n3) Guardar partida\n4) Salir al menú principal\nElija: ";
        } else {
            std::cout << "\033[33mFase de robo: debes tomar una pila disponible.\033[0m\n";
            std::cout << "Opciones:\n2) Tomar pila\n3) Guardar partida\n4) Salir al menú principal\nElija: ";
        }
        int opt;
        if (!(std::cin >> opt)) {
            std::cin.clear(); 
            std::string tmp; 
            std::getline(std::cin,tmp); 
            limpiarPantalla(); 
            std::cout << "\033[31mDato inválido.\033[0m\n"; 
            continue;
        }

        if (faseActual == Fase::COLOCACION && opt == 1) {
            robarYColocar();
            jugadorActual = (jugadorActual + 1) % numJugadores;
        } else if (opt == 2) {
            tomarPila();
            jugadorActual = (jugadorActual + 1) % numJugadores;
        } else if (opt == 3) {
            std::string archivo; 
            std::cout << "Nombre archivo para guardar: "; 
            std::cin >> archivo;
            if (guardar(archivo)) { 
                std::cout << "\033[32mGuardado OK.\033[0m\n"; 
                pausa(); 
            }
            else { 
                std::cout << "\033[31mError guardando.\033[0m\n"; 
                pausa(); 
            }
        } else if (opt == 4) {
            limpiarPantalla();
            return;
        } else {
            limpiarPantalla();
            std::cout << "\033[31mOpción inválida.\033[0m\n";
        }
    }
}

std::string xorCrypt(const std::string &input, char key = 'K') {
    std::string output = input;
    for (size_t i = 0; i < input.size(); i++) {
        output[i] = input[i] ^ key;  // XOR con la clave K
    }
    return output;
}

bool Juego::guardar(const std::string& archivo) const {
    std::ostringstream buffer;

    buffer << (faseActual == Fase::COLOCACION ? 0 : 1) << "\n";
    buffer << jugadorActual << "\n";
    buffer << (ultimaRondaRevelada ? 1 : 0) << "\n";

    // Obtenemos datos del mazo
    for (size_t i = 0; i < mazo->size(); ++i) {
        if (i) buffer << ",";
        buffer << (*mazo)[i];
    }
    buffer << "\n";

    // Obtenemos datos de las Pilas
    for (int i = 0; i < numPilas; ++i) {
        buffer << (pilas[i].robada ? 1 : 0) << "|";
        for (size_t j = 0; j < pilas[i].cartas.size(); ++j) {
            if (j) buffer << ",";
            buffer << pilas[i].cartas[j];
        }
        buffer << "\n";
    }
    // Obtenemos datos de los Jugadores
    buffer << numJugadores << "\n";
    for (int i = 0; i < numJugadores; ++i)
        buffer << jugadores[i]->serializar() << "\n";

    // Ciframos con el XOR
    std::string contenidoCodificado = xorCrypt(buffer.str());

    // Guardar en archivo
    std::ofstream ofs(archivo, std::ios::binary);
    if (!ofs) return false;
    ofs << contenidoCodificado;
    ofs.close();

    return true;
}

bool Juego::cargar(const std::string& archivo) {
    std::ifstream ifs(archivo, std::ios::binary);
    if (!ifs) return false;

    std::ostringstream buffer;
    buffer << ifs.rdbuf();
    std::string contenidoCodificado = buffer.str();

    std::string contenidoDecodificado = xorCrypt(contenidoCodificado);

    std::istringstream in(contenidoDecodificado);
    std::string linea;

    if (!std::getline(in, linea)) return false;
    faseActual = (linea == "0" ? Fase::COLOCACION : Fase::ROBO);

    if (!std::getline(in, linea)) return false;
    jugadorActual = std::stoi(linea);

    if (!std::getline(in, linea)) return false;
    ultimaRondaRevelada = (std::stoi(linea) != 0);

    // Extraemos el mazo
    if (!std::getline(in, linea)) return false;
    if (mazo) { delete mazo; mazo = nullptr; }
    mazo = new std::vector<int>();
    std::istringstream ss(linea);
    std::string tok;
    while (std::getline(ss, tok, ',')) if (!tok.empty()) mazo->push_back(std::stoi(tok));

    // Extraemos las pilas
    if (!pilas) pilas = new Pila[numPilas];
    for (int i = 0; i < numPilas; ++i) {
        if (!std::getline(in, linea)) return false;
        auto pos = linea.find('|');
        if (pos == std::string::npos) return false;
        pilas[i].robada = (std::stoi(linea.substr(0, pos)) != 0);
        pilas[i].cartas.clear();
        std::string rest = linea.substr(pos + 1);
        if (!rest.empty()) {
            std::istringstream ss2(rest); std::string t2;
            while (std::getline(ss2, t2, ',')) if (!t2.empty()) pilas[i].cartas.push_back(std::stoi(t2));
        }
    }

    // Extraemos los jugadores
    if (!std::getline(in, linea)) return false;
    int nj = std::stoi(linea);
    if (jugadores) { for (int i=0;i<numJugadores;++i) delete jugadores[i]; delete[] jugadores; jugadores=nullptr; }
    jugadores = new Jugador*[nj];
    numJugadores = nj;
    for (int i = 0; i < nj; ++i) {
        if (!std::getline(in, linea)) return false;
        Jugador* j = new Jugador("");
        if (!j->deserializar(linea)) { delete j; return false; }
        jugadores[i] = j;
    }

    return true;
}
