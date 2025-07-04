#ifndef F06_MAIN_PARALELO_H
#define F06_MAIN_PARALELO_H

#include "../resources.h"
#include "F01_archivo.h"
#include "F04_comparar.h"
#include "F05_proceso.h"
#include "F08_temporizador.h"

const string rutaTrabajo = "file_workspace_parallel/";

// Función que crea y devuelve un hilo para ejecutar un proceso
thread crearHiloDeProceso(int id, Temporizador &temporizador, mutex &mutex_temporizador, vector<pair<int, string>> &tiempos_terminados)
{
    return thread([id, &temporizador, &mutex_temporizador, &tiempos_terminados]()
                  {
        // Ejecutar el proceso correspondiente
        proceso(rutaTrabajo, id);

        // Proteger acceso compartido
        lock_guard<mutex> lock(mutex_temporizador);

        // Registrar el tiempo de finalización del proceso
        temporizador.registrar();

        // Calcular la duración entre la última marca y la anterior
        int n = static_cast<int>(temporizador.getRegistro().size());
        if (n >= 2) {
            string duracion = temporizador.duracionEntre(n - 2, n - 1);
            tiempos_terminados.push_back({id, duracion});
        } });
}

Temporizador mainParalelo(int copias)
{
    cout << endl;

    Temporizador temporizador_principal;
    mutex mutex_temporizador;

    cout << "================================" << endl;
    cout << "    INICIO PROCESO PARALELO     " << endl;
    cout << "================================" << endl;
    cout << "Tiempo Inicial:     " << temporizador_principal.formatoTextoInicio() << endl;
    cout << "================================" << endl;
    cout << endl;

    vector<thread> hilos;
    vector<pair<int, string>> tiempos_terminados;

    for (int i = 1; i <= copias; ++i)
    {
        thread hilo = crearHiloDeProceso(i, temporizador_principal, mutex_temporizador, tiempos_terminados);
        hilos.push_back(move(hilo));
    }

    for (auto &hilo : hilos)
    {
        if (hilo.joinable())
            hilo.join();
    }

    temporizador_principal.detener();

    // Mostrar los tiempos en el orden en que terminaron los procesos
    for (const auto &par : tiempos_terminados)
    {
        int id = par.first;
        const string &duracion = par.second;

        cout << "TIEMPO PROCESO " << id << ": ";
        if (id < 10)
            cout << " ";
        cout << duracion << endl;
        cout << "--------------------------------" << endl;
    }

    cout << "================================" << endl;
    cout << "      FIN PROCESO PARALELO      " << endl;
    cout << "================================" << endl;
    cout << "Tiempo Final:       " << temporizador_principal.formatoTextoFin() << endl;
    cout << "Tiempo Total:       " << temporizador_principal.tiempoTranscurrido() << endl;
    cout << "Tiempo promedio:    " << temporizador_principal.promedioPorProceso() << endl;
    cout << "================================" << endl;
    return temporizador_principal;
}

#endif // F06_MAIN_PARALELO_H
