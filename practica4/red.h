#ifndef RED_H
#define RED_H

#include <string>
#include <map>
#include "Enrutador.h"

using namespace std;

// La Red es la dueña de los enrutadores (relacion "es parte de") y la
// unica que sabe como estan conectados. Los enlaces son bidireccionales.
class Red {
private:
    map<string, Enrutador> enrutadores;   // nombre -> enrutador

public:
    Red();

    // Gestion de enrutadores y enlaces (en tiempo de ejecucion)
    bool agregarEnrutador(const string &nombre);
    bool eliminarEnrutador(const string &nombre);
    bool agregarEnlace(const string &a, const string &b, int costo);
    bool eliminarEnlace(const string &a, const string &b);

    // Consultas
    bool existeEnrutador(const string &nombre) const;
    const Enrutador* getEnrutador(const string &nombre) const;
    int  getNumEnrutadores() const;

    // Actualizacion de tablas (parcial: solo costos directos por ahora)
    void actualizarTablas();
    void mostrarRed() const;
};

#endif
