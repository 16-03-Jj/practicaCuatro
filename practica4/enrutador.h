#ifndef ENRUTADOR_H
#define ENRUTADOR_H

#include <string>
#include <map>

using namespace std;

// Un enrutador solo conoce INFORMACION LOCAL: su nombre, sus vecinos
// directos (con el costo del enlace) y su tabla de costos a destinos.
// No guarda punteros a otros enrutadores: la red es quien los conecta.
class Enrutador {
private:
    string nombre;
    map<string, int> vecinos;      // vecino -> costo del enlace directo
    map<string, int> tablaCostos;  // destino -> costo conocido

public:
    Enrutador();                       // requerido por map::operator[]
    explicit Enrutador(const string &nombre);

    // Consultas
    string getNombre() const;
    const map<string, int>& getVecinos() const;
    const map<string, int>& getTablaCostos() const;
    bool esVecino(const string &otro) const;
    int  getCosto(const string &destino) const;   // -1 si no hay ruta conocida

    // Gestion de vecinos
    void agregarVecino(const string &otro, int costo);   // agrega o actualiza
    bool eliminarVecino(const string &otro);

    // Gestion de la tabla de costos (la actualiza la Red)
    void inicializarTabla();                              // 0 a si mismo + vecinos directos
    void actualizarTabla(const map<string, int> &nueva);  // reemplaza con info de la Red
    void mostrarTabla() const;
};

#endif
