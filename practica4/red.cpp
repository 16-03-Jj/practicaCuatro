#include "Red.h"
#include <iostream>

Red::Red() {}

bool Red::agregarEnrutador(const string &nombre) {
    if (nombre.empty() || existeEnrutador(nombre)) return false;
    enrutadores[nombre] = Enrutador(nombre);
    return true;
}

bool Red::eliminarEnrutador(const string &nombre) {
    if (!existeEnrutador(nombre)) return false;
    // Quitar el enlace en todos los vecinos antes de borrarlo
    for (map<string, Enrutador>::iterator it = enrutadores.begin(); it != enrutadores.end(); ++it)
        it->second.eliminarVecino(nombre);
    enrutadores.erase(nombre);
    actualizarTablas();   // la topologia cambio
    return true;
}

bool Red::agregarEnlace(const string &a, const string &b, int costo) {
    if (a == b || costo <= 0) return false;
    if (!existeEnrutador(a) || !existeEnrutador(b)) return false;
    enrutadores[a].agregarVecino(b, costo);
    enrutadores[b].agregarVecino(a, costo);
    actualizarTablas();
    return true;
}

bool Red::eliminarEnlace(const string &a, const string &b) {
    if (!existeEnrutador(a) || !existeEnrutador(b)) return false;
    bool ok = enrutadores[a].eliminarVecino(b);
    ok = enrutadores[b].eliminarVecino(a) && ok;
    if (ok) actualizarTablas();
    return ok;
}

bool Red::existeEnrutador(const string &nombre) const {
    return enrutadores.find(nombre) != enrutadores.end();
}

const Enrutador* Red::getEnrutador(const string &nombre) const {
    map<string, Enrutador>::const_iterator it = enrutadores.find(nombre);
    return (it == enrutadores.end()) ? 0 : &it->second;
}

int Red::getNumEnrutadores() const { return (int)enrutadores.size(); }

// PARCIAL: por ahora cada tabla solo tiene los costos directos.
// En la siguiente entrega aqui se calculan los caminos mas cortos.
void Red::actualizarTablas() {
    for (map<string, Enrutador>::iterator it = enrutadores.begin(); it != enrutadores.end(); ++it)
        it->second.inicializarTabla();
}

void Red::mostrarRed() const {
    for (map<string, Enrutador>::const_iterator it = enrutadores.begin(); it != enrutadores.end(); ++it) {
        cout << it->first << " -> ";
        const map<string, int> &v = it->second.getVecinos();
        for (map<string, int>::const_iterator j = v.begin(); j != v.end(); ++j)
            cout << j->first << "(" << j->second << ") ";
        cout << endl;
    }
}
