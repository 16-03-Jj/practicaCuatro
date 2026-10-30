#ifndef ENRUTADOR_H
#define ENRUTADOR_H

#include <string>
#include <map>
#include <list>

using namespace std;

// Un enrutador solo conoce INFORMACION LOCAL: su nombre, sus vecinos
// directos (con el costo del enlace), su tabla de costos y los caminos
// completos hacia cada destino. No guarda punteros a otros enrutadores:
// la red es quien los conecta y quien calcula las rutas.
class Enrutador {
private:
    string nombre;
    map<string, int> vecinos;              // vecino  -> costo del enlace directo
    map<string, int> tablaCostos;          // destino -> costo del mejor camino
    map<string, list<string> > caminos;    // destino -> camino completo (origen..destino)

public:
    Enrutador();                           // requerido por map::operator[]
    explicit Enrutador(const string &nombre);

    // Consultas
    string getNombre() const;
    const map<string, int>& getVecinos() const;
    const map<string, int>& getTablaCostos() const;
    bool esVecino(const string &otro) const;
    int  getCosto(const string &destino) const;              // -1 si no hay ruta
    const list<string>* getCamino(const string &destino) const; // 0 si no hay ruta
    string siguienteSalto(const string &destino) const;      // "" si no hay ruta

    // Gestion de vecinos
    void agregarVecino(const string &otro, int costo);       // agrega o actualiza
    bool eliminarVecino(const string &otro);

    // Gestion de la tabla (la actualiza la Red)
    void inicializarTabla();                                 // 0 a si mismo + vecinos directos
    void actualizarTabla(const map<string, int> &costos,
                         const map<string, list<string> > &nuevosCaminos);
    void mostrarTabla() const;
};

#endif
