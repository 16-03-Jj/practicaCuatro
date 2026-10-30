#include "Enrutador.h"
#include <iostream>

Enrutador::Enrutador() : nombre("") {}

Enrutador::Enrutador(const string &nombre) : nombre(nombre) {
    inicializarTabla();
}

string Enrutador::getNombre() const { return nombre; }
const map<string, int>& Enrutador::getVecinos() const { return vecinos; }
const map<string, int>& Enrutador::getTablaCostos() const { return tablaCostos; }

bool Enrutador::esVecino(const string &otro) const {
    return vecinos.find(otro) != vecinos.end();
}

int Enrutador::getCosto(const string &destino) const {
    map<string, int>::const_iterator it = tablaCostos.find(destino);
    return (it == tablaCostos.end()) ? -1 : it->second;
}

const list<string>* Enrutador::getCamino(const string &destino) const {
    map<string, list<string> >::const_iterator it = caminos.find(destino);
    return (it == caminos.end()) ? 0 : &it->second;
}

string Enrutador::siguienteSalto(const string &destino) const {
    const list<string> *c = getCamino(destino);
    if (!c || c->size() < 2) return "";          // sin ruta, o destino = yo mismo
    list<string>::const_iterator it = c->begin();
    ++it;                                         // el primero es este enrutador
    return *it;
}

void Enrutador::agregarVecino(const string &otro, int costo) {
    vecinos[otro] = costo;   // si ya existia, solo actualiza el costo
}

bool Enrutador::eliminarVecino(const string &otro) {
    return vecinos.erase(otro) > 0;
}

// Estado inicial al encenderse: solo conoce lo directamente conectado.
void Enrutador::inicializarTabla() {
    tablaCostos.clear();
    caminos.clear();
    tablaCostos[nombre] = 0;
    caminos[nombre].push_back(nombre);
    for (map<string, int>::const_iterator it = vecinos.begin(); it != vecinos.end(); ++it) {
        tablaCostos[it->first] = it->second;
        caminos[it->first].push_back(nombre);
        caminos[it->first].push_back(it->first);
    }
}

void Enrutador::actualizarTabla(const map<string, int> &costos,
                                const map<string, list<string> > &nuevosCaminos) {
    tablaCostos = costos;
    caminos = nuevosCaminos;
}

void Enrutador::mostrarTabla() const {
    cout << "Tabla de " << nombre << ":" << endl;
    for (map<string, int>::const_iterator it = tablaCostos.begin(); it != tablaCostos.end(); ++it) {
        cout << "  " << it->first << "  costo=" << it->second << "  camino: ";
        const list<string> *c = getCamino(it->first);
        if (c) {
            for (list<string>::const_iterator p = c->begin(); p != c->end(); ++p) {
                if (p != c->begin()) cout << " -> ";
                cout << *p;
            }
        }
        cout << endl;
    }
}
