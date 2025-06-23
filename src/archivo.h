#ifndef ARCHIVO_H
#define ARCHIVO_H
#include <fstream>
#include <string>
#include "encriptacion.h"
using namespace std;

string generarNombreCopia(int n){
    return "copia" + to_string(n) + ".txt";
}

string generarNombreCopiaEncriptada(string nombre){
    return "encriptado_" + nombre;
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

void encriptarArchivo(string nombreArchivoEntrada) {
    ifstream entrada(nombreArchivoEntrada);
    ofstream salida(generarNombreCopiaEncriptada(nombreArchivoEntrada));

    if (entrada.is_open() && salida.is_open()) {
        string linea;
        while (getline(entrada, linea)) {
            string lineaEncriptada = encriptarLinea(linea);
            salida << lineaEncriptada << endl;
        }
        entrada.close();
        salida.close();
    }
}

void desencriptarArchivo()


#endif // ARCHIVO_H
