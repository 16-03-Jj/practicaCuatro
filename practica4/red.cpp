#include "Red.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <vector>
#include <iomanip>
#include <cctype>

Red::Red() {}

// ---------------------------------------------------------------- topologia
bool Red::agregarEnrutador(const string &nombre) {
    if (nombre.empty() || existeEnrutador(nombre)) return false;
    enrutadores[nombre] = Enrutador(nombre);
    actualizarTablas();
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

void Red::limpiar() { enrutadores.clear(); }

// ----------------------------------------------------------------- consultas
bool Red::existeEnrutador(const string &nombre) const {
    return enrutadores.find(nombre) != enrutadores.end();
}

const Enrutador* Red::getEnrutador(const string &nombre) const {
    map<string, Enrutador>::const_iterator it = enrutadores.find(nombre);
    return (it == enrutadores.end()) ? 0 : &it->second;
}

int Red::getNumEnrutadores() const { return (int)enrutadores.size(); }

// ------------------------------------------------------------------ Dijkstra
void Red::dijkstra(const string &origen, map<string, int> &dist,
                   map<string, string> &prev) const {
    dist.clear();
    prev.clear();

    typedef pair<int, string> Par;                       // (costo, nodo)
    priority_queue<Par, vector<Par>, greater<Par> > cola; // min-heap

    dist[origen] = 0;
    cola.push(Par(0, origen));

    while (!cola.empty()) {
        Par tope = cola.top();
        cola.pop();
        int d = tope.first;
        const string u = tope.second;

        if (d > dist[u]) continue;        // entrada vieja de la cola: ignorar

        const Enrutador *e = getEnrutador(u);
        if (!e) continue;
        const map<string, int> &vec = e->getVecinos();
        for (map<string, int>::const_iterator it = vec.begin(); it != vec.end(); ++it) {
            int nd = d + it->second;
            map<string, int>::iterator f = dist.find(it->first);
            if (f == dist.end() || nd < f->second) {      // relajacion
                dist[it->first] = nd;
                prev[it->first] = u;
                cola.push(Par(nd, it->first));
            }
        }
    }
}

void Red::actualizarTablas() {
    for (map<string, Enrutador>::iterator it = enrutadores.begin(); it != enrutadores.end(); ++it) {
        map<string, int> dist;
        map<string, string> prev;
        dijkstra(it->first, dist, prev);

        // Reconstruir el camino completo a cada destino siguiendo 'prev'
        // hacia atras (destino -> origen) e insertando al frente de la lista.
        map<string, list<string> > caminos;
        for (map<string, int>::const_iterator d = dist.begin(); d != dist.end(); ++d) {
            list<string> c;
            string actual = d->first;
            c.push_front(actual);
            while (actual != it->first) {
                actual = prev[actual];
                c.push_front(actual);
            }
            caminos[d->first] = c;
        }
        it->second.actualizarTabla(dist, caminos);
    }
}

int Red::encontrarCamino(const string &origen, const string &destino,
                         list<string> &camino) const {
    camino.clear();
    const Enrutador *o = getEnrutador(origen);
    if (!o || !existeEnrutador(destino)) return -1;
    int costo = o->getCosto(destino);
    if (costo < 0) return -1;                       // sin ruta (red desconectada)
    camino = *o->getCamino(destino);
    return costo;
}

// ------------------------------------------------------------------ archivos
// Formato (una instruccion por linea, '#' inicia un comentario):
//   ROUTER <nombre>
//   ENLACE <a> <b> <costo>
bool Red::cargarDesdeArchivo(const string &ruta, int &lineasInvalidas) {
    lineasInvalidas = 0;
    ifstream f(ruta.c_str());
    if (!f.is_open()) return false;

    limpiar();
    string linea;
    int n = 0;
    while (getline(f, linea)) {
        ++n;
        size_t c = linea.find('#');
        if (c != string::npos) linea = linea.substr(0, c);

        istringstream ss(linea);
        string cmd;
        if (!(ss >> cmd)) continue;                  // linea vacia
        for (size_t i = 0; i < cmd.size(); ++i) cmd[i] = (char)toupper((unsigned char)cmd[i]);

        bool ok = false;
        if (cmd == "ROUTER") {
            string nombre, extra;
            if ((ss >> nombre) && !(ss >> extra) && !existeEnrutador(nombre) && !nombre.empty())
            { enrutadores[nombre] = Enrutador(nombre); ok = true; }
        } else if (cmd == "ENLACE") {
            string a, b, extra;
            int costo;
            if ((ss >> a >> b >> costo) && !(ss >> extra) &&
                a != b && costo > 0 && existeEnrutador(a) && existeEnrutador(b)) {
                enrutadores[a].agregarVecino(b, costo);
                enrutadores[b].agregarVecino(a, costo);
                ok = true;
            }
        }
        if (!ok) {
            ++lineasInvalidas;
            cerr << "  [aviso] " << ruta << ", linea " << n << " ignorada: " << linea << endl;
        }
    }
    actualizarTablas();   // una sola vez al final
    return true;
}

bool Red::guardarEnArchivo(const string &ruta) const {
    ofstream f(ruta.c_str());
    if (!f.is_open()) return false;
    f << "# Topologia generada por el programa\n";
    for (map<string, Enrutador>::const_iterator it = enrutadores.begin(); it != enrutadores.end(); ++it)
        f << "ROUTER " << it->first << "\n";
    for (map<string, Enrutador>::const_iterator it = enrutadores.begin(); it != enrutadores.end(); ++it) {
        const map<string, int> &v = it->second.getVecinos();
        for (map<string, int>::const_iterator j = v.begin(); j != v.end(); ++j)
            if (it->first < j->first)               // cada enlace una sola vez
                f << "ENLACE " << it->first << " " << j->first << " " << j->second << "\n";
    }
    return true;
}

// -------------------------------------------------------------------- salida
void Red::mostrarRed() const {
    if (enrutadores.empty()) { cout << "(red vacia)" << endl; return; }
    for (map<string, Enrutador>::const_iterator it = enrutadores.begin(); it != enrutadores.end(); ++it) {
        cout << it->first << " -> ";
        const map<string, int> &v = it->second.getVecinos();
        for (map<string, int>::const_iterator j = v.begin(); j != v.end(); ++j)
            cout << j->first << "(" << j->second << ") ";
        cout << endl;
    }
}

void Red::mostrarMatrizCostos() const {
    if (enrutadores.empty()) { cout << "(red vacia)" << endl; return; }
    cout << setw(4) << " ";
    for (map<string, Enrutador>::const_iterator c = enrutadores.begin(); c != enrutadores.end(); ++c)
        cout << setw(5) << c->first;
    cout << endl;
    for (map<string, Enrutador>::const_iterator r = enrutadores.begin(); r != enrutadores.end(); ++r) {
        cout << setw(4) << r->first;
        for (map<string, Enrutador>::const_iterator c = enrutadores.begin(); c != enrutadores.end(); ++c) {
            int costo = r->second.getCosto(c->first);
            if (costo < 0) cout << setw(5) << "-";
            else           cout << setw(5) << costo;
        }
        cout << endl;
    }
}