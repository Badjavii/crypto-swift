#ifndef F06_MAIN_SECUENCIAL_H
#define F06_MAIN_SECUENCIAL_H
#include "../resources.h"
#include "F01_archivo.h"
#include "F04_comparar.h"
#include "F05_proceso.h"
#include "F08_temporizador.h"

void mainSecuencial(int copias)
{
    const string rutaTrabajo = "file_workspace/";
    const string archivoOriginal = rutaTrabajo + "original.txt", extensionCopia = ".txt", extensionEncriptado = ".sha", extensionDesencriptado = ".des";
    string archivoCopia, archivoEncriptado, archivoDesencriptado;
    string hash1, hash2;
    bool resultadoComparacion;

    cout << endl;

    Temporizador temporizador_principal = Temporizador();

    cout << "==========================" << endl;
    cout << "PROCESO SECUENCIAL" << endl;
    cout << "==========================" << endl;
    cout << "Tiempo Inicial: " << temporizador_principal.formatoTextoInicio() << endl;
    cout << "==========================" << endl;

    cout << endl;

    for (int i = 1; i <= copias; i++)
    {
        proceso(i);
    }

    temporizador_principal.detener();
    cout << "==========================" << endl;
    cout << "FIN PROCESO SECUENCIAL" << endl;
    cout << "==========================" << endl;
    cout << "Tiempo promedio:    " << endl;
    cout << "Tiempo Total:       " << temporizador_principal.formatoTextoFin() << endl;
    cout << "==========================" << endl;
    cout << "Tiempo en segundos: " << temporizador_principal.formatoTextoDuracion() << endl 
}

#endif // F06_MAIN_SECUENCIAL_H