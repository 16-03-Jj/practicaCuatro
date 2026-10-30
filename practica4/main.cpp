#include <iostream>
#include <list>
#include <string>
#include <limits>
#include "Red.h"

using namespace std;

static void imprimirCamino(const list<string> &c) {
    for (list<string>::const_iterator it = c.begin(); it != c.end(); ++it) {
        if (it != c.begin()) cout << " -> ";
        cout << *it;
    }
}

// Lee una linea completa (permite espacios) y quita comillas si pegaste
// la ruta con "Copiar como ruta" de Windows.
static string leerLinea() {
    string s;
    cin >> ws;               // descarta el salto de linea pendiente
    getline(cin, s);
    if (!s.empty() && s[s.size() - 1] == '\r') s.erase(s.size() - 1);
    if (s.size() >= 2 && s[0] == '"' && s[s.size() - 1] == '"')
        s = s.substr(1, s.size() - 2);
    return s;
}

static void menu() {
    cout << "\n===== SIMULADOR DE RED DE ENRUTADORES =====\n"
         << " 1. Cargar topologia desde archivo\n"
         << " 2. Guardar topologia en archivo\n"
         << " 3. Agregar enrutador\n"
         << " 4. Eliminar enrutador\n"
         << " 5. Agregar/actualizar enlace\n"
         << " 6. Eliminar enlace\n"
         << " 7. Mostrar red (vecinos)\n"
         << " 8. Mostrar tabla de un enrutador\n"
         << " 9. Mostrar matriz de costos\n"
         << "10. Camino y costo entre dos enrutadores\n"
         << " 0. Salir\n"
         << "Opcion: ";
}

static void cargar(Red &red, const string &ruta) {
    int malas = 0;
    if (!red.cargarDesdeArchivo(ruta, malas))
        cout << "No se pudo abrir '" << ruta << "'." << endl;
    else
        cout << "Red cargada: " << red.getNumEnrutadores() << " enrutadores, "
             << malas << " linea(s) ignorada(s)." << endl;
}

int main(int argc, char *argv[]) {
    Red red;
    if (argc > 1) cargar(red, argv[1]);

    int op = -1;
    while (op != 0) {
        menu();
        if (!(cin >> op)) {
            if (cin.eof()) break;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            op = -1;
            cout << "Entrada invalida." << endl;
            continue;
        }
        string a, b;
        int costo;
        switch (op) {
        case 1:
            cout << "Archivo: "; a = leerLinea();
            cargar(red, a);
            break;
        case 2:
            cout << "Archivo: "; a = leerLinea();
            cout << (red.guardarEnArchivo(a) ? "Guardado." : "No se pudo escribir.") << endl;
            break;
        case 3:
            cout << "Nombre: "; cin >> a;
            cout << (red.agregarEnrutador(a) ? "Agregado." : "Nombre invalido o ya existe.") << endl;
            break;
        case 4:
            cout << "Nombre: "; cin >> a;
            cout << (red.eliminarEnrutador(a) ? "Eliminado." : "No existe.") << endl;
            break;
        case 5:
            cout << "Enrutador A, B y costo: ";
            if (!(cin >> a >> b >> costo)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "Costo invalido." << endl; break; }
            cout << (red.agregarEnlace(a, b, costo) ? "Enlace listo." : "Datos invalidos (existen? costo>0? A!=B?).") << endl;
            break;
        case 6:
            cout << "Enrutador A y B: "; cin >> a >> b;
            cout << (red.eliminarEnlace(a, b) ? "Enlace eliminado." : "Ese enlace no existe.") << endl;
            break;
        case 7:
            red.mostrarRed();
            break;
        case 8: {
            cout << "Enrutador: "; cin >> a;
            const Enrutador *e = red.getEnrutador(a);
            if (e) e->mostrarTabla(); else cout << "No existe." << endl;
            break;
        }
        case 9:
            red.mostrarMatrizCostos();
            break;
        case 10: {
            cout << "Origen y destino: "; cin >> a >> b;
            list<string> camino;
            int c = red.encontrarCamino(a, b, camino);
            if (c < 0) cout << "No hay camino (o algun enrutador no existe)." << endl;
            else { cout << "Camino: "; imprimirCamino(camino); cout << "\nCosto total: " << c << endl; }
            break;
        }
        case 0:
            cout << "Hasta luego." << endl;
            break;
        default:
            cout << "Opcion no valida." << endl;
        }
    }
    return 0;
}