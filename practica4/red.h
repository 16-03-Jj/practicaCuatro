#ifndef RED_H
#define RED_H

#include <string>
#include <map>
#include <list>
#include "Enrutador.h"

using namespace std;

// La Red es la duena de los enrutadores (relacion "es parte de") y la
// unica que sabe como estan conectados. Los enlaces son bidireccionales.
// Modelo centralizado: la Red calcula los caminos minimos (Dijkstra) y
// entrega a cada enrutador su tabla de costos y sus caminos.
class Red {
private:
    map<string, Enrutador> enrutadores;   // nombre -> enrutador

    // Dijkstra desde 'origen'. dist: costo minimo a cada alcanzable;
    // prev: predecesor de cada nodo en su camino minimo.
    void dijkstra(const string &origen, map<string, int> &dist,
                  map<string, string> &prev) const;

public:
    Red();

    // Gestion de enrutadores y enlaces (en tiempo de ejecucion)
    bool agregarEnrutador(const string &nombre);
    bool eliminarEnrutador(const string &nombre);
    bool agregarEnlace(const string &a, const string &b, int costo);
    bool eliminarEnlace(const string &a, const string &b);
    void limpiar();

    // Consultas
    bool existeEnrutador(const string &nombre) const;
    const Enrutador* getEnrutador(const string &nombre) const;
    int  getNumEnrutadores() const;

    // Camino entre dos enrutadores: devuelve el costo (-1 si no existe)
    // y llena 'camino' con la lista ordenada de nodos origen..destino.
    int encontrarCamino(const string &origen, const string &destino,
                        list<string> &camino) const;

    // Recalcula la tabla de TODOS los enrutadores
    void actualizarTablas();

    // Archivos
    bool cargarDesdeArchivo(const string &ruta, int &lineasInvalidas);
    bool guardarEnArchivo(const string &ruta) const;

    // Salida
    void mostrarRed() const;
    void mostrarMatrizCostos() const;
};

#endif
