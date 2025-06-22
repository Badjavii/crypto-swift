#ifndef ARCHIVO_H
#define ARCHIVO_H
#include <fstream>
#include <string>
using namespace std;

string generarNombreCopia(int n){
    return "copia" + to_string(n) + ".txt";
}

void generarCopia(string archivoNombre, int n){
    ifstream archivo(archivoNombre);
    string copiaNombre = generarNombreCopia(n);
    ofstream copia(copiaNombre);
    
    if (archivo.is_open() && copia.is_open()) {
        string linea;
        while (getline(archivo, linea)) {
            copia << linea << endl;
        }
        archivo.close();
        copia.close();
    }
}

#endif // ARCHIVO_H
