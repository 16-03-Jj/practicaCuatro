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

void Enrutador::agregarVecino(const string &otro, int costo) {
    vecinos[otro] = costo;   // si ya existia, solo actualiza el costo
}

bool Enrutador::eliminarVecino(const string &otro) {
    return vecinos.erase(otro) > 0;
}

void Enrutador::inicializarTabla() {
    tablaCostos.clear();
    tablaCostos[nombre] = 0;
    for (map<string, int>::const_iterator it = vecinos.begin(); it != vecinos.end(); ++it)
        tablaCostos[it->first] = it->second;
}

void Enrutador::actualizarTabla(const map<string, int> &nueva) {
    tablaCostos = nueva;
}

void Enrutador::mostrarTabla() const {
    cout << "Tabla de " << nombre << ": ";
    for (map<string, int>::const_iterator it = tablaCostos.begin(); it != tablaCostos.end(); ++it)
        cout << it->first << "=" << it->second << "  ";
    cout << endl;
}
