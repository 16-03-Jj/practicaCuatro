#include <iostream>
#include "Red.h"

int main() {
    Red red;
    red.agregarEnrutador("A"); red.agregarEnrutador("B");
    red.agregarEnrutador("C"); red.agregarEnrutador("D");

    // Topologia de la Figura 1 de la guia
    red.agregarEnlace("A", "B", 4);  red.agregarEnlace("A", "C", 10);
    red.agregarEnlace("A", "D", 5);  red.agregarEnlace("B", "C", 3);
    red.agregarEnlace("B", "D", 1);  red.agregarEnlace("C", "D", 2);

    red.mostrarRed();
    red.getEnrutador("A")->mostrarTabla();

    cout << "\n-- Eliminando el enlace A-C --\n";
    red.eliminarEnlace("A", "C");
    red.getEnrutador("A")->mostrarTabla();

    cout << "\n-- Eliminando el enrutador D --\n";
    red.eliminarEnrutador("D");
    red.mostrarRed();
    return 0;
}