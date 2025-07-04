#ifndef F06_MAIN_SECUENCIAL_H
#define F06_MAIN_SECUENCIAL_H

#include "../resources.h"
#include "F01_archivo.h"
#include "F04_comparar.h"
#include "F05_proceso.h"
#include "F08_temporizador.h"

Temporizador mainSecuencial(int copias)
{
    cout << endl;

    Temporizador temporizador_principal;
    const string rutaTrabajo = "file_workspace_sequential/";

    cout << "================================" << endl;
    cout << "    INICIO PROCESO SECUENCIAL   " << endl;
    cout << "================================" << endl;
    cout << "Tiempo Inicial:     " << temporizador_principal.formatoTextoInicio() << endl;
    cout << "================================" << endl;
    cout << endl;

    for (int i = 1; i <= copias; i++)
    {
        proceso(rutaTrabajo, i);
        temporizador_principal.registrar();
        string duracion = temporizador_principal.duracionEntre(i - 1, i);
        if (i < 10)
        {
            cout << "TIEMPO PROCESO " << i << ":  " << duracion << endl;
        }
        else
        {
            cout << "TIEMPO PROCESO " << i << ": " << duracion << endl;
        }
        cout << "--------------------------------" << endl;
    }

    temporizador_principal.detener();

    cout << "================================" << endl;
    cout << "      FIN PROCESO SECUENCIAL    " << endl;
    cout << "================================" << endl;
    cout << "Tiempo Final:       " << temporizador_principal.formatoTextoFin() << endl;
    cout << "Tiempo Total:       " << temporizador_principal.tiempoTranscurrido() << endl;
    cout << "Tiempo promedio:    " << temporizador_principal.promedioPorProceso() << endl;
    cout << "================================" << endl;
    return temporizador_principal;
}

#endif // F06_MAIN_SECUENCIAL_H
